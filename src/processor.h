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

#ifndef JUNGLE_PROCESSOR_H
#define JUNGLE_PROCESSOR_H

#include <QObject>
#include <QString>
#include "queue.h"

namespace Jungle {

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor();
    virtual ~Processor();

public slots:
    void addFile(const QString& filePath);
    void removeFile(const QString& filePath);
    void resume();

private:
    Queue m_guessItQueue;
    Queue m_tvShowGenQueue;
    Queue m_movieQueue;
    Queue m_tvShowQueue;
    Queue m_tvSeasonQueue;
    Queue m_networkImageQueue;
    Queue m_saveQueue;

    QList<Queue*> m_queues;
};
}

#endif // JUNGLE_PROCESSOR_H
