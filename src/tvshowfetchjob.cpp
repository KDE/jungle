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

#include <tmdbqt/tvsearchjob.h>
#include <tmdbqt/tvshowdblist.h>
#include <tmdbqt/tvshowinfojob.h>
#include <tmdbqt/tvseasondblist.h>
#include <tmdbqt/tvseasoninfojob.h>
#include <tmdbqt/tvepisodedblist.h>

#include <QDebug>
#include <QUrl>

using namespace Jungle;

TvShowFetchJob::TvShowFetchJob(TmdbQt::TheMovieDbApi* api, const QString& name, QObject* parent)
    : Job(parent)
    , m_api(api)
    , m_name(name)
{
    TmdbQt::TvSearchJob* job = m_api->searchTvShow(name);
    connect(job, SIGNAL(result(TmdbQt::TvSearchJob*)),
            this, SLOT(slotResult(TmdbQt::TvSearchJob*)));

    qDebug() << "TvShow:" << name;
}

TvShowFetchJob::~TvShowFetchJob()
{
}

void TvShowFetchJob::slotResult(TmdbQt::TvSearchJob* job)
{
    TmdbQt::TvShowDbList shows = job->result();
    if (shows.isEmpty()) {
        emitFinished();
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

    m_show["title"] = tvshow.name();
    m_show["releaseDate"] = tvshow.firstAiredDate();
    m_show["movieDbId"] = tvshow.id();

    QUrl posterUrl = tvshow.posterUrl(QLatin1String("w342"));
    m_show["posterPath"] = posterUrl;

    TmdbQt::TvSeasonDbList seasons = tvshow.seasons();
    for (int i = 0; i < seasons.size(); i++) {
        TmdbQt::TvSeasonDb sdb = seasons[i];

        QVariantMap season;
        season["airDate"] = sdb.airDate();
        season["movieDbId"] = sdb.id();
        season["seasonNumber"] = sdb.seasonNumber();
        season["show"] = tvshow.id();

        QUrl posterUrl = sdb.posterUrl(QLatin1String("w342"));
        season["posterPath"] = posterUrl;

        m_seasons << season;
    }

    emitFinished();
}
