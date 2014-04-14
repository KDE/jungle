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

#include "themoviedbstore.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>

#include <QNetworkReply>

#include <tmdbqt/searchjob.h>

// Issued to vhanda for personal use
static const char* s_key = "d27948732458af6587dbc9b9764aad37";

using namespace Jungle;

TheMovieDbStore::TheMovieDbStore(QObject* parent)
    : QObject(parent)
    , m_api(QString::fromLatin1(s_key))
{
    connect(&m_api, &TmdbQt::TheMovieDbApi::initialized,
            this, &TheMovieDbStore::initialized);
}

MovieFetchJob* TheMovieDbStore::fetchMovie(const QString& url, const QString& name, int year)
{
    TmdbQt::SearchJob* job = m_api.searchMovie(name, year);
    MovieFetchJob* mjob = new MovieFetchJob(job, url, name, year);

    return mjob;
}

TvShowFetchJob* TheMovieDbStore::fetchTvShow(const QString& name)
{
    TmdbQt::TvSearchJob* job = m_api.searchTvShow(name);
    return new TvShowFetchJob(job, name);
}
