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

#ifndef JUNGLE_EPISODEMODEL_H
#define JUNGLE_EPISODEMODEL_H

#include <QAbstractListModel>
#include "tvepisode.h"

namespace Jungle {

class TvEpisodeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int showId READ showId WRITE setShowId)

public:
    explicit TvEpisodeModel(QObject* parent = 0);

    enum Roles {
        UrlRole = Qt::UserRole + 1,
        CoverRole,
        AirDateRole,
        SeasonRole,
        EpisodeNumberRole
    };

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    int showId() const;
    void setShowId(int id);

private:
    QList<TvEpisode> m_episodes;
    int m_showId;
};
}

#endif // JUNGLE_EPISODEMODEL_H
