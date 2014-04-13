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

#include "show.h"

using namespace Jungle;

Show::Show()
    : m_id(0)
    , m_numSeasons(0)
{
}

QString Show::coverUrl() const
{
    return m_coverUrl;
}

QDate Show::firstAired() const
{
    return m_firstAired;
}

int Show::id() const
{
    return m_id;
}

int Show::numSeasons() const
{
    return m_numSeasons;
}

void Show::setCoverUrl(const QString& url)
{
    m_coverUrl = url;
}

void Show::setFirstAired(const QDate& date)
{
    m_firstAired = date;
}

void Show::setId(int id)
{
    m_id = id;
}

void Show::setNumSeasons(int num)
{
    m_numSeasons = num;
}

void Show::setTitle(const QString& name)
{
    m_title = name;
}

QString Show::title() const
{
    return m_title;
}

