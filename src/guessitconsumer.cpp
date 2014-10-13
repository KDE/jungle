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

#include "guessitconsumer.h"
#include "queueinterface.h"

#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>
#include <QProcess>

using namespace Jungle;

GuessItConsumer::GuessItConsumer(const QList<QueueInterface*> outputQueues)
    : m_outputQueues(outputQueues)
    , m_inputQueue(0)
    , m_process(0)
{
}

void GuessItConsumer::itemsAdded(QueueInterface* queue)
{
    if (m_process) {
        return;
    }

    m_inputQueue = queue;

    m_input = queue->top();
    QString filePath = m_input.value("url").toString();

    Q_ASSERT(!filePath.isEmpty());

    QString guessItPath = QStandardPaths::locate(QStandardPaths::DataLocation, "guess.py");
    QString pythonPath = QStandardPaths::findExecutable("python3");

    Q_ASSERT(!guessItPath.isEmpty());
    Q_ASSERT(!pythonPath.isEmpty());

    m_process = new QProcess(this);
    m_process->setProgram(pythonPath);
    m_process->setArguments(QStringList() << guessItPath << filePath);
    connect(m_process, SIGNAL(finished(int)), this, SLOT(slotProcessFinished(int)));
    m_process->start();
}

void GuessItConsumer::slotProcessFinished(int exitCode)
{
    if (exitCode) {
        qDebug() << "Process crashed for this file" << m_input << "with error" << exitCode;
        qDebug() << "Now what?";
        qDebug() << m_process->error();

        m_process->deleteLater();
        m_process = 0;

        m_inputQueue->pop();
        if (!m_inputQueue->empty()) {
            itemsAdded(m_inputQueue);
        }
        return;
    }

    QByteArray json = m_process->readAllStandardOutput();
    json.replace('\'', '"');

    m_process->deleteLater();
    m_process = 0;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error) {
        qCritical() << error.errorString() << error.offset;
    }

    QVariantMap map = m_input;
    map.unite(doc.toVariant().toMap());

    //
    // Guess It marks everything as a movie if it cannot determine what it is
    //
    QString type = map.value("type").toString();
    if (!map.contains("year") && type == QStringLiteral("movie")) {
        map["type"] = QStringLiteral("video");
    }

    for (QueueInterface* queue : m_outputQueues) {
        queue->add(map);
    }

    m_inputQueue->pop();

    if (!m_inputQueue->empty()) {
        itemsAdded(m_inputQueue);
    }
}

