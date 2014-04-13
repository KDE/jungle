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

#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include "movie.h"

namespace Jungle {

class Database {
public:
    Database(const QString& path);
    ~Database();

    bool init();

    void addMovie(const Movie& movie);
    QList<Movie> allMovies() const;

private:
    QString m_path;
    QSqlDatabase m_sqlDb;
};

}

#endif // DATABASE_H
