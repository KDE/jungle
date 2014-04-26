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
               "posterPath TEXT)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    //
    // Tv Seasons
    //
    query.exec("CREATE TABLE IF NOT EXISTS tvseasons("
               "id INTEGER NOT NULL, "
               "show INTEGER NOT NULL, "
               "seasonNum INTEGER NOT NULL, "
               "airDate TEXT NOT NULL, "
               "posterPath TEXT NOT NULL,"
               "PRIMARY KEY(show, seasonNum))");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    //
    // Tv episodes
    //
    query.exec("CREATE TABLE IF NOT EXISTS tvepisodes("
               "episodeNum INTEGER NOT NULL, "
               "season INTEGER NOT NULL, "
               "show INTEGER NOT NULL, "
               "fid INTEGER, "
               "airDate TEXT NOT NULL, "
               "name TEXT NOT NULL, "
               "overview TEXT NOT NULL, "
               "stillPath TEXT NOT NULL, "
               "PRIMARY KEY(episodeNum, season, show))");

    if (query.lastError().isValid()) {
        qDebug() << "TV EP" << query.lastError();
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
    query.exec("select files.url, mid, title, releaseDate, posterPath from files, "
               "movies where files.id = movies.fid ORDER BY title");
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

int Database::showId(const QString& name)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select id from shows where lower(title) = lower(?)");
    query.addBindValue(name);
    query.exec();

    if (!query.exec()) {
        qDebug() << query.lastError();
        return 0;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

void Database::addShow(const Show& show)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("insert into shows (id, title, releaseDate, posterPath) "
                  "VALUES (?, ?, ?, ?)");
    query.addBindValue(show.id());
    query.addBindValue(show.title());
    query.addBindValue(show.firstAired());
    query.addBindValue(show.coverUrl());

    if (!query.exec()) {
        qDebug() << "SHOW" << query.lastError();
    }

    foreach (const TvSeason& s, show.seasons()) {
        QSqlQuery query(m_sqlDb);
        query.prepare("insert into tvseasons (id, show, seasonNum, airDate, posterPath) "
                      "VALUES (?, ?, ?, ?, ?)");
        query.addBindValue(s.id());
        query.addBindValue(show.id());
        query.addBindValue(s.seasonNumber());
        query.addBindValue(s.airDate());
        query.addBindValue(s.posterUrl());

        if (!query.exec()) {
            qDebug() << s.id() << show.id();
            qDebug() << "SEASON" << query.lastError();
        }
    }
}

void Database::addEpisode(int showId, int seasonId, const TvEpisode& episode)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("insert or replace into tvepisodes "
                  "(episodeNum, season, show, fid, airDate, name, overview, stillPath) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(episode.episodeNumber());
    query.addBindValue(seasonId);
    query.addBindValue(showId);
    query.addBindValue(fileId(episode.url()));
    query.addBindValue(episode.airDate());
    query.addBindValue(episode.name());
    query.addBindValue(episode.overview());
    query.addBindValue(episode.stillUrl());

    qDebug() << episode.url() << fileId(episode.url());
    if (!query.exec()) {
        qDebug() << "EP" << query.lastError();
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

TvEpisode Database::episode(int showId, int season, int epNum)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select * from tvepisodes "
                  "where show = ? AND season = ? AND episodeNum = ?");
    query.addBindValue(showId);
    query.addBindValue(season);
    query.addBindValue(epNum);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return TvEpisode();
    }

    TvEpisode ep;
    if (query.next()) {
        ep.setAirDate(query.value("airDate").toDate());
        ep.setName(query.value("name").toString());
        ep.setOverview(query.value("overview").toString());
        ep.setStillUrl(query.value("stillPath").toString());
        ep.setEpisodeNumber(query.value("episodeNum").toInt());

        int fid = query.value("fid").toInt();
        ep.setUrl(fileUrl(fid));
    }

    return ep;
}

QList<TvEpisode> Database::allEpisodes(int showId)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select * from tvepisodes, files "
                  "where show = ? AND fid = files.id ORDER BY episodeNum");
    query.addBindValue(showId);

    if (!query.exec()) {
        qDebug() << "EE" << query.lastError();
        return QList<TvEpisode>();
    }

    QList<TvEpisode> epList;
    while (query.next()) {
        TvEpisode ep;
        ep.setAirDate(query.value("airDate").toDate());
        ep.setName(query.value("name").toString());
        ep.setOverview(query.value("overview").toString());
        ep.setStillUrl(query.value("stillPath").toString());
        ep.setEpisodeNumber(query.value("episodeNum").toInt());
        ep.setUrl(query.value("url").toString());
        ep.setSeason(query.value("season").toInt());

        epList << ep;
    }

    return epList;
}


bool Database::hasEpisodes(int show, int season)
{
    QSqlQuery query(m_sqlDb);
    query.prepare("select 1 from tvepisodes where show = ? AND season = ?");
    query.addBindValue(show);
    query.addBindValue(season);

    if (!query.exec()) {
        return false;
    }

    return query.next();
}
