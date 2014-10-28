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

#include "../networkimagefetchjob.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>
#include <QStandardPaths>

class NetworkImageFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

using namespace Jungle;

void NetworkImageFetchJobTest::test()
{
    QStandardPaths::setTestModeEnabled(true);

    const QString url("https://www.google.es/images/srpr/logo11w.png");

    QVariantMap data;
    data["type"] = "tvepisode";
    data["thumbnail"] = url;

    auto job = new NetworkImageFetchJob(data);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QVariantMap output = job->data();
    QString newUrl = output.value("thumbnail").toString();
    QVERIFY(!newUrl.isEmpty());
    QVERIFY(newUrl.startsWith("/"));
    QVERIFY(newUrl.contains("tvepisode"));
}

QTEST_MAIN(NetworkImageFetchJobTest);

#include "networkimagefetchjobtest.moc"
