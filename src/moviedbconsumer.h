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

#ifndef JUNGLE_MOVIE_DBCONSUMER_H
#define JUNGLE_MOVIE_DBCONSUMER_H

#include "consumerinterface.h"
#include "themoviedbstore.h"

namespace Jungle {

class MovieDbConsumer : public QObject, public ConsumerInterface
{
    Q_OBJECT
public:
    explicit MovieDbConsumer(QList<QueueInterface*> outputQueues, QObject* parent = 0);

    virtual void itemsAdded(QueueInterface* queue);

private slots:
    void slotResult(MovieFetchJob* job);

private:
    QList<QueueInterface*> m_outputQueues;
    QueueInterface* m_inputQueue;

    TheMovieDbStore* m_store;
    MovieFetchJob* m_job;

    QVariantMap m_input;
};
}

#endif
