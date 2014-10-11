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

#include "consumerinterface.h"
#include <QProcess>

namespace Jungle {

class GuessItConsumer : public QObject, public ConsumerInterface
{
    Q_OBJECT
public:
    GuessItConsumer(const QList<QueueInterface*> outputQueues);

    virtual void itemsAdded(QueueInterface* queue);

private slots:
    void slotProcessFinished(int exitCode);

private:
    QList<QueueInterface*> m_outputQueues;
    QueueInterface* m_inputQueue;

    QProcess* m_process;
    QVariantMap m_input;
};

}

#endif // GUESSITCONSUMER_H
