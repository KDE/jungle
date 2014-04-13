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

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace Jungle;

MoviesModel::MoviesModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QTimer::singleShot(0, this, SLOT(slotPopulate()));

    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    names.insert(ReleaseDateRole, "date");
    setRoleNames(names);
}

void MoviesModel::slotPopulate()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle";
    QDir().mkpath(dataDir);

    Database db(dataDir);
    db.init();

    QSqlQuery query(db.sqlDatabase());
    query.prepare("select url, mid, title, releaseDate, posterPath from movies");
    query.exec();

    beginResetModel();
    m_movies.clear();
    while (query.next()) {
        MovieInfo info;
        info.url = query.value("url").toString();
        info.id = query.value("mid").toInt();
        info.title = query.value("title").toString();
        info.releaseDate = query.value("releaseDate").toDate();
        info.posterUrl = query.value("posterPath").toString();

        m_movies << info;
    }
    endResetModel();
}

QVariant MoviesModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    MovieInfo info = m_movies[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return info.title;

        case UrlRole:
            return info.url;

        case CoverRole:
            return info.posterUrl;

        case ReleaseDateRole:
            return info.releaseDate;
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
