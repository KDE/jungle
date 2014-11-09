/*
 * Copyright (C) 2014  Vishesh Handa <vhanda@kde.org>
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

#include "databaseconsumer.h"
#include "database.h"
#include "queueinterface.h"

#include <QDebug>

using namespace Jungle;

DatabaseConsumer::DatabaseConsumer()
{
    m_db = Database::instance();
}

static QVariantMap merge(const QVariantMap& left, const QVariantMap& right)
{
    QVariantMap map = left;
    for (auto it = right.constBegin(); it != right.constEnd(); it++) {
        map.insert(it.key(), it.value());
    }

    return map;
}

void DatabaseConsumer::itemsAdded(QueueInterface* queue)
{
    QVariantMap item = queue->head();

    //
    // Merge based on URL
    const QString url = item.value("url").toString();
    QVariantMap prevItem = m_db->item("url", url);
    if (!prevItem.isEmpty()) {
        item = merge(prevItem, item);
    }

    const QString type = item.value("type").toString();
    if (type == QStringLiteral("tvshow")) {
        QString id = item.value("id").toString();
        if (!id.isEmpty()) {
            QVariantMap query = {{"type", "tvshow"},
                                 {"id", id}};
            QVariantMap existingItem = m_db->query(query);
            item = merge(existingItem, item);
        }
    }
    else if (type == QStringLiteral("tvseason")) {
        QString showId = item.value("showId").toString();
        int season = item.value("seasonNumber").toInt();
        if (!showId.isEmpty() && season) {
            QVariantMap query = {{"type", "tvseason"},
                                 {"showId", showId},
                                 {"season", season}};
            QVariantMap existingItem = m_db->query(query);
            item = merge(existingItem, item);
        }
    }
    else if (type == QStringLiteral("tvepisode")) {
        QString showId = item.value("showId").toString();
        int season = item.value("season").toInt();
        int ep = item.value("episodeNumber").toInt();
        if (!showId.isEmpty() && season) {
            QVariantMap query = {{"type", "tvepisode"},
                                 {"showId", showId},
                                 {"season", season},
                                 {"episodeNumber", ep}};
            QVariantMap existingItem = m_db->query(query);
            item = merge(existingItem, item);
        }
    }

    if (item.value("delete").toBool() && item.contains("_id")) {
        m_db->remove(item.value("_id").toString());
    }
    else {
        m_db->add(item);
    }

    queue->dequeue();
}
