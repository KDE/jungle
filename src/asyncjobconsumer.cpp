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

#include "asyncjobconsumer.h"
#include "job.h"

#include <QDebug>

using namespace Jungle;

AsyncJobConsumer::AsyncJobConsumer(QueueInterface* input, QList< QueueInterface* > output, QObject* parent)
    : QObject(parent)
    , m_job(0)
    , m_outputQueues(output)
    , m_inputQueue(input)
    , m_ready(true)
{
}

void AsyncJobConsumer::itemsAdded(QueueInterface* queue)
{
    Q_ASSERT(m_inputQueue == queue);
    if (m_job) {
        return;
    }

    if (!m_ready) {
        return;
    }

    m_input = queue->head();
    m_job = fetchJob(m_input);
    if (m_job == 0) {
        m_inputQueue->dequeue();
        if (!m_inputQueue->empty()) {
            itemsAdded(m_inputQueue);
        }
        return;
    }

    connect(m_job, SIGNAL(finished(Job*)), this, SLOT(slotFinished(Job*)));
    connect(m_job, SIGNAL(invalidData(Job*)), this, SLOT(slotInvalidData(Job*)));
}

void AsyncJobConsumer::slotFinished(Job* job)
{
    const QVariantMap data = job->data();
    if (data.isEmpty()) {
        m_inputQueue->dequeue();
        m_job = 0;

        if (!m_inputQueue->empty()) {
            itemsAdded(m_inputQueue);
        }
        return;
    }

    // Merge input and output
    for (auto it = data.begin(); it != data.end(); it++)
        m_input.insert(it.key(), it.value());

    for (QueueInterface* queue : m_outputQueues) {
        queue->enqueue(m_input);
    }

    auto list = job->extraData();
    for (const QVariantMap& map: list) {
        for (QueueInterface* queue : m_outputQueues) {
            queue->enqueue(map);
        }
    }

    m_inputQueue->dequeue();
    m_job = 0;

    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}

void AsyncJobConsumer::slotInvalidData(Job* job)
{
    m_input.insert("delete", true);

    for (QueueInterface* queue : m_outputQueues) {
        queue->enqueue(m_input);
    }

    m_inputQueue->dequeue();
    m_job = 0;

    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}

bool AsyncJobConsumer::ready() const
{
    return m_ready;
}

void AsyncJobConsumer::setReady(bool status)
{
    m_ready = status;
    emit readyChanged();

    if (m_ready && m_inputQueue && !m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}

