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
#include <tmdbqt/tvdblist.h>

#include <QStandardPaths>
#include <QNetworkRequest>
#include <QFile>
#include <QTimer>

using namespace Jungle;

TvShowFetchJob::TvShowFetchJob(TmdbQt::TvSearchJob* job, const QString& name, QObject* parent)
    : QObject(parent)
    , m_name(name)
{
    connect(job, SIGNAL(result(TmdbQt::TvSearchJob*)),
            this, SLOT(slotResult(TmdbQt::TvSearchJob*)));
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));

    qDebug() << "TvShow:" << name;
}

TvShowFetchJob::~TvShowFetchJob()
{
}

void TvShowFetchJob::slotResult(TmdbQt::TvSearchJob* job)
{
    TmdbQt::TvDbList shows = job->result();
    if (shows.isEmpty()) {
        emit result(this);
        return;
    }

    TmdbQt::TvDb tvshow = shows.first();
    m_show.setTitle(tvshow.name());
    m_show.setFirstAired(tvshow.firstAiredDate());
    m_show.setId(tvshow.id());

    QUrl posterUrl = tvshow.posterUrl(QLatin1String("w342"));
    m_network.get(QNetworkRequest(posterUrl));
}

void TvShowFetchJob::slotNetworkReply(QNetworkReply* reply)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QString url = dataDir + "/jungle/tvshow-" + QString::number(m_show.id());

    QFile file(url);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();

    m_show.setCoverUrl(url);

    emit result(this);
}
