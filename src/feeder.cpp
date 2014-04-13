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
#include "moviefetchjob.h"

#include <QTimer>
#include <QDebug>
#include <QRegularExpression>
#include <QFileInfo>

#include <baloo/query.h>
#include <baloo/resultiterator.h>

#include <QSqlQuery>
#include <QSqlError>

using namespace Jungle;

Feeder::Feeder(Database* db, QObject* parent)
    : QObject(parent)
    , m_db(db)
{
    QTimer::singleShot(0, this, SLOT(fetchFiles()));
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
        int id = it.id().mid(QByteArray("file:").size()).toInt();

        if (!m_db->hasVideo(id)) {
            m_files << it.url().toLocalFile();
        }
    }

    if (!m_files.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(processNext()));
    } else {
        qDebug() << "No files to process";
    }
}

QString Feeder::filterFileName(const QString& fileName)
{
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
        return QString();
    }

    QString name = fileName;

    // Stupid hueristic
    QStringList fillers;
    fillers << allowedVideoTypes;
    fillers << "." << "-" << "[" << "]" << "(" << ")"
            << "hdtv" << "x264" << "LOL" << "720p" << "1080p"
            << "BluRay" << "BRRIP" << "xvid" << "YIFY" << "VTV" << "KILLERS"
            << "webrip" << "DVDScr" << "EXCELLENCE" << "juggs" << "dvdrip"
            << "MP3" << "RARBG"
            << "eng" << "bellatrix";

    foreach (const QString& f, fillers) {
        name.replace(f, " ", Qt::CaseInsensitive);
    }

    return name.simplified();
}

bool Feeder::filterUrl(const QString& url)
{
    QFileInfo info(url);

    // A video should be at least 100mb
    if (info.size() <= 100 * 1024 * 1024) {
        return true;
    }

    return false;
}

void Feeder::processNext()
{
    const QString url = m_files.takeLast();
    if (filterUrl(url)) {
        if (!m_files.isEmpty())
            QTimer::singleShot(0, this, SLOT(processNext()));
        return;
    }

    QString fileName = QUrl::fromLocalFile(url).fileName();
    fileName = filterFileName(fileName);

    if (fileName.isEmpty()) {
        if (!m_files.isEmpty())
            QTimer::singleShot(0, this, SLOT(processNext()));
        return;
    }

    //
    // Check if tv show or movie
    //
    QRegularExpression tvshowRegexp("\\b[Ss]([\\d]{1,2})[Ee]([\\d]{1,2})\\b");
    QRegularExpressionMatch match = tvshowRegexp.match(fileName);
    if (match.hasMatch()) {
        int season = match.captured(1).toInt();
        int episode = match.captured(2).toInt();

        Q_UNUSED(season);
        Q_UNUSED(episode);
        fileName.replace(tvshowRegexp, "");

        // TODO: Implement tv show fetching
        qDebug() << fileName.simplified() << season << episode;
        if (!m_files.isEmpty())
            QTimer::singleShot(0, this, SLOT(processNext()));
        return;
    }

    //
    // Must be a movie otherwise
    //
    int year = 0;

    QRegularExpression yearRegexp("\\b([\\d]{4})\\b");
    QRegularExpressionMatch m = yearRegexp.match(fileName);
    if (m.isValid()) {
        year = m.captured(1).toInt();
        fileName.replace(yearRegexp, "");
    }

    auto job = new MovieFetchJob(url, fileName, year, this);
    connect(job, &MovieFetchJob::result, this, &Feeder::slotResult);
}

void Feeder::slotResult(MovieFetchJob* job)
{
    // Mark the url as processed
    m_db->addVideo(job->url());

    // Add data if there is any
    if (job->id() == 0) {
        if (!m_files.isEmpty())
            QTimer::singleShot(0, this, SLOT(processNext()));
        return;
    }
    job->deleteLater();

    Movie movie;
    movie.setId(job->id());
    movie.setUrl(job->url());
    movie.setTitle(job->title());
    movie.setReleaseDate(job->releaseDate());
    movie.setPosterUrl(job->posterUrl());

    m_db->addMovie(movie);

    if (!m_files.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(processNext()));
    }
}
