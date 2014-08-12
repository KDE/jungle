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

#include "tvseason.h"

using namespace Jungle;

TvSeason::TvSeason()
    : m_id(0)
    , m_seasonNum(0)
{
}

QDate TvSeason::airDate() const
{
    return m_airDate;
}

QList< TvEpisode > TvSeason::episodes() const
{
    return m_episodes;
}

QString TvSeason::overview()
{
    return m_overview;
}

QString TvSeason::posterUrl() const
{
    return m_posterUrl;
}

int TvSeason::seasonNumber() const
{
    return m_seasonNum;
}

int TvSeason::id() const
{
    return m_id;
}

void TvSeason::setAirDate(const QDate& date)
{
    m_airDate = date;
}

void TvSeason::setEpisodes(const QList< TvEpisode >& episodes)
{
    m_episodes = episodes;
}

void TvSeason::setId(int id)
{
    m_id = id;
}

void TvSeason::setOverview(const QString& overview)
{
    m_overview = overview;
}

void TvSeason::setPosterUrl(const QString& url)
{
    m_posterUrl = url;
}

void TvSeason::setSeasonNumber(int num)
{
    m_seasonNum = num;
}

