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
#include <QDir>

using namespace Jungle;

Queue::Queue(const QString& name)
    : m_consumer(0)
{
    static QString jungleDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle/";
    QDir().mkpath(jungleDir);

    Q_ASSERT(!name.isEmpty());
    m_db = new JsonDatabase();
    m_db->setPath(jungleDir + QStringLiteral("queue_") + name);
    if (!m_db->open()) {
        Q_ASSERT_X(0, "", "Queue could not open database");
    }
    m_coll = m_db->collection("queue");
}

Queue::~Queue()
{
    delete m_db;
}

void Queue::add(const QVariantMap& input)
{
    Q_ASSERT(!input.isEmpty());

    m_coll.insert(input);
    if (m_consumer)
        m_consumer->itemsAdded(this);
}

bool Queue::empty()
{
    return (m_coll.count(QVariantMap()) == 0);
}

void Queue::pop()
{
    QVariantMap map = m_coll.findOne(QVariantMap());
    if (map.isEmpty()) {
        Q_ASSERT_X(0, "", "Queue::pop called when empty!");
    }
    QString id = map.value("_id").toString();
    m_coll.remove(id);
}

void Queue::setConsumer(ConsumerInterface* consumer)
{
    m_consumer = consumer;
}

QVariantMap Queue::top()
{
    QVariantMap map = m_coll.findOne(QVariantMap());
    if (map.isEmpty()) {
        Q_ASSERT_X(0, "", "Queue::top called when empty!");
    }
    map.remove("_id");

    return map;
}

