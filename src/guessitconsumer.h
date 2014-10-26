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

#ifndef GUESSITCONSUMER_H
#define GUESSITCONSUMER_H

#include "asyncjobconsumer.h"
#include "guessitjob.h"

namespace Jungle {

class GuessItConsumer : public AsyncJobConsumer
{
public:
    explicit GuessItConsumer(QList<QueueInterface*> output, QObject* parent = 0)
        : AsyncJobConsumer(output, parent)
    {
    }

protected:
    virtual Job* fetchJob(const QVariantMap& input)
    {
        QString filePath = input.value("url").toString();
        Q_ASSERT(!filePath.isEmpty());

        return new GuessItJob(filePath);
    }
};

}

#endif // GUESSITCONSUMER_H