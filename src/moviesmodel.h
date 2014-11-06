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

#ifndef JUNGLE_MOVIESMODEL_H
#define JUNGLE_MOVIESMODEL_H

#include <QVariant>
#include <QAbstractListModel>
#include <QDate>

namespace Jungle {

class MoviesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MoviesModel(QObject* parent = 0);

    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual int rowCount(const QModelIndex& parent) const;

    enum Roles {
        UrlRole = Qt::UserRole + 1,
        CoverRole,
        ReleaseDateRole,
        YearRole
    };

private slots:
    void slotPopulate();
    void slotNewMovie(const QVariantMap& movie);

private:
    QList<QVariantMap> m_movies;
};
}

#endif // JUNGLE_MOVIESMODEL_H
