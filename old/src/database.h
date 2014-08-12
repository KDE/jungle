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
#include "tvshow.h"

namespace Jungle {

class JUNGLE_EXPORT Database : public QObject
{
    Q_OBJECT
public:
    /**
     * Create a database at path \p path which will contain all of the
     * data. The parameter \p fileMapDb should be the path to a sqlite
     * db which maps an integer to a file.
     */
    Database(const QString& path, const QString& fileMapDb);
    ~Database();

    static Database* instance();

    bool init();
    bool initialized();

    void addMovie(const Movie& movie);
    QList<Movie> allMovies() const;

    bool hasVideo(int fileId);
    void addVideo(const QString& url);

    bool isWatched(const QString& url);
    bool isWatched(int fileId);
    void markWatched(const QString& url);

    int showId(const QString& name);
    void addShow(const TvShow& show);

    QList<TvShow> allShows() const;

    bool hasEpisodes(int show, int season);
    void addEpisode(const TvEpisode& episode);

    TvEpisode episode(int showId, int season, int epNum);
    QList<TvEpisode> allEpisodes(int showId, int season = -1);

signals:
    void movieAdded(const Movie& movie);
    void tvShowAdded(const TvShow& show);
    void tvEpisodeAdded(const TvEpisode& episode);

private:
    int fileId(const QString& url);
    QString fileUrl(int fid);

    QString m_path;
    QString m_fileMapDb;
    bool m_initialized;

    QSqlDatabase m_sqlDb;
};

}

#endif // DATABASE_H
