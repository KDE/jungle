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

#include "thumbnailcreationjob.h"

#include <QStandardPaths>
#include <QDir>
#include <QSize>
#include <QUuid>
#include <QPixmap>
#include <QDebug>

using namespace Jungle;

ThumbnailCreationJob::ThumbnailCreationJob(const QString& filePath)
    : m_filePath(filePath)
{
    KFileItemList list;
    list << KFileItem(QUrl::fromLocalFile(filePath));

    qDebug() << KIO::PreviewJob::availablePlugins();
    auto job = KIO::filePreview(list, QSize(512, 512));
    connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)),
            this, SLOT(gotPreview(KFileItem,QPixmap)));
    connect(job, SIGNAL(failed(KFileItem)),
            this, SLOT(slotFailed()));

    job->start();
}

QVariantMap ThumbnailCreationJob::data() const
{
    return m_data;
}

void ThumbnailCreationJob::gotPreview(const KFileItem&, const QPixmap& pixmap)
{
    static QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle/";

    QString path = dataDir + QUuid::createUuid().toString() + ".jpeg";
    pixmap.save(path, "JPEG");

    m_data["thumbnail"] = path;
    qDebug() << m_data;

    deleteLater();
    emit finished(this);
}

void ThumbnailCreationJob::slotFailed()
{
    qDebug() << m_filePath;
    deleteLater();
    emit finished(this);
}
