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

#include "videosmodel.h"
#include "database.h"

#include <QUrl>
#include <QTimer>

using namespace Jungle;

VideosModel::VideosModel(QObject* parent): QAbstractListModel(parent)
{
    QTimer::singleShot(0, this, SLOT(slotPopulate()));

    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    setRoleNames(names);

    connect(Database::instance(), SIGNAL(videoAdded(QVariantMap)),
            this, SLOT(slotNewVideo(QVariantMap)));
}

void VideosModel::slotPopulate()
{
    beginResetModel();
    m_videos = Database::instance()->allVideos();
    endResetModel();
}

void VideosModel::slotNewVideo(const QVariantMap& video)
{
    QString url = video.value("url").toString();
    for (int i = 0; i < m_videos.size(); i++) {
        if (m_videos[i].value("url").toString() == url) {
            m_videos[i] = video;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }

    beginInsertRows(QModelIndex(), m_videos.size(), m_videos.size());
    m_videos << video;
    endInsertRows();
}

QVariant VideosModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    QVariantMap video = m_videos[index.row()];
    switch (role) {
        case Qt::DisplayRole:
            return QUrl::fromLocalFile(video["url"].toString()).fileName();

        case UrlRole:
            return video["url"].toString();
    }

    return QVariant();
}

int VideosModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_videos.size();
}
