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

#ifndef THUMBNAILCREATIONJOB_H
#define THUMBNAILCREATIONJOB_H

#include <QObject>
#include <QString>
#include <QVariant>

#include <kio/previewjob.h>

namespace Jungle {

class ThumbnailCreationJob : public QObject
{
    Q_OBJECT
public:
    ThumbnailCreationJob(const QString& filePath);

    QVariantMap data() const;

signals:
    void finished(ThumbnailCreationJob* job);

private slots:
    void gotPreview(const KFileItem& item, const QPixmap& pixmap);
    void slotFailed();

private:
    QString m_filePath;
    QVariantMap m_data;
};

}

#endif // THUMBNAILCREATIONJOB_H
