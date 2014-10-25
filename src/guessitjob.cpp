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
#include <QDebug>

Jungle::GuessItJob::GuessItJob(const QString& filePath)
    : m_filePath(filePath)
{
    static QString exePath = QStandardPaths::findExecutable("guessit");
    Q_ASSERT(!exePath.isEmpty());

    m_process = new QProcess(this);
    m_process->setProgram(exePath);
    m_process->setArguments(QStringList() << "-y" << filePath);
    connect(m_process, SIGNAL(finished(int)), this, SLOT(slotProcessFinished(int)));
    m_process->start();
}

void Jungle::GuessItJob::slotProcessFinished(int exitCode)
{
    deleteLater();

    if (exitCode) {
        qDebug() << "Process crashed for this file" << m_filePath << "with error" << exitCode;
        Q_ASSERT(0);
    }

    QString output = QString::fromUtf8(m_process->readAllStandardOutput());
    QTextStream s(&output, QIODevice::ReadOnly);

    QString line = s.readLine();
    Q_ASSERT(line.startsWith("?"));

    QString yaml = s.readAll().mid(1);
    QTextStream stream(&yaml, QIODevice::ReadOnly);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList list = line.split(':', QString::SkipEmptyParts);
        Q_ASSERT(list.size() == 2);

        const QString property = list.first().simplified();
        const QString value = list.last().simplified();

        bool okay = false;
        int intValue = value.toInt(&okay);
        if (okay) {
            m_data.insert(property, intValue);
        } else {
            m_data.insert(property, value);
        }
    }

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

