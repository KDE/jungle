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

#include "tvshow.h"

using namespace Jungle;

TvShow::TvShow()
    : m_id(0)
{
}

QString TvShow::coverUrl() const
{
    return m_coverUrl;
}

QDate TvShow::firstAired() const
{
    return m_firstAired;
}

int TvShow::id() const
{
    return m_id;
}

void TvShow::setCoverUrl(const QString& url)
{
    m_coverUrl = url;
}

void TvShow::setFirstAired(const QDate& date)
{
    m_firstAired = date;
}

void TvShow::setId(int id)
{
    m_id = id;
}

void TvShow::setTitle(const QString& name)
{
    m_title = name;
}

QString TvShow::title() const
{
    return m_title;
}
