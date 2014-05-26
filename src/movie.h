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

#ifndef MOVIE_H
#define MOVIE_H

#include "jungle_export.h"
#include <QString>
#include <QDate>

namespace Jungle {

class JUNGLE_EXPORT Movie
{
public:
    Movie();

    /**
     * Return a unique identifier for this movie
     */
    int id() const;
    void setId(int id);

    /**
     * Return the local url of where this movie is stored on the
     * hard drive
     */
    QString url() const;
    void setUrl(const QString& url);

    /**
     * The title of the movie
     */
    QString title() const;
    void setTitle(const QString& title);

    QDate releaseDate() const;
    void setReleaseDate(const QDate& date);

    QString posterUrl() const;
    void setPosterUrl(const QString& url);

    bool operator ==(const Movie& rhs) const;

private:
    int m_id;
    QString m_url;
    QString m_title;
    QDate m_releaseDate;
    QString m_posterUrl;
};

}

Q_DECLARE_METATYPE(Jungle::Movie)

#endif // MOVIE_H
