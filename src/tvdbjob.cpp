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

#include <tmdbqt/searchjob.h>
#include <tmdbqt/moviedblist.h>

#include <QDebug>
#include <QUrl>

using namespace Jungle;

TvDbJob::TvDbJob(TmdbQt::TheMovieDbApi& api, const QVariantMap& input, QList<DataQueueInterface*> queues)
    : QObject()
    , m_inputData(input)
    , m_queues(queues)
    , m_api(api)
{
}

void TvDbJob::start()
{
    if (!m_api.isInitialized()) {
        qWarning() << "TvDbApi is not initialized";
        return;
    }

    QString type = m_inputData.value("type").toString();
    if (type == QStringLiteral("movie")) {
        QString name = m_inputData.value("title").toString();
        int year = m_inputData.value("year").toInt();

        TmdbQt::SearchJob* job = m_api.searchMovie(name, year);
        connect(job, SIGNAL(result(TmdbQt::SearchJob*)),
                this, SLOT(slotMovieResult(TmdbQt::SearchJob*)));
        qDebug();
    }
    else if (type == QStringLiteral("episode")) {
        //
        // How do we implement this exactly?
        // We get the following objects -
        // 1. Tv Show
        // 2. Tv Season

        // We cannot actually get much info about the tv show. Or can we?
        // We could get the tv show object + tv season object
        // and just add them

        /*
        QString series = m_inputData.value("series");
        int episodeNumber = m_inputData.value("episodeNumber");
        int season = m_inputData.value("season");
        */

        // TvShow fetch job gives us - tvshow + tv season (partial info)
        // We need an extra tvseason fetch job which gives us the episodes
        // and then we need to collate it with the url

        // TODO
        // 1. Fetch the series info
        // 2. Fetch the season - no that should go in another job?
    }

}

void TvDbJob::slotMovieResult(TmdbQt::SearchJob* job)
{
    TmdbQt::MovieDbList list = job->result();
    if (list.isEmpty()) {
        qWarning() << "Could not find anything for" << m_inputData;
        return;
    }

    TmdbQt::MovieDb movie = list.first();
    int year = m_inputData.value("year").toInt();

    // A simple heuristic in case of multiple results
    for (const TmdbQt::MovieDb& mov : list) {
        if (mov.releaseDate().year() == year) {
            movie = mov;
            break;
        }
    }

    QVariantMap output = m_inputData;
    output["title"] = movie.title();
    output["posterUrl"] = movie.posterUrl(QLatin1String("w342"));

    for (DataQueueInterface* queue : m_queues) {
        queue->add(output);
    }
}

