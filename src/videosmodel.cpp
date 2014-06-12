/*
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

#include <QTimer>

#include <Baloo/query.h>
#include <Baloo/resultiterator.h>

using namespace Jungle;

VideosModel::VideosModel(QObject* parent): QAbstractListModel(parent)
{
    QTimer::singleShot(0, this, SLOT(init()));

    QHash<int, QByteArray> names = roleNames();
    names.insert(UrlRole, "url");
    names.insert(CoverRole, "cover");
    setRoleNames(names);
}

void VideosModel::init()
{
    Baloo::Query query;
    query.setType("Video");

    auto it = query.exec();

    beginResetModel();
    m_results.clear();
    while (it.next()) {
        m_results << it.result();
    }
    endResetModel();
}

int VideosModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_results.size();
}

QVariant VideosModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    int row = index.row();
    switch (role) {
        case Qt::DisplayRole:
            return m_results[row].text();

        case Qt::DecorationRole:
            return m_results[row].icon();

        case UrlRole:
            return m_results[row].url();

        case CoverRole:
            return QLatin1Literal("/home/vishesh/frozen.jpg");
    }

    return QVariant();
}
