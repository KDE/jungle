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
#include <QDebug>

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

TheMovieDbStore::~TheMovieDbStore()
{
}

MovieFetchJob* TheMovieDbStore::fetchMovie(const QString& name, int year)
{
    return new MovieFetchJob(&m_api, name, year);
}

TvShowFetchJob* TheMovieDbStore::fetchTvShow(const QString& name, const QString& dbShowId)
{
    return new TvShowFetchJob(&m_api, name, dbShowId, this);
}

TvSeasonFetchJob* TheMovieDbStore::fetchTvSeason(int showId, int seasonNum, const QString& dbShowId)
{
    return new TvSeasonFetchJob(&m_api, showId, seasonNum, dbShowId, this);
}

bool TheMovieDbStore::isInitialized() const
{
    return m_api.isInitialized();
}
