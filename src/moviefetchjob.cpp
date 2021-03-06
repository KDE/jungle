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
#include <QDebug>

#include <tmdbqt/searchjob.h>
#include <tmdbqt/themoviedbapi.h>

using namespace Jungle;

MovieFetchJob::MovieFetchJob(TmdbQt::TheMovieDbApi* api, const QString& searchTerm,
                             int year, QObject* parent)
    : Job(parent)
    , m_api(api)
    , m_searchTerm(searchTerm)
    , m_year(year)
{
    qDebug() << searchTerm << year;
    TmdbQt::SearchJob* job = m_api->searchMovie(searchTerm, year);
    connect(job, SIGNAL(result(TmdbQt::SearchJob*)),
            this, SLOT(slotMovieResult(TmdbQt::SearchJob*)));
}

void MovieFetchJob::slotMovieResult(TmdbQt::SearchJob* job)
{
    if (job->hasError()) {
        qDebug() << job->errorMessage();
    }
    TmdbQt::MovieDbList list = job->result();
    if (list.isEmpty()) {
        emitFinished();
        return;
    }

    TmdbQt::MovieDb movie = list.takeFirst();
    if (m_year == 0 && !list.isEmpty()) {
        emitFinished();
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
                emitFinished();
                return;
            }
        }
    }

    m_data["type"] = QStringLiteral("movie");
    m_data["movieDbId"] = movie.id();
    m_data["title"] = movie.title();
    m_data["releaseDate"] = movie.releaseDate();

    QUrl posterUrl = movie.posterUrl(QLatin1String("w342"));
    m_data["posterPath"] = posterUrl;

    emitFinished();
}

