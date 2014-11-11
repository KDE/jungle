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

#ifndef JUNGLE_NETWORK_IMAGE_CONSUMER
#define JUNGLE_NETWORK_IMAGE_CONSUMER

#include "networkconsumer.h"
#include "networkimagefetchjob.h"

namespace Jungle {

class NetworkImageConsumer : public NetworkConsumer
{
public:
    explicit NetworkImageConsumer(QNetworkAccessManager* manager, QueueInterface* input,
                                  QList<QueueInterface*> output, QObject* parent = 0)
        : NetworkConsumer(manager, input, output, parent)
        , m_manager(manager)
    {
    }

protected:
    virtual Job* fetchJob(const QVariantMap& input)
    {
        return new NetworkImageFetchJob(m_manager, input, this);
    }

private:
    QNetworkAccessManager* m_manager;
};

}

#endif
