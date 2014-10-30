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

#include "queue.h"
#include "jsonquery.h"

#include <QDebug>
#include <QStandardPaths>

using namespace Jungle;

Queue::Queue(const QString& name)
{
    static QString jungleDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle/";

    Q_ASSERT(!name.isEmpty());
    m_db = new JsonDatabase();
    m_db->setPath(jungleDir + QStringLiteral("queue_") + name);
    m_coll = m_db->collection("queue");
}

Queue::~Queue()
{
    delete m_db;
}

void Queue::add(const QVariantMap& input)
{
    m_coll.insert(input);
    m_consumer->itemsAdded(this);
}

bool Queue::empty()
{
    JsonQuery q = m_coll.execute(QVariantMap());
    return q.totalCount();
}

void Queue::pop()
{
    QVariantMap t = top();
    QString id = t.value("_id").toString();
    m_coll.remove(id);
}

void Queue::setConsumer(ConsumerInterface* consumer)
{
    m_consumer = consumer;
}

QVariantMap Queue::top()
{
    JsonQuery q = m_coll.execute(QVariantMap());
    if (q.next()) {
        return q.result();
    }

    Q_ASSERT_X(0, "", "Queue::top called when empty!");
    return QVariantMap();
}

