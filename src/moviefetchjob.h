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

#ifndef MOVIEFETCHJOB_H
#define MOVIEFETCHJOB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <tmdbqt/themoviedbapi.h>

namespace Jungle {

class MovieFetchJob : public QObject
{
    Q_OBJECT
public:
    MovieFetchJob(TmdbQt::SearchJob* job, const QString& searchTerm,
                  int year, QObject* parent = 0);

    QVariantMap data() const { return m_data; }
    QString url() const { return m_url; }

signals:
    void result(MovieFetchJob* job);

private slots:
    void slotMovieResult(TmdbQt::SearchJob* job);

private:
    QString m_url;
    QString m_searchTerm;
    int m_year;

    QVariantMap m_data;
};

}
#endif // MOVIEFETCHJOB_H
