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

#include "moviedbconsumer.h"
#include "queueinterface.h"

using namespace Jungle;

MovieDbConsumer::MovieDbConsumer(QList<QueueInterface*> output, QObject* parent)
    : AsyncJobConsumer(output, parent)
{
    m_store = new TheMovieDbStore(this);
}

Job* MovieDbConsumer::fetchJob(const QVariantMap& input)
{
    const QString type = input.value("type").toString();
    if (type == QStringLiteral("movie")) {
        if (!input.contains("year") || !input.contains("title"))
            return 0;

        const QString name = input.value("title").toString();
        const int year = input.value("year").toInt();

        return m_store->fetchMovie(name, year);
    }

    if (type == QStringLiteral("tvshow")) {
        Q_ASSERT(input.contains("title"));

        const QString title = input.value("title").toString();
        return m_store->fetchTvShow(title);
    }

    return 0;
}

