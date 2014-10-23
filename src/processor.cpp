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

#include "processor.h"
#include "guessitconsumer.h"
#include "moviedbconsumer.h"
#include "databaseconsumer.h"

#include <QVariantMap>
#include <QDebug>

using namespace Jungle;

Processor::Processor()
{
    QList<QueueInterface*> giOutputQ = {&m_saveQueue, &m_movieDbQueue};
    GuessItConsumer* guessItConsumer = new GuessItConsumer(giOutputQ);
    m_guessItQueue.setConsumer(guessItConsumer);

    QList<QueueInterface*> mdbOutputQ = {&m_saveQueue};
    MovieDbConsumer* movieDbConsumer = new MovieDbConsumer(giOutputQ);
    m_movieDbQueue.setConsumer(movieDbConsumer);

    DatabaseConsumer* dbConsumer = new DatabaseConsumer();
    m_saveQueue.setConsumer(dbConsumer);
}

void Processor::addFile(const QString& filePath)
{
    Q_ASSERT(!filePath.isEmpty());

    QVariantMap map;
    map.insert("url", filePath);

    m_guessItQueue.add(map);
}
