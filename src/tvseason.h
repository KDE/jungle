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

#ifndef JUNGLE_TVSEASON_H
#define JUNGLE_TVSEASON_H

#include "jungle_export.h"
#include <QString>
#include <QDate>

namespace Jungle {

class JUNGLE_EXPORT TvSeason
{
public:
    TvSeason();

    int id() const;
    void setId(int id);

    int seasonNumber() const;
    void setSeasonNumber(int num);

    QDate airDate() const;
    void setAirDate(const QDate& date);

    QString posterUrl() const;
    void setPosterUrl(const QString& url);

    QString overview();
    void setOverview(const QString& overview);

private:
    int m_id;
    int m_seasonNum;
    QDate m_airDate;
    QString m_posterUrl;
    QString m_overview;
};
}

#endif // JUNGLE_TVSEASON_H
