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

#include "../job.h"
#include "../asyncjobconsumer.h"
#include "../queueinterface.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>
#include <QTimer>
#include <QQueue>

class AsyncJobConsumerTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSimple();
    void testSkippingItems();
};

using namespace Jungle;

class SimpleJob : public Job
{
public:
    SimpleJob() {
        QTimer::singleShot(0, this, SLOT(emitFinished()));
    }

    QVariantMap data() const {
        return {{"some", "value"}};
    }
};

class Queue : public QueueInterface
{
public:
    virtual void enqueue(const QVariantMap& input) {
        m_queue.enqueue(input);
        if (consumer)
            consumer->itemsAdded(this);
    }

    virtual bool empty() const {
        return m_queue.isEmpty();
    }

    virtual int size() const {
        return m_queue.size();
    }

    virtual void dequeue() {
        m_queue.dequeue();
    }

    virtual QVariantMap head() {
        return m_queue.first();
    }

    QQueue<QVariantMap> m_queue;
    ConsumerInterface* consumer = 0;
};

class SimpleJobConsumer : public AsyncJobConsumer
{
public:
    explicit SimpleJobConsumer(QList< QueueInterface* > output, QObject* parent = 0)
        : AsyncJobConsumer(output, parent)
    {
    }
protected:
    virtual Job* fetchJob(const QVariantMap&) {
        return new SimpleJob();
    }
};

void AsyncJobConsumerTest::testSimple()
{
    // GIVEN
    Queue outQueue;
    QList<QueueInterface*> l = {&outQueue};
    SimpleJobConsumer consumer(l);

    Queue queue;
    queue.consumer = &consumer;

    // WHEN
    QVariantMap d1 = {{"test1", "test"}};
    QVariantMap d2 = {{"test2", "test"}};
    QVariantMap d3 = {{"test3", "test"}};

    queue.enqueue(d1);
    queue.enqueue(d2);
    queue.enqueue(d3);

    QTest::qWait(50);

    // THEN
    QCOMPARE(queue.empty(), true);
    QCOMPARE(outQueue.m_queue.size(), 3);

    QVariantMap add = {{"some", "value"}};
    QCOMPARE(outQueue.m_queue[0], d1.unite(add));
    QCOMPARE(outQueue.m_queue[1], d2.unite(add));
    QCOMPARE(outQueue.m_queue[2], d3.unite(add));
}

class SkippingConsumer : public AsyncJobConsumer
{
public:
    explicit SkippingConsumer(QList< QueueInterface* > output, QObject* parent = 0)
        : AsyncJobConsumer(output, parent)
    {
    }
protected:
    virtual Job* fetchJob(const QVariantMap& input) {
        if (input.contains("url"))
            return new SimpleJob();
        return 0;
    }
};
void AsyncJobConsumerTest::testSkippingItems()
{
    // GIVEN
    Queue outQueue;
    QList<QueueInterface*> l = {&outQueue};
    SkippingConsumer consumer(l);

    Queue queue;
    queue.consumer = &consumer;

    // WHEN
    QVariantMap d1 = {{"url", "test"}};
    QVariantMap d2 = {{"test2", "test"}};
    QVariantMap d3 = {{"url", "test2"}};

    queue.enqueue(d1);
    queue.enqueue(d2);
    queue.enqueue(d3);

    QTest::qWait(50);

    // THEN
    QCOMPARE(queue.empty(), true);
    QCOMPARE(outQueue.m_queue.size(), 2);
    QVariantMap add = {{"some", "value"}};
    QCOMPARE(outQueue.m_queue[0], d1.unite(add));
    QCOMPARE(outQueue.m_queue[1], d3.unite(add));
}


QTEST_MAIN(AsyncJobConsumerTest)

#include "asyncjobconsumertest.moc"
