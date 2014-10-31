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

#include "../tvshowfetchjob.h"
#include "../themoviedbstore.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>

class TvShowFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

using namespace Jungle;

void TvShowFetchJobTest::test()
{
    TheMovieDbStore store;
    QSignalSpy s(&store, SIGNAL(initialized()));
    s.wait();

    const QString dbId("db-Id-string");
    TvShowFetchJob* job = store.fetchTvShow("Lost", dbId);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    QVERIFY(spy.wait());

    QVariantMap data;
    data["type"] = "tvshow";
    data["movieDbId"] = 4607;
    data["title"] = "Lost";
    data["releaseDate"] = QDate(2004, 9, 22);
    data["posterPath"] = QUrl("http://image.tmdb.org/t/p/w342/lMt67Q7H6KTDGQJ0P0dZSRld0OV.jpg");

    QCOMPARE(job->data(), data);

    QList<QVariantMap> extraData = job->extraData();
    QCOMPARE(extraData.size(), 7);
    for (const QVariantMap& season : extraData) {
        QCOMPARE(season.value("type").toString(), QStringLiteral("tvseason"));
        QCOMPARE(season.value("showId").toString(), dbId);
        QCOMPARE(season.value("movieDbShowId").toInt(), 4607);
        QVERIFY(!season.value("posterPath").toString().isEmpty());
        QVERIFY(season.contains("seasonNumber"));
        QVERIFY(season.contains("airDate"));
    }
}

QTEST_MAIN(TvShowFetchJobTest);

#include "tvshowfetchjobtest.moc"
