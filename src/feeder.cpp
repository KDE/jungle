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
#include "tvshowfetchjob.h"

#include <QTimer>
#include <QDebug>
#include <QRegularExpression>
#include <QFileInfo>
#include <qeventloop.h>

#include <baloo/query.h>
#include <baloo/resultiterator.h>

#include <QSqlQuery>
#include <QSqlError>

using namespace Jungle;

Feeder::Feeder(Database* db, QObject* parent)
    : QObject(parent)
    , m_db(db)
{
    m_theMovieDb = new TheMovieDbStore(this);
    connect(m_theMovieDb, SIGNAL(initialized()),
            this, SLOT(fetchFiles()));
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

    QStringList basicFilters;
    basicFilters << "." << "-" << "[" << "]" << "(" << ")" << "{" << "}";
    foreach (const QString& f, basicFilters) {
        name.replace(f, " ");
    }

    // Stupid hueristic
    QStringList fillers;
    fillers << allowedVideoTypes;
    fillers << "hdtv" << "x264" << "LOL" << "720p" << "1080p" << "480p"
            << "BluRay" << "BRRIP" << "xvid" << "YIFY" << "VTV" << "KILLERS"
            << "webrip" << "DVDScr" << "EXCELLENCE" << "juggs" << "dvdrip"
            << "MP3" << "RARBG" << "DIMENSION" << "NTb" << "DD5" << "2HD" << "NF"
            << "REMARKABLE" << "PROPER" << "AFG" << "bdrip" << "miguel" << "axxo"
            << "occor" << "1337x" << "noir" << "Hindi" << "ViZNU" << "AAC"
            << "eng" << "bellatrix" << "hon3y" << "vk007";

    foreach (const QString& f, fillers) {
        QRegularExpression exp("\\b" + f + "\\b", QRegularExpression::CaseInsensitiveOption);
        name.replace(exp, " ");
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

        fileName.replace(tvshowRegexp, "");
        fileName = fileName.simplified();

        int showId = fetchOrCreateShow(fileName);
        if (!showId) {
            if (!m_files.isEmpty())
                QTimer::singleShot(0, this, SLOT(processNext()));
            return;
        }

        if (!m_db->hasEpisodes(showId, season)) {
            TvSeasonFetchJob* job = m_theMovieDb->fetchTvSeason(showId, season);

            QEventLoop loop;
            connect(job, SIGNAL(result(TvSeasonFetchJob*)),
                    &loop, SLOT(quit()));
            loop.exec();

            TvSeason tvseason = job->result();
            foreach (const TvEpisode& ep, tvseason.episodes()) {
                m_db->addEpisode(showId, season, ep);
            }
        }

        TvEpisode ep = m_db->episode(showId, season, episode);
        if (ep.episodeNumber() == episode) {
            ep.setUrl(url);
            m_db->addEpisode(showId, season, ep);
        }
        qDebug() << fileName.simplified() << season << episode;
        if (!m_files.isEmpty()) {
            QTimer::singleShot(0, this, SLOT(processNext()));
        }
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
        fileName = fileName.simplified();
    }

    auto job = m_theMovieDb->fetchMovie(url, fileName, year);
    connect(job, SIGNAL(result(MovieFetchJob*)),
            this, SLOT(slotResult(MovieFetchJob*)));
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

int Feeder::fetchOrCreateShow(const QString& showName)
{
    int showId = m_db->showId(showName);
    if (showId)
        return showId;

    TvShowFetchJob* job = m_theMovieDb->fetchTvShow(showName);

    QEventLoop loop;
    connect(job, SIGNAL(result(TvShowFetchJob*)),
            &loop, SLOT(quit()));
    loop.exec();

    Show show = job->result();
    if (show.id()) {
        m_db->addShow(show);
    }

    return show.id();
}

