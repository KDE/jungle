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

    QVariantMap item(const QString& key, const QVariant& value);
    QVariantMap query(const QVariantMap& map);

    void add(const QVariantMap& item);


    QList<QVariantMap> allMovies() const;

    bool hasVideo(const QString& url);

    QString showId(const QString& name);

    QList<QVariantMap> allShows() const;

    bool hasEpisodes(int show, int season);

    QVariantMap episode(int showId, int season, int epNum);
    QList<QVariantMap> allEpisodes(const QString& showId, int season = -1);

    QList<QVariantMap> allVideos() const;
signals:
    void movieAdded(const QVariantMap& movie);
    void tvShowAdded(const QVariantMap& show);
    void tvEpisodeAdded(const QVariantMap& episode);

private:
    QString m_path;
    bool m_initialized;

    JsonDatabase m_jdb;
    JsonCollection m_coll;
};

}

#endif // DATABASE_H
