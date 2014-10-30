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

#include "jsonquery.h"
#include "tools.h"

#include <QDebug>

JsonQuery::JsonQuery(EJQ* q, EJCOLL* coll)
    : m_ejq(q)
    , m_pos(-1)
{
    m_result = ejdbqryexecute(coll, q, &m_count, 0, 0);
}

JsonQuery::~JsonQuery()
{
    ejdbqresultdispose(m_result);
//    ejdbquerydel(m_ejq);
}

int JsonQuery::totalCount()
{
    return m_count;
}

bool JsonQuery::next()
{
    m_pos++;
    return static_cast<uint32_t>(m_pos) < m_count;
}

QVariantMap JsonQuery::result()
{
    int size;
    const void* data = ejdbqresultbsondata(m_result, m_pos, &size);
    if (!data) {
        return QVariantMap();
    }

    bson* rec = bson_create_from_buffer(data, size);
    QVariantMap map = bsonToMap(rec);
    bson_del(rec);

    return map;
}
