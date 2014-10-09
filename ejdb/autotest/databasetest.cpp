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

#include "jsondatabase.h"
#include "jsoncollection.h"

#include <QTest>
#include <QVariantMap>
#include <QDebug>
#include <QTemporaryDir>

class DatabaseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

void DatabaseTest::test()
{
    QVariantMap data;
    data["type"] = "episode";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;

    QTemporaryDir dir;
    JsonDatabase db;
    db.setPath(dir.path() + "/db");
    QVERIFY(db.open());

    JsonCollection col = db.collection("testCol");
    QByteArray id = col.add(data);
    QVariantMap output = col.fetch(id);

    data["_id"] = QString::fromUtf8(id);
    QCOMPARE(output, data);
}

QTEST_MAIN(DatabaseTest);

#include "databasetest.moc"
