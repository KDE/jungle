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

#include "jsoncollection.h"
#include "jsonquery.h"
#include "tools.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

JsonCollection::JsonCollection(EJDB* db, const QString& name)
    : m_db(db)
    , m_collectionName(name)
{
    m_coll = ejdbcreatecoll(m_db, name.toUtf8().constData(), 0);
}

JsonCollection::~JsonCollection()
{
}

QString JsonCollection::collectionName() const
{
    return m_collectionName;
}

QByteArray JsonCollection::insert(const QVariantMap& map)
{
    bson* rec = mapToBson(map);

    bson_oid_t oid;
    ejdbsavebson(m_coll, rec, &oid);

    char str[26];
    bson_oid_to_string(&oid, str);
    QByteArray id(str);

    bson_destroy(rec);
    return id;
}

QVariantMap JsonCollection::fetch(const QByteArray& id)
{
    bson_oid_t oid;
    bson_oid_from_string(&oid, id.constData());

    bson* rec = ejdbloadbson(m_coll, &oid);
    if (!rec) {
        qDebug() << "Record not found" << id;
        return QVariantMap();
    }

    QVariantMap map = bsonToMap(rec);
    bson_del(rec);

    return map;
}

JsonQuery JsonCollection::execute(const QVariantMap& map)
{
    bson* rec = mapToBson(map);

    EJQ* q = ejdbcreatequery(m_db, rec, 0, 0, 0);
    JsonQuery query(q, m_coll);

    ejdbquerydel(q);

    bson_destroy(rec);

    return query;
}
