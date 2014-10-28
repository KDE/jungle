/*
 * <one line to give the library's name and an idea of what it does.>
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

#include "networkimagefetchjob.h"

#include <QStandardPaths>
#include <QUuid>
#include <QFile>

Jungle::NetworkImageFetchJob::NetworkImageFetchJob(const QVariantMap& input, QObject* parent)
    : Job(parent)
    , m_input(input)
{
    connect(&m_network, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotNetworkReply(QNetworkReply*)));

    QString val = m_input.value("thumbnail").toString();
    if (val.startsWith("http://") || val.startsWith("https://")) {
        m_network.get(QNetworkRequest(QUrl(val)));
    } else {
        emitFinished();
    }
}

QVariantMap Jungle::NetworkImageFetchJob::data() const
{
    return m_input;
}

void Jungle::NetworkImageFetchJob::slotNetworkReply(QNetworkReply* reply)
{
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

    m_input["thumbnail"] = url;
    emitFinished();
}
