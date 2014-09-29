/*
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

#ifndef JUNGLE_TVDBJOB_H
#define JUNGLE_TVDBJOB_H

#include <QObject>
#include "interfaces/dataqueueinterface.h"

#include <tmdbqt/searchjob.h>
#include <tmdbqt/themoviedbapi.h>

namespace Jungle {

class TvDbJob : public QObject
{
    Q_OBJECT
public:
    TvDbJob(TmdbQt::TheMovieDbApi& api, const QVariantMap& input,
            QList<DataQueueInterface*> queues);

    void start();

private Q_SLOTS:
     void slotMovieResult(TmdbQt::SearchJob*);

private:
    QVariantMap m_inputData;
    QList<DataQueueInterface*> m_queues;

    TmdbQt::TheMovieDbApi& m_api;
};

}

#endif // JUNGLE_TVDBJOB_H
