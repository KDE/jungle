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

#include <QDebug>
#include <QUrl>

using namespace Jungle;

TvSeasonFetchJob::TvSeasonFetchJob(TmdbQt::TheMovieDbApi* api, int show, int season,
                                   const QString& showId, QObject* parent)
    : Job(parent)
    , m_api(api)
    , m_showId(show)
    , m_seasonNum(season)
    , m_databaseShowId(showId)
{
    qDebug() << show << season;
    TmdbQt::TvSeasonInfoJob* job = m_api->getTvSeasonInfo(show, season);
    connect(job, SIGNAL(result(TmdbQt::TvSeasonInfoJob*)),
            this, SLOT(slotResult(TmdbQt::TvSeasonInfoJob*)));
}

void TvSeasonFetchJob::slotResult(TmdbQt::TvSeasonInfoJob* job)
{
    TmdbQt::TvSeasonDb sdb = job->result();

    m_season["type"] = QStringLiteral("tvseason");
    m_season["airDate"] = sdb.airDate();
    m_season["movieDbId"] = sdb.id();
    m_season["overview"] = sdb.overview();
    m_season["seasonNumber"] = sdb.seasonNumber();
    m_season["showId"] = m_databaseShowId;

    TmdbQt::TvEpisodeDbList epList = sdb.episodes();
    for (int i = 0; i < epList.size(); i++) {
        TmdbQt::TvEpisodeDb ep = epList[i];

        QVariantMap episode;
        episode["type"] = QStringLiteral("tvepisode");
        episode["airDate"] = ep.airDate();
        episode["episodeNumber"] = ep.episodeNumber();
        episode["name"] = ep.name();
        episode["overview"] = ep.overview();
        episode["season"] = m_seasonNum;
        episode["movieDbShowId"] = m_showId;
        episode["showId"] = m_databaseShowId;

        QUrl stillUrl = ep.stillUrl(QLatin1String("w342"));
        episode["stillPath"] = stillUrl;

        m_episodes << episode;
    }

    m_pendingJobs = epList.size();
    emitFinished();
}

QVariantMap TvSeasonFetchJob::data() const
{
    return m_season;
}

QList<QVariantMap> TvSeasonFetchJob::extraData() const
{
    return m_episodes;
}

