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

#ifndef JUNGLE_TVSEASONFETCHJOB_H
#define JUNGLE_TVSEASONFETCHJOB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "show.h"
#include "tvseason.h"
#include "tvepisode.h"

#include <tmdbqt/themoviedbapi.h>

namespace Jungle {

class TvSeasonFetchJob : public QObject
{
    Q_OBJECT
public:
    TvSeasonFetchJob(TmdbQt::TheMovieDbApi* api, int show, int season, QObject* parent = 0);

    TvSeason result() const;

signals:
    void result(TvSeasonFetchJob* job);

private slots:
    void slotResult(TmdbQt::TvSeasonInfoJob* job);
    void slotNetworkReply(QNetworkReply* reply);

private:
    TmdbQt::TheMovieDbApi* m_api;
    QNetworkAccessManager m_network;

    int m_showId;
    int m_seasonNum;
    int m_pendingJobs;

    TvSeason m_season;
    QList<TvEpisode> m_episodes;
};

}


#endif // JUNGLE_TVSEASONFETCHJOB_H
