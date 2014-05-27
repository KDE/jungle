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

using namespace Jungle;

TvEpisodeModel::TvEpisodeModel(QObject* parent)
    : QAbstractListModel(parent)
    , m_showId(0)
    , m_season(-1)
{
    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    names.insert(AirDateRole, "date");
    names.insert(SeasonRole, "season");
    names.insert(EpisodeNumberRole, "episodeNumber");
    setRoleNames(names);

    connect(Database::instance(), SIGNAL(tvEpisodeAdded(TvEpisode)),
            this, SLOT(slotNewTvEpisode(TvEpisode)));
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
        case Qt::DisplayRole || NameRole:
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

    beginResetModel();
    m_episodes = Database::instance()->allEpisodes(m_showId);
    endResetModel();
}

void TvEpisodeModel::slotNewTvEpisode(const TvEpisode& episode)
{
    //If the added episode is not of this tvshow, skip it.
    if (episode.show() != m_showId) {
        return;
    }

    beginInsertRows(QModelIndex(), m_episodes.size(), m_episodes.size());
    m_episodes << episode;
    endInsertRows();
}

void TvEpisodeModel::setSeason(int season)
{
    if (!m_showId) {
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

