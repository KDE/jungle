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

#include "../guessitjob.h"

#include <QTest>
#include <QVariantMap>
#include <QSignalSpy>
#include <QDebug>

class GuessItJobTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testEpisode();
    void testDoubleEpisode();
    void testDefaultTypeShouldBeVideo();
    void testNotTvShowWhenNoSeason();
};

using namespace Jungle;

void GuessItJobTest::testEpisode()
{
    const QString url("/home/vishesh/Outlander.S01E05.HDTV.x264-KILLERS.mp4");

    QVariantMap data;
    data["type"] = "tvepisode";
    data["releaseGroup"] = "KILLERS";
    data["mimetype"] = "video/mp4";
    data["series"] = "Outlander";
    data["episodeNumber"] = 5;
    data["videoCodec"] = "h264";
    data["format"] = "HDTV";
    data["season"] = 1;
    data["container"] = "mp4";

    GuessItJob* job = new GuessItJob(url);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QCOMPARE(job->data(), data);
}

void GuessItJobTest::testDoubleEpisode()
{
    const QString url("./How.I.Met.Your.Mother.S09E23-E24.HDTV.x264-EXCELLENCE.mp4");

    QVariantMap data;
    data["type"] = "tvepisode";
    data["releaseGroup"] = "EXCELLENCE";
    data["mimetype"] = "video/mp4";
    data["series"] = "How I Met Your Mother";
    data["episodeNumber"] = 23;
    data["videoCodec"] = "h264";
    data["format"] = "HDTV";
    data["season"] = 9;
    data["container"] = "mp4";

    QVariantList list;
    list << 23 << 24;
    data["episodeList"] = list;

    GuessItJob* job = new GuessItJob(url);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QCOMPARE(job->data(), data);
}

void GuessItJobTest::testDefaultTypeShouldBeVideo()
{
    const QString url("./RandomVideo.mp4");

    QVariantMap data;
    data["type"] = "video";
    data["mimetype"] = "video/mp4";
    data["container"] = "mp4";
    data["title"] = "RandomVideo";

    GuessItJob* job = new GuessItJob(url);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QCOMPARE(job->data(), data);
}

void GuessItJobTest::testNotTvShowWhenNoSeason()
{
    const QString url("/home/vishesh/Images/N9/13090064.mp4");

    GuessItJob* job = new GuessItJob(url);
    QSignalSpy spy(job, SIGNAL(finished(Job*)));
    spy.wait();

    QVariantMap out = job->data();
    QCOMPARE(out.value("type").toString(), QStringLiteral("video"));
}


QTEST_MAIN(GuessItJobTest);

#include "guessitjobtest.moc"
