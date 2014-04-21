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

Database::Database(const QString& path, const QString& fileMapDb)
    : m_path(path)
    , m_fileMapDb(fileMapDb)
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

    //
    // Movies table
    //
    QSqlQuery query(m_sqlDb);
    query.exec("CREATE TABLE IF NOT EXISTS movies("
               "fid INTEGER NOT NULL PRIMARY KEY, "
               "mid INTEGER, "
               "title TEXT NOT NULL, "
               "releaseDate TEXT NOT NULL, "
               "posterPath TEXT)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    // Videos table
    // This is used to check if a video has been processed
    query.exec("CREATE TABLE IF NOT EXISTS videos("
               "fid INTEGER NOT NULL PRIMARY KEY)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    //
    // Tv shows
    //
    query.exec("CREATE TABLE IF NOT EXISTS shows("
               "id INTEGER NOT NULL PRIMARY KEY, "
               "title TEXT NOT NULL, "
               "releaseDate TEXT NOT NULL, "
               "numSeasons INTEGER NOT NULL, "
               "posterPath TEXT)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    //
    // Attach the file mapping db
    //
    query.prepare("ATTACH DATABASE ? AS fileMap");
    query.addBindValue(m_fileMapDb);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

void Database::addMovie(const Movie& movie)
{
    int id = fileId(movie.url());
    if (id == 0) {
        qDebug() << "Could not find an id for" << movie.url();
        return;
    }

    QSqlQuery query(m_sqlDb);
    query.prepare("insert into movies (fid, mid, title, releaseDate, posterPath) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(id);
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
    query.exec("select files.url, mid, title, releaseDate, posterPath from files, movies where files.id = movies.fid");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return QList<Movie>();
    }

    QList<Movie> movies;
    while (query.next()) {
        Movie movie;
        movie.setUrl(query.value("url").toString());
        movie.setId(query.value("mid").toInt());
        movie.setTitle(query.value("title").toString());
        movie.setReleaseDate(query.value("releaseDate").toDate());
        movie.setPosterUrl(query.value("posterPath").toString());

        movies << movie;
    }

    return movies;
}

bool Database::hasVideo(int fileId)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select 1 from videos where fid = ?");
    query.addBindValue(fileId);

    if (!query.exec()) {
        qDebug() << "hasVideo:" << query.lastError();
        return false;
    }

    return query.next();
}


void Database::addVideo(const QString& url)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("insert into videos VALUES (?)");
    query.addBindValue(fileId(url));
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

int Database::fileId(const QString& url)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select id from files where url = ? LIMIT 1");
    query.addBindValue(url);
    query.exec();

    int id = 0;
    while (query.next()) {
        id = query.value(0).toInt();
    }

    return id;
}

QString Database::fileUrl(int fid)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select url from files where id = ? LIMIT 1");
    query.addBindValue(fid);
    query.exec();

    QString url;
    while (query.next()) {
        url = query.value(0).toString();
    }

    return url;
}

bool Database::hasShow(const QString& name)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select 1 from shows where title = ?");
    query.addBindValue(name);
    query.exec();

    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    return query.next();
}

void Database::addShow(const Show& show)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("insert into shows (id, title, releaseDate, numSeasons, posterPath) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(show.id());
    query.addBindValue(show.title());
    query.addBindValue(show.firstAired());
    query.addBindValue(show.seasons().size());
    query.addBindValue(show.coverUrl());

    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

QList<Show> Database::allShows() const
{
    QSqlQuery query(m_sqlDb);
    query.exec("select id, title, releaseDate, posterPath from shows");
    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return QList<Show>();
    }

    QList<Show> shows;
    while (query.next()) {
        Show show;
        show.setId(query.value("id").toInt());
        show.setTitle(query.value("title").toString());
        show.setFirstAired(query.value("releaseDate").toDate());
        show.setCoverUrl(query.value("posterPath").toString());

        shows << show;
    }

    return shows;
}


