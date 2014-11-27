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

#ifndef _KVARIANT_STORE_H
#define _KVARIANT_STORE_H

#include "kvariantstore_export.h"
#include <QVariantMap>
#include <tcejdb/ejdb.h>

class KVariantCollection;

class KVARIANTSTORE_EXPORT KVariantStore
{
public:
    KVariantStore();
    ~KVariantStore();

    void setPath(const QString& filePath);
    QString filePath() const;

    bool open();

    KVariantCollection collection(const QString& name);

private:
    EJDB* m_jdb;

    QString m_filePath;
};

#endif
