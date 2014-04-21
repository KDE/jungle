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

#include "tvepisode.h"

using namespace Jungle;

TvEpisode::TvEpisode()
    : m_episodeNum(0)
{
}

QDate TvEpisode::airDate() const
{
    return m_airDate;
}

int TvEpisode::episodeNumber() const
{
    return m_episodeNum;
}

QString TvEpisode::name() const
{
    return m_name;
}

QString TvEpisode::overview() const
{
    return m_overview;
}

void TvEpisode::setAirDate(const QDate& date)
{
    m_airDate = date;
}

void TvEpisode::setEpisodeNumber(int num)
{
    m_episodeNum = num;
}

void TvEpisode::setName(const QString& name)
{
    m_name = name;
}

void TvEpisode::setOverview(const QString& overview)
{
    m_overview = overview;
}

void TvEpisode::setStillUrl(const QString& url)
{
    m_stillUrl = url;
}

QString TvEpisode::stillUrl() const
{
    return m_stillUrl;
}

