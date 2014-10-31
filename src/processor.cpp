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

#include "processor.h"
#include "guessitconsumer.h"
#include "movieconsumer.h"
#include "tvshowconsumer.h"
#include "tvseasonconsumer.h"
#include "databaseconsumer.h"
#include "tvshowgenerationconsumer.h"
#include "networkimageconsumer.h"
#include "themoviedbstore.h"

#include <QVariantMap>
#include <QDebug>

using namespace Jungle;

Processor::Processor()
    : m_guessItQueue("guessit")
    , m_tvShowGenQueue("tvshowgen")
    , m_movieQueue("movie")
    , m_tvShowQueue("tvshow")
    , m_tvSeasonQueue("tvseason")
    , m_networkImageQueue("networkimage")
    , m_saveQueue("save")
{
    QList<QueueInterface*> giOutputQ = {&m_saveQueue, &m_movieQueue, &m_tvShowGenQueue};
    GuessItConsumer* guessItConsumer = new GuessItConsumer(giOutputQ);
    m_guessItQueue.setConsumer(guessItConsumer);

    QList<QueueInterface*> tvGenOutputQ = {&m_saveQueue, &m_tvShowQueue};
    TvShowGenerationConsumer* tvGenCon = new TvShowGenerationConsumer(tvGenOutputQ);
    m_tvShowGenQueue.setConsumer(tvGenCon);

    TheMovieDbStore* movieDbApi = new TheMovieDbStore();

    QList<QueueInterface*> movieOutputQ = {&m_saveQueue, &m_networkImageQueue};
    MovieConsumer* movieConsumer = new MovieConsumer(movieDbApi, movieOutputQ);
    m_movieQueue.setConsumer(movieConsumer);

    QList<QueueInterface*> tvShowOutputQ = {&m_saveQueue, &m_tvSeasonQueue, &m_networkImageQueue};
    TvShowConsumer* tvShowConsumer = new TvShowConsumer(movieDbApi, tvShowOutputQ);
    m_tvShowQueue.setConsumer(tvShowConsumer);

    QList<QueueInterface*> tvSeasonOutputQ = {&m_saveQueue, &m_networkImageQueue};
    TvSeasonConsumer* tvSeasonConsumer = new TvSeasonConsumer(movieDbApi, tvSeasonOutputQ);
    m_tvSeasonQueue.setConsumer(tvSeasonConsumer);

    QList<QueueInterface*> netOutQ = {&m_saveQueue};
    NetworkImageConsumer* netImgConsumer = new NetworkImageConsumer(netOutQ);
    m_networkImageQueue.setConsumer(netImgConsumer);

    DatabaseConsumer* dbConsumer = new DatabaseConsumer();
    m_saveQueue.setConsumer(dbConsumer);

    m_queues << &m_guessItQueue << &m_tvShowGenQueue << &m_tvSeasonQueue
             << &m_tvShowGenQueue << &m_movieQueue << &m_networkImageQueue
             << &m_saveQueue;
}

Processor::~Processor()
{
}

void Processor::addFile(const QString& filePath)
{
    Q_ASSERT(!filePath.isEmpty());

    QVariantMap map;
    map.insert("url", filePath);

    m_guessItQueue.enqueue(map);
}

void Processor::resume()
{
    for (Queue* q : m_queues) {
        if (!q->empty()) {
            auto consumer = q->consumer();
            consumer->itemsAdded(q);
        }
    }
}
