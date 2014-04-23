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

#include "tvepisodemodel.h"
#include "database.h"

#include <QStandardPaths>
#include <QDir>

using namespace Jungle;

TvEpisodeModel::TvEpisodeModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    names.insert(AirDateRole, "date");
    names.insert(SeasonRole, "season");
    names.insert(EpisodeNumberRole, "episodeNumber");
    setRoleNames(names);
}

int TvEpisodeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_episodes.size();
}

QVariant TvEpisodeModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid())
        return QVariant();

    TvEpisode ep = m_episodes.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return ep.name();

        case UrlRole:
            return ep.url();

        case CoverRole:
            return ep.stillUrl();

        case AirDateRole:
            return ep.airDate();

        case SeasonRole:
            return ep.season();

        case EpisodeNumberRole:
            return ep.episodeNumber();
    }

    return QVariant();
}

int TvEpisodeModel::showId() const
{
    return m_showId;
}

void TvEpisodeModel::setShowId(int id)
{
    m_showId = id;

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle";
    QDir().mkpath(dataDir);

    QString fileMapDb = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/baloo/file/fileMap.sqlite3";

    Database db(dataDir, fileMapDb);
    db.init();

    beginResetModel();
    m_episodes = db.allEpisodes(m_showId);
    endResetModel();
}

