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

#include <QSqlDatabase>

#include <QObject>
#include <QStringList>

#include <tmdbqt/themoviedbapi.h>

class Feeder : public QObject
{
    Q_OBJECT
public:
    explicit Feeder(QSqlDatabase& sqlDb, QObject* parent = 0);
    virtual ~Feeder();

private Q_SLOTS:
    void fetchFiles();
    void processNext();

    void slotMovieResult(TmdbQt::SearchJob* job);

private:
    QStringList m_files;
    TmdbQt::TheMovieDbApi m_api;

    QSqlDatabase& m_sqlDb;

    bool fetchNameAndYear(const QString& fileName, QString& name, int& year);
    bool filterUrl(const QString& url);
};

#endif // FEEDER_H
