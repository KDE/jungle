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

#include "tvseasonfetchjob.h"

#include <tmdbqt/tvseasondblist.h>
#include <tmdbqt/tvseasoninfojob.h>
#include <tmdbqt/tvepisodedblist.h>

#include <QStandardPaths>
#include <QNetworkRequest>
#include <QFile>
#include <QTimer>

using namespace Jungle;

TvSeasonFetchJob::TvSeasonFetchJob(TmdbQt::TheMovieDbApi* api, int show, int season, QObject* parent)
    : QObject(parent)
    , m_api(api)
    , m_showId(show)
    , m_seasonNum(season)
{
    TmdbQt::TvSeasonInfoJob* job = m_api->getTvSeasonInfo(show, season);
    connect(job, SIGNAL(result(TmdbQt::TvSeasonInfoJob*)),
            this, SLOT(slotResult(TmdbQt::TvSeasonInfoJob*)));
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));
}

TvSeason TvSeasonFetchJob::result() const
{
    return m_season;
}

void TvSeasonFetchJob::slotResult(TmdbQt::TvSeasonInfoJob* job)
{
    TmdbQt::TvSeasonDb sdb = job->result();

    m_season.setAirDate(sdb.airDate());
    m_season.setId(sdb.id());
    m_season.setOverview(sdb.overview());
    m_season.setSeasonNumber(sdb.seasonNumber());

    TmdbQt::TvEpisodeDbList epList = sdb.episodes();
    for (int i = 0; i < epList.size(); i++) {
        TmdbQt::TvEpisodeDb ep = epList[i];

        QVariantMap episode;
        episode["airDate"] = ep.airDate();
        episode["episodeNumber"] = ep.episodeNumber();
        episode["name"] = ep.name();
        episode["overview"] = ep.overview();
        episode["season"] = m_seasonNum;
        episode["show"] = m_showId;

        m_episodes << episode;

        QUrl stillUrl = ep.stillUrl(QLatin1String("w342"));
        QNetworkReply* reply = m_network.get(QNetworkRequest(stillUrl));
        reply->setProperty("episodeNum", ep.episodeNumber());
        reply->setProperty("index", i);
    }

    m_pendingJobs = epList.size();
}

void TvSeasonFetchJob::slotNetworkReply(QNetworkReply* reply)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    int episodeNum = reply->property("episodeNum").toInt();
    QString url = QString::fromLatin1("%1/jungle/tvshow-%2-season%3-ep%4")
                  .arg(dataDir).arg(m_showId).arg(m_seasonNum).arg(episodeNum);

    QByteArray data = reply->readAll();

    bool containsData = !data.isEmpty();
    if (data.size() < 100 || data.contains("Format not Supported"))
        containsData = false;

    if (containsData) {
        QFile file(url);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
    }
    else {
        url.clear();
    }

    int index = reply->property("index").toInt();
    m_episodes[index]["stillPath"] = url;

    m_pendingJobs--;
    if (m_pendingJobs == 0) {
        emit result(this);
    }
}

QList<QVariantMap> TvSeasonFetchJob::episodes() const
{
    return m_episodes;
}

