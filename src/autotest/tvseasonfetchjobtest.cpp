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

#include "../tvseasonfetchjob.h"
#include "../themoviedbstore.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>

class TvSeasonFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

using namespace Jungle;

void TvSeasonFetchJobTest::test()
{
    TheMovieDbStore store;
    QSignalSpy s(&store, SIGNAL(initialized()));
    s.wait();

    const QString dbId("db-Id-string");
    TvSeasonFetchJob* job = store.fetchTvSeason(4607, 1, dbId);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QVariantMap data = job->data();
    QCOMPARE(data.value("type").toString(), QString("tvseason"));
    QCOMPARE(data.value("airDate").toDate(), QDate(2004, 9, 22));
    QCOMPARE(data.value("movieDbId").toInt(), 14041);
    QCOMPARE(data.value("seasonNumber").toInt(), 1);
    QCOMPARE(data.value("showId").toString(), dbId);
    QVERIFY(data.contains("overview"));

    QList<QVariantMap> extraData = job->extraData();
    for (const QVariantMap& episode : extraData) {
        QCOMPARE(episode.value("type").toString(), QStringLiteral("tvepisode"));
        QCOMPARE(episode.value("showId").toString(), dbId);
        QCOMPARE(episode.value("movieDbShowId").toInt(), 4607);
        QCOMPARE(episode.value("season").toInt(), 1);
        QVERIFY(!episode.value("stillPath").toString().isEmpty());
        QVERIFY(episode.contains("stillPath"));
        QVERIFY(episode.contains("episodeNumber"));
        QVERIFY(episode.contains("overview"));
    }
}

QTEST_MAIN(TvSeasonFetchJobTest);

#include "tvseasonfetchjobtest.moc"
