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

#include <QDate>

using namespace Jungle;

TvEpisodeModel::TvEpisodeModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_season(-1)
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    names.insert(AirDateRole, "date");
    names.insert(SeasonRole, "season");
    names.insert(EpisodeNumberRole, "episodeNumber");
    setRoleNames(names);

    connect(Database::instance(), SIGNAL(tvEpisodeAdded(QVariantMap)),
            this, SLOT(slotNewTvEpisode(QVariantMap)));
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

    if(!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= m_episodes.count()) {
        return QVariant();
    }

    QVariantMap ep = m_episodes.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            if (ep.contains("name")) {
                return ep["name"].toString();
            }
            return ep.value("series").toString();

        case UrlRole:
            return ep["url"].toString();

        case CoverRole:
            return ep["stillPath"].toString();

        case AirDateRole:
            return ep["airDate"].toDate();

        case SeasonRole:
            return ep["seasonNumber"].toInt();

        case EpisodeNumberRole:
            return ep["episodeNumber"].toInt();
    }

    return QVariant();
}

QString TvEpisodeModel::showId() const
{
    return m_showId;
}

void TvEpisodeModel::setShowId(const QString& id)
{
    m_showId = id;

    beginResetModel();
    m_episodes = Database::instance()->allEpisodes(m_showId);
    endResetModel();
}

void TvEpisodeModel::slotNewTvEpisode(const QVariantMap& episode)
{
    //If the added episode is not of this tvshow, skip it.
    if (episode["showId"].toString() != m_showId) {
        return;
    }

    QVariant url = episode.value("url");
    for (int i = 0; i < m_episodes.size(); i++) {
        if (m_episodes[i].value("url") == url) {
            m_episodes[i] = episode;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_episodes.size(), m_episodes.size());
    m_episodes << episode;
    endInsertRows();
}

void TvEpisodeModel::setSeason(int season)
{
    if (m_showId.isEmpty()) {
        beginResetModel();
        m_episodes.clear();
        endResetModel();
        return;
    }
    m_season = season;

    beginResetModel();
    m_episodes = Database::instance()->allEpisodes(m_showId, season);
    endResetModel();
}

