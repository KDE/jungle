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
#include <QDateTime>

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

void Queue::enqueue(const QVariantMap& input)
{
    Q_ASSERT(!input.isEmpty());

    QVariantMap map(input);
    // Just using the mtime is not enough as this function is often called
    // with the same milliseconds. The `count` solution is hack, but it
    // mostly works with little drawbacks.
    static int count = 0;
    map["_mtime"] = QDateTime::currentDateTime().toMSecsSinceEpoch() + (count++);
    m_coll.insert(map);
    if (m_consumer)
        m_consumer->itemsAdded(this);
}

bool Queue::empty()
{
    return (m_coll.count(QVariantMap()) == 0);
}

void Queue::dequeue()
{
    QVariantMap order = {{"_mtime", 1}};
    QVariantMap hint = {{"$orderby", order}};
    QVariantMap map = m_coll.findOne(QVariantMap(), hint);
    if (map.isEmpty()) {
        Q_ASSERT_X(0, "", "Queue::dequeue called when empty!");
    }
    QString id = map.value("_id").toString();
    m_coll.remove(id);
}

void Queue::setConsumer(ConsumerInterface* consumer)
{
    m_consumer = consumer;
}

ConsumerInterface* Queue::consumer()
{
    return m_consumer;
}

QVariantMap Queue::head()
{
    QVariantMap order = {{"_mtime", 1}};
    QVariantMap hint = {{"$orderby", order}};
    QVariantMap map = m_coll.findOne(QVariantMap(), hint);
    if (map.isEmpty()) {
        Q_ASSERT_X(0, "", "Queue::head called when empty!");
    }
    map.remove("_id");
    map.remove("_mtime");

    return map;
}

