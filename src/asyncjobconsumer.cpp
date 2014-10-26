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

#include "asyncjobconsumer.h"
#include "job.h"

using namespace Jungle;

AsyncJobConsumer::AsyncJobConsumer(QList< QueueInterface* > output, QObject* parent)
    : QObject(parent)
    , m_job(0)
    , m_outputQueues(output)
{
}

void AsyncJobConsumer::itemsAdded(QueueInterface* queue)
{
    if (m_job) {
        return;
    }

    m_inputQueue = queue;

    m_input = queue->top();
    m_job = fetchJob(m_input);
    if (m_job == 0) {
        m_inputQueue->pop();
        if (!m_inputQueue->empty()) {
            itemsAdded(m_inputQueue);
        }
        return;
    }

    connect(m_job, SIGNAL(finished(Job*)), this, SLOT(slotFinished(Job*)));
}

void AsyncJobConsumer::slotFinished(Job* job)
{
    const QVariantMap data = job->data();
    for (auto it = data.begin(); it != data.end(); it++)
        m_input.insert(it.key(), it.value());

    for (QueueInterface* queue : m_outputQueues) {
        queue->add(m_input);
    }

    m_inputQueue->pop();
    m_job = 0;

    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}
