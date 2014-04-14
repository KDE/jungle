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

#include "moviefetchjob.h"

#include <QUrl>
#include <QTimer>
#include <QRegularExpression>

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>

#include <QNetworkReply>

#include <tmdbqt/searchjob.h>

using namespace Jungle;

MovieFetchJob::MovieFetchJob(TmdbQt::SearchJob* job, const QString& url,
                             const QString& searchTerm, int year, QObject* parent)
    : QObject(parent)
    , m_url(url)
    , m_searchTerm(searchTerm)
    , m_year(year)
    , m_id(0)
{
    qDebug() << url << searchTerm << year;
    connect(job, SIGNAL(result(TmdbQt::SearchJob*)),
            this, SLOT(slotMovieResult(TmdbQt::SearchJob*)));
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));
}

void MovieFetchJob::slotMovieResult(TmdbQt::SearchJob* job)
{
    TmdbQt::MovieDbList list = job->result();
    if (list.isEmpty()) {
        emit result(this);
        return;
    }

    TmdbQt::MovieDb movie = list.takeFirst();
    if (m_year == 0 && !list.isEmpty()) {
        emit result(this);
        return;
    }

    for (const TmdbQt::MovieDb& mov : list) {
        bool sameYear = (mov.releaseDate().year() == m_year);

        QStringList origWords = m_searchTerm.split(' ', QString::SkipEmptyParts);
        QStringList resultWords = mov.title().split(' ', QString::SkipEmptyParts);

        // simple huerisitic
        if (sameYear && origWords.size() == resultWords.size()) {
            if (movie.id() == -1) {
                movie = mov;
            } else {
                emit result(this);
                return;
            }
        }
    }

    m_id = movie.id();
    m_title = movie.title();
    m_date = movie.releaseDate();

    QUrl posterUrl = movie.posterUrl(QLatin1String("w342"));
    m_network.get(QNetworkRequest(posterUrl));
}

void MovieFetchJob::slotNetworkReply(QNetworkReply* reply)
{
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    m_posterUrl = dataDir + "/jungle/movie-" + QString::number(m_id);

    QFile file(m_posterUrl);
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();

    emit result(this);
}

