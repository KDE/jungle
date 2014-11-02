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

#include "tvshowgenerationconsumer.h"
#include "queueinterface.h"
#include "database.h"

#include <QUuid>
#include <QDebug>

using namespace Jungle;

TvShowGenerationConsumer::TvShowGenerationConsumer(const QList<QueueInterface*> output)
    : m_outputQueues(output)
{
}

void TvShowGenerationConsumer::itemsAdded(QueueInterface* queue)
{
    QVariantMap map = queue->head();

    if (map.value("type").toString() != QStringLiteral("tvepisode")) {
        queue->dequeue();
        return;
    }

    QString series = map.value("series").toString();
    if (series.isEmpty()) {
        queue->dequeue();
        return;
    }

    Database* db = Database::instance();
    QString showId = db->showId(series);
    if (showId.isEmpty()) {
        // FIXME: This complicated id generation was supposed to be for _id
        //        But that doesn't seem to work. It still gets a new id
        showId = QUuid::createUuid().toString();
        showId.remove('-');
        showId = showId.mid(1, 25);

        QVariantMap tvshow;
        tvshow["type"] = QStringLiteral("tvshow");
        tvshow["id"] = showId;
        tvshow["title"] = series;

        for (auto q : m_outputQueues)
            q->enqueue(tvshow);
    }

    map["showId"] = showId;
    for (auto q : m_outputQueues)
        q->enqueue(map);

    queue->dequeue();
}
