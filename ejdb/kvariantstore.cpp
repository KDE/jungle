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

#include "kvariantstore.h"
#include "kvariantcollection.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

KVariantStore::KVariantStore()
{
    m_jdb = ejdbnew();
}

KVariantStore::~KVariantStore()
{
    ejdbclose(m_jdb);
    ejdbdel(m_jdb);
}

QString KVariantStore::filePath() const
{
    return m_filePath;
}

void KVariantStore::setPath(const QString& filePath)
{
    m_filePath = filePath;
}

bool KVariantStore::open()
{
    QByteArray path = QFile::encodeName(m_filePath);
    if (!ejdbopen(m_jdb, path.constData(), JBOWRITER | JBOCREAT)) {
        qDebug() << "Could not open db" << m_filePath;
        return false;
    }

    return true;
}

KVariantCollection KVariantStore::collection(const QString& name)
{
    return KVariantCollection(m_jdb, name);
}
