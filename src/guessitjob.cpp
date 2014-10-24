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

#include "guessitjob.h"

#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>

Jungle::GuessItJob::GuessItJob(const QString& filePath)
    : m_filePath(filePath)
{
    static QString guessItPath = QStandardPaths::locate(QStandardPaths::DataLocation, "guess.py");
    static QString pythonPath = QStandardPaths::findExecutable("python2");

    Q_ASSERT(!guessItPath.isEmpty());
    Q_ASSERT(!pythonPath.isEmpty());

    m_process = new QProcess(this);
    m_process->setProgram(pythonPath);
    m_process->setArguments(QStringList() << guessItPath << filePath);
    connect(m_process, SIGNAL(finished(int)), this, SLOT(slotProcessFinished(int)));
    m_process->start();
}

void Jungle::GuessItJob::slotProcessFinished(int exitCode)
{
    deleteLater();

    if (exitCode) {
        qDebug() << "Process crashed for this file" << m_filePath << "with error" << exitCode;
        emit finished(this);
        return;
    }

    QByteArray json = m_process->readAllStandardOutput();
    json.replace('\'', '"');

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error) {
        qCritical() << error.errorString() << error.offset;
    }

    m_data = doc.toVariant().toMap();

    // Guess It marks everything as a movie if it cannot determine what it is
    QString type = m_data.value("type").toString();
    if (!m_data.contains("year") && type == QStringLiteral("movie")) {
        m_data["type"] = QStringLiteral("video");
    }

    emit finished(this);
}

QVariantMap Jungle::GuessItJob::data() const
{
    return m_data;
}

