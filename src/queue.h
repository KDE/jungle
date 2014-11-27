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

#ifndef JUNGLE_QUEUE_H
#define JUNGLE_QUEUE_H

#include "queueinterface.h"
#include "consumerinterface.h"
#include "kvariantstore.h"
#include "kvariantcollection.h"

#include <QQueue>

namespace Jungle {

class Queue : public QObject, public QueueInterface
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
public:
    Queue(const QString& name);
    virtual ~Queue();

    void setConsumer(ConsumerInterface* consumer);
    ConsumerInterface* consumer();

    virtual void enqueue(const QVariantMap& input);
    virtual bool empty() const;
    virtual void dequeue();
    virtual QVariantMap head();
    virtual int size() const;

signals:
    void sizeChanged();

private:
    ConsumerInterface* m_consumer;

    KVariantStore* m_db;
    KVariantCollection m_coll;
};
}

#endif // JUNGLE_QUEUE_H
