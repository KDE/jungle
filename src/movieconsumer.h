/*
 * Copyright (C) 2014  Vishesh Handa <vhanda@kde.org>
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

#ifndef JUNGLE_MOVIECONSUMER_H
#define JUNGLE_MOVIECONSUMER_H

#include "themoviedbconsumer.h"
#include "moviefetchjob.h"

namespace Jungle {

class MovieConsumer : public TheMovieDbConsumer
{
public:
    explicit MovieConsumer(TheMovieDbStore* api, QueueInterface* input, QList<QueueInterface*> output, QObject* parent = 0)
        : TheMovieDbConsumer(api, input, output, parent)
        , m_api(api)
    {
    }

protected:
    virtual Job* fetchJob(const QVariantMap& input)
    {
        const QString type = input.value("type").toString();
        if (type == QStringLiteral("movie")) {
            if (!input.contains("year") || !input.contains("title"))
                return 0;

            const QString name = input.value("title").toString();
            const int year = input.value("year").toInt();

            return m_api->fetchMovie(name, year);
        }

        return 0;
    }

private:
    TheMovieDbStore* m_api;
};

}

#endif
