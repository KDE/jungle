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

#include "moviedbconsumer.h"
#include "queueinterface.h"

using namespace Jungle;

MovieDbConsumer::MovieDbConsumer(QList<QueueInterface*> output, QObject* parent)
    : QObject(parent)
    , m_outputQueues(output)
    , m_job(0)
{
    m_store = new TheMovieDbStore(this);
}

void MovieDbConsumer::itemsAdded(QueueInterface* queue)
{
    if (m_job) {
        queue->pop();
        return;
    }

    m_inputQueue = queue;
    m_input = queue->top();


    const QString type = m_input.value("type").toString();
    if (type != QStringLiteral("movie")) {
        queue->pop();
        return;
    }

    if (!m_input.contains("year")) {
        queue->pop();
        return;
    }

    const QString name = m_input.value("title").toString();
    const QString url = m_input.value("url").toString();
    int year = m_input.value("year").toInt();

    m_job = m_store->fetchMovie(url, name, year);
    connect(m_job, SIGNAL(result(MovieFetchJob*)), this, SLOT(slotResult(MovieFetchJob*)));
}

void MovieDbConsumer::slotResult(MovieFetchJob* job)
{
    QVariantMap map = m_input;
    qDebug() << job->data() << map;

    const QVariantMap data = job->data();
    for (auto it = data.begin(); it != data.end(); it++) {
        map.insert(it.key(), it.value());
    }

    for (QueueInterface* queue : m_outputQueues) {
        queue->add(map);
    }

    m_job = 0;
    m_inputQueue->pop();
    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}


