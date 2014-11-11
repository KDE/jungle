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

#include "networkimagefetchjob.h"

#include <QStandardPaths>
#include <QUuid>
#include <QFile>
#include <QTimer>
#include <QNetworkRequest>

Jungle::NetworkImageFetchJob::NetworkImageFetchJob(QNetworkAccessManager* manager,
                                                   const QVariantMap& input, QObject* parent)
    : Job(parent)
    , m_input(input)
    , m_networkRequests(0)
{
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));

    for (auto it = m_input.begin(); it != m_input.end(); it++) {
        if (it.value().type() != QVariant::String) {
            continue;
        }

        QString val = it.value().toString();
        if (val.startsWith("http://") || val.startsWith("https://")) {
            QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(val)));
            connect(reply, SIGNAL(finished()), this, SLOT(slotNetworkReply()));
            m_networkRequests++;
        }
    }

    if (m_networkRequests == 0) {
        m_input.clear();
        QTimer::singleShot(0, this, SLOT(emitFinished()));
    }
}

QVariantMap Jungle::NetworkImageFetchJob::data() const
{
    return m_input;
}

void Jungle::NetworkImageFetchJob::slotNetworkReply()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    QString name = QUuid::createUuid().toString();
    QString type = m_input.value("type").toString();
    QString url = QString::fromLatin1("%1/jungle/%2-%3")
                  .arg(dataDir).arg(type).arg(name);

    QByteArray data = reply->readAll();

    if (data.size() < 100 || data.contains("Format not Supported")) {
        m_input.clear();
        emitFinished();
        return;
    }

    QFile file(url);
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    for (auto it = m_input.begin(); it != m_input.end(); it++) {
        if (it.value().type() != QVariant::String) {
            continue;
        }

        QString val = it.value().toString();
        if (val == reply->url().toString()) {
            it.value() = url;
        }
    }

    m_networkRequests--;
    if (m_networkRequests == 0) {
        emitFinished();
    }
}
