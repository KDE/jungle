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

#ifndef FEEDER_H
#define FEEDER_H

#include "database.h"
#include "themoviedbstore.h"

#include <QObject>
#include <QStringList>

namespace Jungle {

class MovieFetchJob;
class TvShowFetchJob;

class Feeder : public QObject
{
    Q_OBJECT
public:
    explicit Feeder(Database* db, QObject* parent = 0);
    virtual ~Feeder();

private Q_SLOTS:
    void fetchFiles();
    void processNext();

    void slotResult(MovieFetchJob* job);
    void slotFileMetaDataChanged(const QStringList& list);
private:
    /**
     * Remove extra crap from the file name
     */
    static QString filterFileName(const QString& fileName);
    static bool filterUrl(const QString& url);

    int fetchOrCreateShow(const QString& show);

    QStringList m_files;

    Database* m_db;
    TheMovieDbStore* m_theMovieDb;
};

}

#endif // FEEDER_H
