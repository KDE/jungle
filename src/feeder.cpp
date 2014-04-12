/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "feeder.h"

#include <QTimer>
#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>

#include <baloo/query.h>
#include <baloo/resultiterator.h>

#include <tmdbqt/searchjob.h>

#include <QSqlQuery>
#include <QSqlError>

// Issued to vhanda for personal use
static const char* s_key = "d27948732458af6587dbc9b9764aad37";

Feeder::Feeder(QSqlDatabase& sqlDb, QObject* parent)
    : QObject(parent)
    , m_api(QString::fromLatin1(s_key))
    , m_sqlDb(sqlDb)
{
    connect(&m_api, SIGNAL(initialized()), this, SLOT(fetchFiles()));
}

Feeder::~Feeder()
{

}

void Feeder::fetchFiles()
{
    Baloo::Query query;
    query.setType("Video");

    auto it = query.exec();
    while (it.next()) {
        m_files << it.url().toLocalFile();
    }

    QTimer::singleShot(0, this, SLOT(processNext()));
}

void Feeder::processNext()
{
    const QString url = m_files.takeLast();
    if (filterUrl(url)) {
        if (!m_files.isEmpty()) {
            QTimer::singleShot(0, this, SLOT(processNext()));
        }
        return;
    }

    const QString fileName = QUrl::fromLocalFile(url).fileName();

    QString name;
    int year = 0;

    if (!fetchNameAndYear(fileName, name, year)) {
        if (!m_files.isEmpty()) {
            QTimer::singleShot(0, this, SLOT(processNext()));
        }
        return;
    }

    qDebug() << name << year;

    TmdbQt::SearchJob* job = m_api.searchMovie(name, year);
    connect(job, SIGNAL(result(TmdbQt::SearchJob*)),
            this, SLOT(slotMovieResult(TmdbQt::SearchJob*)));
}

void Feeder::slotMovieResult(TmdbQt::SearchJob* job)
{
    // FIXME: Try match the year, if the year matches, then it is probably the
    //        same movie
    TmdbQt::MovieDbList list = job->result();
    if (list.size() != 1) {
        qDebug() << "Have no received a single result";
        if (!m_files.isEmpty()) {
            QTimer::singleShot(0, this, SLOT(processNext()));
        }
        return;
    }

    TmdbQt::MovieDb movie = list.first();

    qDebug() << movie.id() << movie.title() << movie.releaseDate();

    //
    // FIXME: The poster needs to be fetched and saved somewhere?
    //

    // Push into the db
    QSqlQuery query;
    query.prepare("insert into movies (mid, title, releaseDate, posterPath) "
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(movie.id());
    query.addBindValue(movie.title());
    query.addBindValue(movie.releaseDate());
    query.addBindValue(movie.posterPath());

    if (!query.exec()) {
        qDebug() << query.lastError();
    }

    if (!m_files.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(processNext()));
    }
}

bool Feeder::filterUrl(const QString& url)
{
    QFileInfo info(url);

    // A movie should be at least 100mb
    if (info.size() <= 100 * 1024 * 1024) {
        return true;
    }

    return false;
}

bool Feeder::fetchNameAndYear(const QString& fileName, QString& name, int& year)
{
    // FIXME: Use the actual mimetype
    QStringList allowedVideoTypes;
    allowedVideoTypes << "mp4" << "avi" << "mkv";

    bool found = false;
    foreach (const QString& type, allowedVideoTypes) {
        if (fileName.endsWith(type)) {
            found = true;
            break;
        }
    }

    if (!found) {
        return false;
    }

    name = fileName;

    // Stupid hueristic
    QStringList fillers;
    fillers << allowedVideoTypes;
    fillers << "." << "-" << "[" << "]" << "(" << ")"
            << "hdtv" << "x264" << "LOL" << "720p" << "1080p"
            << "BluRay" << "BRRIP" << "xvid" << "YIFY" << "VTV" << "KILLERS"
            << "webrip" << "DVDScr" << "EXCELLENCE" << "juggs" << "dvdrip"
            << "eng" << "bellatrix";

    foreach (const QString& f, fillers) {
        name.replace(f, " ", Qt::CaseInsensitive);
    }

    name = name.simplified();

    // Remove the movie year if present
    QRegularExpression yearRegexp("\\b([\\d]{4})\\b");
    QRegularExpressionMatch m = yearRegexp.match(name);
    if (m.isValid()) {
        year = m.captured(1).toInt();
        name.replace(yearRegexp, "");
    }

    QRegularExpression tvshowRegexp("\\b[Ss][\\d]{1,2}[Ee][\\d]{1,2}\\b");
    if (name.contains(tvshowRegexp)) {
        return false;
    }

    name = name.simplified();
    // TODO: Check the length of the file. If < 2 minutes, then ignore

    return true;
}
