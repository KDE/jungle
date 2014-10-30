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

JsonCollection::JsonCollection()
    : m_db(0)
{
}

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

QString JsonCollection::insert(const QVariantMap& map)
{
    bson* rec = mapToBson(map);

    bson_oid_t oid;
    ejdbsavebson(m_coll, rec, &oid);

    char str[26];
    bson_oid_to_string(&oid, str);
    QByteArray id = QByteArray::fromRawData(str, 25);

    bson_destroy(rec);

    return QString::fromUtf8(id);
}

QVariantMap JsonCollection::fetch(const QString& id) const
{
    bson_oid_t oid;
    bson_oid_from_string(&oid, id.toUtf8().constData());

    bson* rec = ejdbloadbson(m_coll, &oid);
    if (!rec) {
        qDebug() << "Record not found" << id;
        return QVariantMap();
    }

    QVariantMap map = bsonToMap(rec);
    bson_del(rec);

    return map;
}

bool JsonCollection::remove(const QString& id)
{
    bson_oid_t oid;
    bson_oid_from_string(&oid, id.toUtf8().constData());

    return ejdbrmbson(m_coll, &oid);
}

JsonQuery JsonCollection::execute(const QVariantMap& map) const
{
    bson* rec = mapToBson(map);

    EJQ* q = ejdbcreatequery(m_db, rec, 0, 0, 0);
    JsonQuery query(q, m_coll);

    ejdbquerydel(q);

    bson_destroy(rec);

    return query;
}

int JsonCollection::count(const QVariantMap& map) const
{
    bson* rec = mapToBson(map);

    u_int32_t count;
    EJQ* q = ejdbcreatequery(m_db, rec, 0, 0, 0);
    ejdbqryexecute(m_coll, q, &count, JBQRYCOUNT, 0);

    ejdbquerydel(q);
    bson_del(rec);
    return count;
}

QVariantMap JsonCollection::findOne(const QVariantMap& query) const
{
    bson* rec = mapToBson(query);

    u_int32_t count;
    EJQ* q = ejdbcreatequery(m_db, rec, 0, 0, 0);
    EJQRESULT result = ejdbqryexecute(m_coll, q, &count, JBQRYFINDONE, 0);

    int size;
    const void* data = ejdbqresultbsondata(result, 0, &size);

    QVariantMap map;
    if (data) {
        bson* rec2 = bson_create_from_buffer(data, size);
        map = bsonToMap(rec2);
        bson_del(rec2);
    }

    ejdbquerydel(q);
    bson_del(rec);
    return map;

}

