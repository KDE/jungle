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

#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

using namespace Jungle;

Database::Database(const QString& path)
    : m_path(path)
{
}

Database::~Database()
{
    const QString name = m_sqlDb.connectionName();

    m_sqlDb = QSqlDatabase();
    QSqlDatabase::removeDatabase(name);
}

bool Database::init()
{
    qDebug() << "PATH:" << m_path;
    m_sqlDb = QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_sqlDb.setDatabaseName(m_path + "/webdata.sqlite3");

    if (!m_sqlDb.open()) {
        qDebug() << "Failed to open db" << m_sqlDb.lastError().text();
        return false;
    }

    QSqlQuery query(m_sqlDb);
    query.exec("CREATE TABLE IF NOT EXISTS movies("
               "url TEXT NOT NULL PRIMARY KEY, "
               "mid INTEGER, "
               "title TEXT NOT NULL, "
               "releaseDate TEXT NOT NULL, "
               "posterPath TEXT)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

void Database::addMovie(const Movie& movie)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("insert into movies (url, mid, title, releaseDate, posterPath) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(movie.url());
    query.addBindValue(movie.id());
    query.addBindValue(movie.title());
    query.addBindValue(movie.releaseDate());
    query.addBindValue(movie.posterUrl());

    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

QList<Movie> Database::allMovies() const
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select url, mid, title, releaseDate, posterPath from movies");
    query.exec();

    QList<Movie> movies;
    while (query.next()) {
        Movie movie;
        movie.setUrl(query.value("url").toString());
        movie.setId(query.value("id").toInt());
        movie.setTitle(query.value("title").toString());
        movie.setReleaseDate(query.value("releaseDate").toDate());
        movie.setPosterUrl(query.value("posterPath").toString());

        movies << movie;
    }

    return movies;
}
