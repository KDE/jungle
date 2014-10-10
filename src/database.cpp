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
#include "jsonquery.h"

#include <QVariantMap>
#include <QDebug>

#include <QStandardPaths>
#include <QDir>

using namespace Jungle;

Database::Database(const QString& path)
    : m_path(path)
    , m_initialized(false)
{
}

Database::~Database()
{
}

static QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle/db";
Q_GLOBAL_STATIC_WITH_ARGS(Database, s_database, (dataDir));

// static
Database* Database::instance()
{
    if (!s_database->initialized()) {
        QString jungleDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle";
        QDir().mkpath(jungleDir);

        s_database->init();
    }
    return &(*s_database);
}

bool Database::initialized()
{
    return m_initialized;
}

bool Database::init()
{
    m_jdb.setPath(m_path);
    if (!m_jdb.open()) {
        return false;
    }

    m_coll = m_jdb.collection("videos");
    m_initialized = true;
    return true;
}

void Database::addMovie(const QVariantMap& movie)
{
    QVariantMap map = movie;
    map["type"] = "movie";

    m_coll.insert(map);
    emit movieAdded(map);
}

QList<QVariantMap> Database::allMovies() const
{
    QVariantMap queryMap = {{"type", "movie"}};
    JsonQuery query = m_coll.execute(queryMap);

    QList<QVariantMap> movies;
    while (query.next()) {
        QVariantMap map = query.result();
        movies << map;
    }

    return movies;
}

bool Database::hasVideo(const QString& url)
{
    QVariantMap queryMap = {{"url", url}};
    JsonQuery query = m_coll.execute(queryMap);

    return query.totalCount();
}

int Database::showId(const QString& name)
{
    // FIXME: This needs to be done in lowercase!
    QVariantMap queryMap;
    queryMap["type"] = "tvshow";
    queryMap["title"] = name;

    JsonQuery query = m_coll.execute(queryMap);
    if (query.next()) {
        QVariantMap map = query.result();
        return map["id"].toInt();
    }

    return 0;
}

void Database::addShow(const TvShow& show)
{
    QVariantMap map;
    map["type"] = "tvshow";
    map["id"] = show.id();
    map["title"] = show.title();
    map["releaseDate"] = show.firstAired();
    map["posterPath"] = show.coverUrl();

    m_coll.insert(map);
    emit tvShowAdded(show);
}

void Database::addSeason(const QVariantMap& season)
{
    QVariantMap map = season;
    map["type"] = "tvseason";

    m_coll.insert(map);
}

void Database::addEpisode(const QVariantMap& episode)
{
    // FIXME: Should be insert or replace!!
    QVariantMap map = episode;
    map["type"] = "tvepisode";

    m_coll.insert(map);
    emit tvEpisodeAdded(episode);
}

QList<TvShow> Database::allShows() const
{
    QVariantMap queryMap = {{"type", "tvshow"}};
    JsonQuery query = m_coll.execute(queryMap);

    QList<TvShow> shows;
    while (query.next()) {
        QVariantMap map = query.result();

        TvShow show;
        show.setId(map.value("id").toInt());
        show.setTitle(map.value("title").toString());
        show.setFirstAired(map.value("releaseDate").toDate());
        show.setCoverUrl(map.value("posterPath").toString());

        shows << show;
    }

    return shows;
}

QVariantMap Database::episode(int showId, int season, int epNum)
{
    QVariantMap queryMap = {{"type", "tvepisode"},
                            {"show", showId},
                            {"season", season},
                            {"episodeNumber", epNum}};
    JsonQuery query = m_coll.execute(queryMap);

    if (query.next()) {
        QVariantMap map = query.result();
        return map;
    }

    return QVariantMap();
}

QList<QVariantMap> Database::allEpisodes(int showId, int season)
{
    QVariantMap queryMap = {{"type", "tvepisode"},
                            {"show", showId}};

    if (season != -1) {
        queryMap.insert("season", season);
    }
    JsonQuery query = m_coll.execute(queryMap);

    QList<QVariantMap> epList;
    while (query.next()) {
        QVariantMap map = query.result();
        epList << map;
    }

    return epList;
}


bool Database::hasEpisodes(int show, int season)
{
    QVariantMap queryMap = {{"type", "tvepisode"},
                            {"show", show}};

    if (season != -1) {
        queryMap.insert("season", season);
    }
    JsonQuery query = m_coll.execute(queryMap);
    return query.totalCount();
}

bool Database::isWatched(const QString& url)
{
    return false;
}

void Database::markWatched(const QString& url)
{
    // FIXME: Implement me!
    /*
    QSqlQuery query(m_sqlDb);
    query.prepare("insert into watched VALUES (?)");
    query.addBindValue(fileId(url));
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
    */
}
