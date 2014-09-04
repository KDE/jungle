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

#include "guessitjob.h"
#include "interfaces/dataqueueinterface.h"

#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>

using namespace Jungle;

GuessItJob::GuessItJob(const QString& filePath, const QList<DataQueueInterface*>& queues)
    : m_filePath(filePath)
    , m_queues(queues)
{
}

void GuessItJob::start()
{
    QString guessItPath = QStandardPaths::locate(QStandardPaths::DataLocation, "guess.py");
    QString pythonPath = QStandardPaths::findExecutable("python3");

    Q_ASSERT(!guessItPath.isEmpty());
    Q_ASSERT(!pythonPath.isEmpty());

    m_process.setProgram(pythonPath);
    m_process.setArguments(QStringList() << guessItPath << m_filePath);
    connect(&m_process, SIGNAL(finished(int)), this, SLOT(slotProcessFinished(int)));
    m_process.start();
}

void GuessItJob::slotProcessFinished(int exitCode)
{
    if (exitCode) {
        return;
    }

    QByteArray json = m_process.readAllStandardOutput();
    json.replace('\'', '"');

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error) {
        qCritical() << error.errorString() << error.offset;
    }
    QVariantMap map = doc.toVariant().toMap();

    for (DataQueueInterface* queue : m_queues) {
        queue->add(map);
    }

    // Should mark the original queue with job done!
}


