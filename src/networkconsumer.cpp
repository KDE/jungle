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

#include "networkconsumer.h"

using namespace Jungle;

NetworkConsumer::NetworkConsumer(QNetworkAccessManager* manager,
                                 QList<QueueInterface*> output, QObject* parent)
    : AsyncJobConsumer(output, parent)
{
    bool accessible = (manager->networkAccessible() == QNetworkAccessManager::Accessible);
    setReady(accessible);

    connect(manager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(slotNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
}

void NetworkConsumer::slotNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility access)
{
    if (access == QNetworkAccessManager::Accessible) {
        setReady(true);
    }
}
