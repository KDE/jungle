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

#include "job.h"
#include <tmdbqt/themoviedbapi.h>

namespace Jungle {

class TvSeasonFetchJob : public Job
{
    Q_OBJECT
public:
    TvSeasonFetchJob(TmdbQt::TheMovieDbApi* api, int show, int season,
                     const QString& showid, QObject* parent = 0);

    QVariantMap data() const;
    QList<QVariantMap> extraData() const;

private slots:
    void slotResult(TmdbQt::TvSeasonInfoJob* job);

private:
    TmdbQt::TheMovieDbApi* m_api;

    int m_showId;
    int m_seasonNum;
    int m_pendingJobs;

    QString m_databaseShowId;

    QVariantMap m_season;
    QList<QVariantMap> m_episodes;
};

}


#endif // JUNGLE_TVSEASONFETCHJOB_H
