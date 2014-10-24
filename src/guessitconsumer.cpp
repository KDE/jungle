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

#include "guessitconsumer.h"
#include "queueinterface.h"

using namespace Jungle;

GuessItConsumer::GuessItConsumer(const QList<QueueInterface*> outputQueues)
    : m_outputQueues(outputQueues)
    , m_inputQueue(0)
    , m_job(0)
{
}

void GuessItConsumer::itemsAdded(QueueInterface* queue)
{
    if (m_job) {
        return;
    }

    m_inputQueue = queue;

    m_input = queue->top();
    QString filePath = m_input.value("url").toString();

    Q_ASSERT(!filePath.isEmpty());

    m_job = new GuessItJob(filePath);
    connect(m_job, SIGNAL(finished(GuessItJob*)), this, SLOT(slotFinished(GuessItJob*)));
}

void GuessItConsumer::slotFinished(GuessItJob* job)
{
    QVariantMap map = m_input;
    map.unite(job->data());

    m_job = 0;

    for (QueueInterface* queue : m_outputQueues) {
        queue->add(map);
    }

    m_inputQueue->pop();
    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}

