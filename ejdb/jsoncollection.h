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

#ifndef JSONCOLLECTION_H
#define JSONCOLLECTION_H

#include "kjsondatabase_export.h"
#include <QVariantMap>
#include <tcejdb/ejdb.h>

class JsonDatabase;
class JsonQuery;

class JUNGLE_EXPORT JsonCollection
{
public:
    JsonCollection();
    ~JsonCollection();

    QString collectionName() const;

    QString insert(const QVariantMap& map);
    QVariantMap fetch(const QString& id) const;
    bool remove(const QString& id);

    JsonQuery execute(const QVariantMap& query) const;
private:
    JsonCollection(EJDB* db, const QString& name);

    EJDB* m_db;
    EJCOLL* m_coll;
    QString m_collectionName;

    friend class JsonDatabase;
};

#endif // JSONCOLLECTION_H
