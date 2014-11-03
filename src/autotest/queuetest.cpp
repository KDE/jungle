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

#include "../queue.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QStandardPaths>
#include <QDebug>

using namespace Jungle;

class QueueTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase() {
        QStandardPaths::setTestModeEnabled(true);
    }
    void testEmpty() {
        Queue q("testEmpty");
        QVERIFY(q.empty());
    }

    void testEnqueue() {
        Queue q("testEnqueue");
        QVariantMap map = {{"url", "a"}};
        q.enqueue(map);

        QCOMPARE(q.empty(), false);
        QCOMPARE(q.head(), map);
    }

    void testDequeue() {
        Queue q("testDequeue");
        QVariantMap map = {{"url", "a"}};
        q.enqueue(map);
        q.dequeue();

        QCOMPARE(q.empty(), true);
    }

    void testHead() {
        Queue q("testHead");
        QVariantMap map1 = {{"url", "a"}};
        QVariantMap map2 = {{"url", "b"}};

        q.enqueue(map1);
        q.enqueue(map2);

        QCOMPARE(q.empty(), false);
        QCOMPARE(q.head(), map1);
    }

    void testRetainState() {
        QVariantMap map1 = {{"url", "a"}};
        QVariantMap map2 = {{"url", "b"}};

        {
            Queue q("testRetainState");
            QCOMPARE(q.empty(), true);
            q.enqueue(map1);
            q.enqueue(map2);
        }
        {
            Queue q("testRetainState");
            QCOMPARE(q.empty(), false);
            QCOMPARE(q.head(), map1);
            q.dequeue();
        }

        {
            Queue q("testRetainState");
            QCOMPARE(q.empty(), false);
            QCOMPARE(q.head(), map2);
            q.dequeue();
            QCOMPARE(q.empty(), true);
        }
    }

    void testHeadStaysTheSame() {
        Queue q("testHeadStaysTheSame");

        QVariantMap map1 = {{"a", "1"}};
        QVariantMap map2 = {{"a", "2"}};
        QVariantMap map3 = {{"a", "3"}};

        q.enqueue(map1);
        q.enqueue(map2);

        qDebug() << q.head();
        QCOMPARE(q.head(), map1);
        q.dequeue();
        QCOMPARE(q.head(), map2);
        q.enqueue(map3);
        QCOMPARE(q.head(), map2);
    }
};

QTEST_MAIN(QueueTest);

#include "queuetest.moc"
