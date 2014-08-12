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

#include "tvshowfetchjob.h"
#include "tvseason.h"
#include "tvepisode.h"

#include <tmdbqt/tvsearchjob.h>
#include <tmdbqt/tvshowdblist.h>
#include <tmdbqt/tvshowinfojob.h>
#include <tmdbqt/tvseasondblist.h>
#include <tmdbqt/tvseasoninfojob.h>
#include <tmdbqt/tvepisodedblist.h>

#include <QStandardPaths>
#include <QNetworkRequest>
#include <QFile>
#include <QTimer>

using namespace Jungle;

TvShowFetchJob::TvShowFetchJob(TmdbQt::TheMovieDbApi* api, const QString& name, QObject* parent)
    : QObject(parent)
    , m_api(api)
    , m_name(name)
{
    TmdbQt::TvSearchJob* job = m_api->searchTvShow(name);
    connect(job, SIGNAL(result(TmdbQt::TvSearchJob*)),
            this, SLOT(slotResult(TmdbQt::TvSearchJob*)));
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));

    qDebug() << "TvShow:" << name;
}

TvShowFetchJob::~TvShowFetchJob()
{
}

void TvShowFetchJob::slotResult(TmdbQt::TvSearchJob* job)
{
    TmdbQt::TvShowDbList shows = job->result();
    if (shows.isEmpty()) {
        emit result(this);
        return;
    }

    TmdbQt::TvShowDb tvshow = shows.first();
    TmdbQt::TvShowInfoJob* infoJob = m_api->getTvShowInfo(tvshow.id());
    connect(infoJob, SIGNAL(result(TmdbQt::TvShowInfoJob*)),
            this, SLOT(slotResult(TmdbQt::TvShowInfoJob*)));
}

void TvShowFetchJob::slotResult(TmdbQt::TvShowInfoJob* job)
{
    TmdbQt::TvShowDb tvshow = job->result();

    m_show.setTitle(tvshow.name());
    m_show.setFirstAired(tvshow.firstAiredDate());
    m_show.setId(tvshow.id());

    TmdbQt::TvSeasonDbList seasons = tvshow.seasons();
    for (int i = 0; i < seasons.size(); i++) {
        TmdbQt::TvSeasonDb season = seasons[i];

        TvSeason s;
        s.setAirDate(season.airDate());
        s.setId(season.id());
        s.setSeasonNumber(season.seasonNumber());

        m_seasons << s;

        QUrl posterUrl = season.posterUrl(QLatin1String("w342"));
        QNetworkReply* reply = m_network.get(QNetworkRequest(posterUrl));
        reply->setProperty("seasonNum", s.seasonNumber());
        reply->setProperty("index", i);
    }

    QUrl posterUrl = tvshow.posterUrl(QLatin1String("w342"));
    QNetworkReply* reply = m_network.get(QNetworkRequest(posterUrl));
    reply->setProperty("showPoster", posterUrl);

    m_pendingJobs = seasons.size() + 1;
}

void TvShowFetchJob::slotNetworkReply(QNetworkReply* reply)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    if (reply->property("showPoster").isValid()) {
        QString url = dataDir + "/jungle/tvshow-" + QString::number(m_show.id());

        QFile file(url);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        m_show.setCoverUrl(url);
    }
    else {
        int seasonNum = reply->property("seasonNum").toInt();
        QString url = QString::fromLatin1("%1/jungle/tvshow-%2-season%3")
                      .arg(dataDir).arg(m_show.id()).arg(seasonNum);

        QFile file(url);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        int index = reply->property("index").toInt();
        m_seasons[index].setPosterUrl(url);
    }

    m_pendingJobs--;
    if (!m_pendingJobs) {
        m_show.setTvSeasons(m_seasons);
        emit result(this);
    }
}
