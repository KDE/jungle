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

#ifndef JUNGLE_TVEPISODE_H
#define JUNGLE_TVEPISODE_H

#include "jungle_export.h"
#include <QDate>
#include <QString>

namespace Jungle {

class JUNGLE_EXPORT TvEpisode
{
public:
    TvEpisode();

    int episodeNumber() const;
    void setEpisodeNumber(int num);

    QDate airDate() const;
    void setAirDate(const QDate& date);

    QString name() const;
    void setName(const QString& name);

    QString overview() const;
    void setOverview(const QString& overview);

    QString stillUrl() const;
    void setStillUrl(const QString& url);

    QString url() const;
    void setUrl(const QString& url);

    int season() const;
    void setSeason(int s);

    int show() const;
    void setShow(int show);

private:
    int m_episodeNum;
    QDate m_airDate;
    QString m_name;
    QString m_overview;
    QString m_stillUrl;
    QString m_url;
    int m_season;
    int m_show;
};
}

#endif // JUNGLE_TVEPISODE_H
