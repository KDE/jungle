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

#include "guessitjob.h"
#include "interfaces/dataqueueinterface.h"
#include "shared.h"

#include <QTest>
#include <QVariantMap>
#include <QDebug>

class GuessItJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

class MockDataQueue : public DataQueueInterface
{
public:
    MOCK_METHOD1(add, void(const QVariantMap&));
};

void GuessItJobTest::test()
{
    const QString url("/home/vishesh/Outlander.S01E05.HDTV.x264-KILLERS.mp4");

    QVariantMap data;
    data["type"] = "episode";
    data["releaseGroup"] = "KILLERS";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;
    data["videoCodec"] = "h264";
    data["format"] = "HDTV";
    data["season"] = 1;
    data["container"] = "mp4";
    data["filePath"] = url;

    MockDataQueue dataQueue;
    EXPECT_CALL(dataQueue, add(data)).Times(1);

    QList<DataQueueInterface*> queues;
    queues << &dataQueue;

    Jungle::GuessItJob guessItJob(url, queues);
    guessItJob.start();

    QTest::qWait(500);
}

QTEST_GMOCK_MAIN(GuessItJobTest);

#include "guessitjobtest.moc"
