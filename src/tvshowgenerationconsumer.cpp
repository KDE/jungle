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

#include <QUuid>
#include <QDebug>

using namespace Jungle;

TvShowGenerationConsumer::TvShowGenerationConsumer(const QList<QueueInterface*> output)
    : m_outputQueues(output)
{
}

void TvShowGenerationConsumer::itemsAdded(QueueInterface* queue)
{
    QVariantMap map = queue->top();

    if (map.value("type").toString() != QStringLiteral("tvepisode")) {
        queue->pop();
        return;
    }

    QString series = map.value("series").toString();
    int season = map.value("season").toInt();

    // FIXME: This complicated id generation was supposed to be for _id
    //        But that doesn't seem to work. It still gets a new id
    QString showId = QUuid::createUuid().toString();
    showId.remove('-');
    showId = showId.mid(1, 25);

    QVariantMap tvshow;
    tvshow["type"] = QStringLiteral("tvshow");
    tvshow["id"] = showId;
    tvshow["title"] = series;

    QVariantMap tvseason;
    tvseason["type"] = QStringLiteral("tvseason");
    tvseason["season"] = season;
    tvseason["showId"] = showId;

    map["showId"] = showId;

    for (auto q : m_outputQueues)
        q->add(tvshow);
    for (auto q : m_outputQueues)
        q->add(tvseason);
    for (auto q : m_outputQueues)
        q->add(map);

    queue->pop();
}
