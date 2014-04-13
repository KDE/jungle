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

#include "movie.h"

using namespace Jungle;

Movie::Movie()
{
}

int Movie::id() const
{
    return m_id;
}

void Movie::setId(int id)
{
    m_id = id;
}

QString Movie::url() const
{
    return m_url;
}

void Movie::setUrl(const QString& url)
{
    m_url = url;
}

QString Movie::title() const
{
    return m_title;
}

void Movie::setTitle(const QString& title)
{
    m_title = title;
}

void Movie::setReleaseDate(const QDate& date)
{
    m_releaseDate = date;
}

QDate Movie::releaseDate() const
{
    return m_releaseDate;
}

void Movie::setPosterUrl(const QString& url)
{
    m_posterUrl = url;
}

QString Movie::posterUrl() const
{
    return m_posterUrl;
}
