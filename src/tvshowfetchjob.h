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

#ifndef TVSHOWFETCHJOB_H
#define TVSHOWFETCHJOB_H

#include <QObject>

#include "job.h"
#include <tmdbqt/themoviedbapi.h>

namespace Jungle {

class TvShowFetchJob : public Job
{
    Q_OBJECT
public:
    explicit TvShowFetchJob(TmdbQt::TheMovieDbApi* api, const QString& name, QObject* parent = 0);
    virtual ~TvShowFetchJob();

    QVariantMap data() const { return m_show; }
    QList<QVariantMap> seasons() const { return m_seasons; }

private slots:
    void slotResult(TmdbQt::TvSearchJob* job);
    void slotResult(TmdbQt::TvShowInfoJob* job);

private:
    TmdbQt::TheMovieDbApi* m_api;
    QString m_name;

    QVariantMap m_show;
    QList<QVariantMap> m_seasons;
};

}
#endif // TVSHOWFETCHJOB_H
