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

#include "tvshowsmodel.h"
#include "database.h"

#include <QModelIndex>
#include <QTimer>
#include <QDir>

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using namespace Jungle;

TvShowsModel::TvShowsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QTimer::singleShot(0, this, SLOT(slotPopulate()));

    QHash<int, QByteArray> names = roleNames();
    names.insert(CoverRole, "cover");
    names.insert(ReleaseDateRole, "date");
    setRoleNames(names);
}

void TvShowsModel::slotPopulate()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle";
    QDir().mkpath(dataDir);

    QString fileMapDb = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/baloo/file/fileMap.sqlite3";

    Database db(dataDir, fileMapDb);
    db.init();

    beginResetModel();
    m_shows = db.allShows();
    endResetModel();
}

QVariant TvShowsModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    Show show = m_shows[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return show.title();

        case CoverRole:
            return show.coverUrl();

        case ReleaseDateRole:
            return show.firstAired();
    }

    return QVariant();
}

int TvShowsModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int TvShowsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_shows.size();
}
