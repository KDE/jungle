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

static QVariant toVariant(const QString& str)
{
    bool okay = false;
    int intValue = str.toInt(&okay);
    if (okay) {
        return intValue;
    }

    return str;
}

void Jungle::GuessItJob::slotProcessFinished(int exitCode)
{
    if (exitCode) {
        qDebug() << "Process crashed for this file" << m_filePath << "with error" << exitCode;
        emitFinished();
        return;
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
        if (list.size() == 2) {
            const QString property = list.first().simplified();
            const QVariant value = toVariant(list.last().simplified());
            m_data.insert(property, value);
        }
        else if (list.size() == 1) {
            const QString property = list.first().simplified();
            QList<QVariant> varList;

            while (!stream.atEnd()) {
                QString line = stream.readLine();
                if (!line.contains(":")) {
                    line.remove('-');
                    varList << toVariant(line.simplified());
                } else {
                    QStringList list = line.split(':', QString::SkipEmptyParts);
                    const QString property = list.first().simplified();
                    const QVariant value = toVariant(list.last().simplified());
                    m_data.insert(property, value);
                    break;
                }
            }
            m_data.insert(property, varList);
        }
        else {
            qDebug() << m_filePath;
            qDebug() << line;
            Q_ASSERT(0);
        }
    }

    // Guess It marks everything as a movie if it cannot determine what it is
    QString type = m_data.value("type").toString();
    if (!m_data.contains("year") && type == QStringLiteral("movie")) {
        m_data["type"] = QStringLiteral("video");
    }

    if (type == QString("episode")) {
        m_data["type"] = "tvepisode";
    }

    emitFinished();
}

QVariantMap Jungle::GuessItJob::data() const
{
    return m_data;
}

