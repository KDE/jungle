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

#ifndef EJDB_DATABASE_H
#define EJDB_DATABASE_H

#include <QVariantMap>
#include <tcejdb/ejdb.h>

namespace Jungle {

class Database
{
public:
    Database(const QString& dbPath);
    virtual ~Database();

    QByteArray add(const QVariantMap& map);

    QVariantMap fetch(const QByteArray& id);

private:
    EJDB* m_jdb;
    EJCOLL* m_coll;
};
}

#endif
