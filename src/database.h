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

#include "tvshow.h"

#include "jsondatabase.h"
#include "jsoncollection.h"

namespace Jungle {

class JUNGLE_EXPORT Database : public QObject
{
    Q_OBJECT
public:
    Database(const QString& path);
    ~Database();

    static Database* instance();

    bool init();
    bool initialized();

    void addMovie(const QVariantMap& movie);
    QList<QVariantMap> allMovies() const;

    void addVideo(const QString& url);

    bool isWatched(const QString& url);
    bool isWatched(int fileId);
    void markWatched(const QString& url);

    int showId(const QString& name);
    void addShow(const TvShow& show);
    void addSeason(const QVariantMap& season);

    QList<TvShow> allShows() const;

    bool hasEpisodes(int show, int season);
    void addEpisode(const QVariantMap& episode);

    QVariantMap episode(int showId, int season, int epNum);
    QList<QVariantMap> allEpisodes(int showId, int season = -1);

signals:
    void movieAdded(const QVariantMap& movie);
    void tvShowAdded(const TvShow& show);
    void tvEpisodeAdded(const QVariantMap& episode);

private:
    QString m_path;
    bool m_initialized;

    JsonDatabase m_jdb;
    JsonCollection m_coll;
};

}

#endif // DATABASE_H
