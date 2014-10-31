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

#include "../moviefetchjob.h"
#include "../themoviedbstore.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>

class MovieFetchJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void test();
};

using namespace Jungle;

void MovieFetchJobTest::test()
{
    TheMovieDbStore store;
    QSignalSpy s(&store, SIGNAL(initialized()));
    s.wait();

    MovieFetchJob* job = store.fetchMovie("Before Sunset", 2004);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QVariantMap data;
    data["movieDbId"] = 80;
    data["title"] = "Before Sunset";
    data["releaseDate"] = QDate(2004, 02, 10);
    data["posterPath"] = QUrl("http://image.tmdb.org/t/p/w342/cIj6yWJKUjdCCO7vuZQKl0NqCQe.jpg");

    QCOMPARE(job->data(), data);
    QVERIFY(job->extraData().isEmpty());
}

QTEST_MAIN(MovieFetchJobTest);

#include "moviefetchjobtest.moc"
