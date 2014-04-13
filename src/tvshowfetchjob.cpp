/*
 * <one line to give the library's name and an idea of what it does.>
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

#include "tvshowfetchjob.h"

#include <QStandardPaths>
#include <QNetworkRequest>
#include <QFile>
#include <QTimer>

using namespace Jungle;

TvShowFetchJob::TvShowFetchJob(const QString& name, QObject* parent)
    : QObject(parent)
    , m_name(name)
{
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));

    m_client = new Tvdb::Client(this);
    connect(m_client, SIGNAL(finished(Tvdb::Series)),
            this, SLOT(slotFinished(Tvdb::Series)));

    qDebug() << "Calling" << name;
    //QTimer::singleShot(0, this, SLOT(slotBah()));
    //return;

    m_client->getSeriesByName(name);
}

void TvShowFetchJob::slotBah()
{
    emit result(this);
}


TvShowFetchJob::~TvShowFetchJob()
{
    delete m_client;
}

void TvShowFetchJob::slotFinished(const Tvdb::Series& series)
{
    m_show.setTitle(series.name());
    m_show.setFirstAired(series.firstAired());
    m_show.setNumSeasons(series.numSeasons());
    m_show.setId(series.id());

    QList<QUrl> urls = series.posterUrls();
    if (urls.isEmpty()) {
        emit result(this);
        return;
    }

    qDebug() << "NET" << urls.first();
    QNetworkReply* reply = m_network.get(QNetworkRequest(urls.first()));
    connect(reply, SIGNAL(finished()), this, SLOT(slotNetworkReply()));

    delete m_client;
    qDebug() << reply << reply->isRunning();
}

void TvShowFetchJob::slotMultipleResultsFound(const QList<Tvdb::Series>& series)
{
    // FIXME: Better multiple result handling?
    qDebug() << "MULIT" << series.size();
    slotFinished(series.first());
}

void TvShowFetchJob::slotNetworkReply(QNetworkReply* reply)
{
    qDebug() << "NETsssssssssssss";
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QString url = dataDir + "/jungle/" + QString::number(m_show.id());

    QFile file(url);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();

    m_show.setCoverUrl(url);

    emit result(this);
}

void TvShowFetchJob::slotNetworkReply()
{
    qDebug() << "EEEE";
    slotNetworkReply(qobject_cast<QNetworkReply*>(sender()));
}



