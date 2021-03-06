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

namespace Jungle {

class TvEpisodeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString showId READ showId WRITE setShowId)

public:
    explicit TvEpisodeModel(QObject* parent = 0);

    enum Roles {
        NameRole = Qt::DisplayRole,
        UrlRole = Qt::UserRole + 1,
        CoverRole,
        AirDateRole,
        SeasonRole,
        EpisodeNumberRole
    };

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QString showId() const;
    void setShowId(const QString& id);

    int season() const;
    /**
     * Set the season from which the episodes should be shown.
     * Setting the season to -1 (the default), results in all the available
     * episodes being shown
     */
    void setSeason(int season);

private slots:
    void slotNewTvEpisode(const QVariantMap& episode);

private:
    QList<QVariantMap> m_episodes;
    QString m_showId;
    int m_season;
};
}

#endif // JUNGLE_EPISODEMODEL_H
