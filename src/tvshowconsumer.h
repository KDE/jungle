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

#ifndef JUNGLE_TVSHOW_CONSUMER_H
#define JUNGLE_TVSHOW_CONSUMER_H

#include "asyncjobconsumer.h"
#include "themoviedbstore.h"
#include "tvshowfetchjob.h"

namespace Jungle {

class TvShowConsumer : public AsyncJobConsumer
{
public:
    explicit TvShowConsumer(TheMovieDbStore* api, QList<QueueInterface*> output, QObject* parent = 0)
        : AsyncJobConsumer(output, parent)
        , m_api(api)
    {
    }

protected:
    virtual Job* fetchJob(const QVariantMap& input)
    {
        const QString type = input.value("type").toString();
        if (type == QStringLiteral("tvshow")) {
            Q_ASSERT(input.contains("title"));

            const QString title = input.value("title").toString();
            const QString dbShowId = input.value("id").toString();
            return m_api->fetchTvShow(title, dbShowId);
        }

        return 0;
    }

private:
    TheMovieDbStore* m_api;
};

}

#endif
