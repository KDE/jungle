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

#ifndef JUNGLE_GUESSITJOB_H
#define JUNGLE_GUESSITJOB_H

#include <QObject>
#include <QVariantMap>
#include <QProcess>

class DataQueueInterface;

namespace Jungle {

class GuessItJob : public QObject
{
    Q_OBJECT
public:
    GuessItJob(const QString& filePath, const QList<DataQueueInterface*>& queues);

    void start();

private Q_SLOTS:
    void slotProcessFinished(int exitCode);

private:
    QString m_filePath;
    QProcess m_process;

    QList<DataQueueInterface*> m_queues;
};
}

#endif // JUNGLE_GUESSITJOB_H
