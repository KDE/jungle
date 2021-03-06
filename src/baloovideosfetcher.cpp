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

#include "baloovideosfetcher.h"

#include <QThreadPool>

using namespace Jungle;

BalooVideosFetcher::BalooVideosFetcher(QObject* parent): QObject(parent)
{

}

void BalooVideosFetcher::fetch()
{
    Baloo::Query query;
    query.setType("Video");

    Baloo::QueryRunnable* runnable = new Baloo::QueryRunnable(query, this);
    connect(runnable, SIGNAL(queryResult(Baloo::QueryRunnable*,QString)),
            this, SLOT(queryResult(Baloo::QueryRunnable*,QString)));
    connect(runnable, SIGNAL(finished(Baloo::QueryRunnable*)),
            this, SLOT(slotFinished()));

    QThreadPool* pool = QThreadPool::globalInstance();
    pool->start(runnable);
}

void BalooVideosFetcher::queryResult(Baloo::QueryRunnable*, const QString& filePath)
{
    if (!filePath.isEmpty()) {
        emit videoResult(filePath);
    }
}

void BalooVideosFetcher::slotFinished()
{
    deleteLater();
    emit finished();
}
