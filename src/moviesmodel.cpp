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

#include "moviesmodel.h"
#include "database.h"

#include <QModelIndex>
#include <QTimer>
#include <QDir>
#include <QDebug>

using namespace Jungle;

MoviesModel::MoviesModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QTimer::singleShot(0, this, SLOT(slotPopulate()));

    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    names.insert(ReleaseDateRole, "date");
    names.insert(YearRole, "year");
    setRoleNames(names);

    connect(Database::instance(), SIGNAL(movieAdded(QVariantMap)),
            this, SLOT(slotNewMovie(QVariantMap)));
}

void MoviesModel::slotPopulate()
{
    beginResetModel();
    m_movies = Database::instance()->allMovies();
    endResetModel();
}

void MoviesModel::slotNewMovie(const QVariantMap& movie)
{
    QString url = movie.value("url").toString();
    for (int i = 0; i < m_movies.size(); i++) {
        if (m_movies[i].value("url").toString() == url) {
            m_movies[i] = movie;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_movies.size(), m_movies.size());
    m_movies << movie;
    endInsertRows();
}

QVariant MoviesModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    QVariantMap movie = m_movies[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return movie.value("title").toString();

        case UrlRole:
            return movie.value("url").toString();

        case CoverRole:
            return movie.value("posterPath").toString();

        case ReleaseDateRole:
            return movie.value("releaseDate").toDate();

        case YearRole:
            return movie.value("year").toInt();
    }

    return QVariant();
}

int MoviesModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int MoviesModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_movies.size();
}
