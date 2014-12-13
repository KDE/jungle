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

#ifndef JUNGLE_ASYNCCONSUMER_H
#define JUNGLE_ASYNCCONSUMER_H

#include "consumerinterface.h"
#include "queueinterface.h"

namespace Jungle {

class Job;

class AsyncJobConsumer : public QObject, public ConsumerInterface
{
    Q_OBJECT
public:
    explicit AsyncJobConsumer(QueueInterface* input, QList<QueueInterface*> output, QObject* parent = 0);

    virtual void itemsAdded(QueueInterface* queue);

signals:
    void readyChanged();

protected:
    /**
     * Create the job out of the input. Return 0 if the input
     * should be ignored
     */
    virtual Job* fetchJob(const QVariantMap& input) = 0;

    void setReady(bool status);
    bool ready() const;

private slots:
    void slotFinished(Job* job);
    void slotInvalidData(Job* job);

private:
    Job* m_job;

    QList<QueueInterface*> m_outputQueues;
    QueueInterface* m_inputQueue;

    QVariantMap m_input;
    bool m_ready;
};

}

#endif // JUNGLE_ASYNCCONSUMER_H
