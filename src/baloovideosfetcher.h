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

#ifndef JUNGLE_BALOOVIDEOSFETCHER_H
#define JUNGLE_BALOOVIDEOSFETCHER_H

#include <QStringList>
#include <QObject>

#include <Baloo/Query>
#include <Baloo/QueryRunnable>

namespace Jungle {

class BalooVideosFetcher : public QObject
{
    Q_OBJECT
public:
    explicit BalooVideosFetcher(QObject* parent = 0);

    void fetch();

signals:
    void videoResult(const QString& filePath);
    void finished();

private slots:
    void queryResult(Baloo::QueryRunnable*, const QString& filePath);
    void slotFinished();
};
}

#endif // JUNGLE_BALOOVIDEOSFETCHER_H
