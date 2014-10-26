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

#include "asyncjobconsumer.h"
#include "themoviedbstore.h"

namespace Jungle {

class MovieDbConsumer : public AsyncJobConsumer
{
public:
    explicit MovieDbConsumer(QList<QueueInterface*> outputQueues, QObject* parent = 0);

protected:
    virtual Job* fetchJob(const QVariantMap& input);

private:
    TheMovieDbStore* m_store;
};
}

#endif
