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

#include "tvdbjob.h"
#include "interfaces/dataqueueinterface.h"
#include "shared.h"

#include <QTest>
#include <QVariantMap>
#include <QDebug>

#include <tmdbqt/themoviedbapi.h>

class TvDbJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testMovie();
    void testInvalidMovie();
    void testMultipleMovieResults();
};

class MockDataQueue : public DataQueueInterface
{
public:
    MOCK_METHOD1(add, void(const QVariantMap&));
};

// Issued to vhanda for personal use
static const char* s_key = "d27948732458af6587dbc9b9764aad37";

void TvDbJobTest::testMovie()
{
    const QString url("/home/vishesh/Shrek.2001.mp4");

    QVariantMap input;
    input["type"] = "movie";
    input["filePath"] = url;
    input["title"] = "Shrek";
    input["year"] = 2001;

    QVariantMap data = input;
    data["posterUrl"] = "http://image.tmdb.org/t/p/w342/140ewbWv8qHStD3mlBDvvGd0Zvu.jpg";

    MockDataQueue dataQueue;
    EXPECT_CALL(dataQueue, add(data)).Times(1);

    QList<DataQueueInterface*> queues;
    queues << &dataQueue;

    TmdbQt::TheMovieDbApi api(QString::fromLatin1(s_key));
    // FIXME: There must be a better way?
    QEventLoop loop;
    connect(&api, SIGNAL(initialized()), &loop, SLOT(quit()));
    loop.exec();

    Jungle::TvDbJob job(api, input, queues);
    job.start();

    QTest::qWait(10000);
}

void TvDbJobTest::testInvalidMovie()
{

}

void TvDbJobTest::testMultipleMovieResults()
{

}


QTEST_GMOCK_MAIN(TvDbJobTest);

#include "tvdbjobtest.moc"
