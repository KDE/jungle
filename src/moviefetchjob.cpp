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

#include "moviefetchjob.h"

#include <QUrl>
#include <QTimer>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>

#include <tmdbqt/searchjob.h>

// Issued to vhanda for personal use
static const char* s_key = "d27948732458af6587dbc9b9764aad37";

MovieFetchJob::MovieFetchJob(const QString& url, QObject* parent)
    : QObject(parent)
    , m_api(QString::fromLatin1(s_key))
    , m_url(url)
    , m_id(0)
{
    qDebug() << url;
    connect(&m_api, SIGNAL(initialized()), this, SLOT(slotInitialized()));
}

void MovieFetchJob::slotInitialized()
{
    if (filterUrl(m_url)) {
        emit result(this);
        return;
    }

    const QString fileName = QUrl::fromLocalFile(m_url).fileName();

    QString name;
    int year = 0;

    if (!fetchNameAndYear(fileName, name, year)) {
        emit result(this);
        return;
    }

    TmdbQt::SearchJob* job = m_api.searchMovie(name, year);
    connect(job, SIGNAL(result(TmdbQt::SearchJob*)),
            this, SLOT(slotMovieResult(TmdbQt::SearchJob*)));
}

void MovieFetchJob::slotMovieResult(TmdbQt::SearchJob* job)
{
    // FIXME: Try match the year, if the year matches, then it is probably the
    //        same movie
    TmdbQt::MovieDbList list = job->result();
    if (list.size() != 1) {
        emit result(this);
        return;
    }

    TmdbQt::MovieDb movie = list.first();
    m_id = movie.id();
    m_title = movie.title();
    m_date = movie.releaseDate();
    m_posterUrl = movie.posterUrl(QLatin1String("w32")).toLocalFile();

    qDebug() << m_id << m_title << m_date << m_posterUrl;
    // FIXME: The poster needs to be fetched
    emit result(this);
}

bool MovieFetchJob::filterUrl(const QString& url)
{
    QFileInfo info(url);

    // A movie should be at least 100mb
    if (info.size() <= 100 * 1024 * 1024) {
        return true;
    }

    return false;
}

bool MovieFetchJob::fetchNameAndYear(const QString& fileName, QString& name, int& year)
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

