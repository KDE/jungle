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

#include "../jsondatabase.h"
#include "../jsoncollection.h"
#include "../jsonquery.h"

#include <QTest>
#include <QVariantMap>
#include <QDebug>
#include <QTemporaryDir>

class DatabaseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testInsertAndFetch();
    void testInsertAndQuery();
};

void DatabaseTest::testInsertAndFetch()
{
    QTemporaryDir dir;
    JsonDatabase db;
    db.setPath(dir.path() + "/db");
    QVERIFY(db.open());

    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;

    JsonCollection col = db.collection("testCol");
    QByteArray id = col.insert(data);
    QVariantMap output = col.fetch(id);

    data["_id"] = QString::fromUtf8(id);
    QCOMPARE(output, data);
}

void DatabaseTest::testInsertAndQuery()
{
    QTemporaryDir dir;
    JsonDatabase db;
    db.setPath(dir.path() + "/db");
    QVERIFY(db.open());

    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;

    JsonCollection col = db.collection("testCol");
    QByteArray id1 = col.insert(data);

    data["episodeNumber"] = 6;
    QByteArray id2 = col.insert(data);

    QVariantMap queryMap = {{"type", "episode"}};
    JsonQuery query = col.execute(queryMap);

    QCOMPARE(query.totalCount(), 2);
    QVERIFY(query.next());
    data["_id"] = QString::fromUtf8(id1);
    data["episodeNumber"] = 5;
    QCOMPARE(query.result(), data);
    QVERIFY(query.next());
    data["_id"] = QString::fromUtf8(id2);
    data["episodeNumber"] = 6;
    QCOMPARE(query.result(), data);
    QVERIFY(!query.next());
}

QTEST_MAIN(DatabaseTest);

#include "databasetest.moc"
