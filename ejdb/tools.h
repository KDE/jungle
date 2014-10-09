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

#include <tcejdb/ejdb.h>

#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>

// The bson must be destroyed on your own
inline bson* mapToBson(const QVariantMap& map)
{
    QJsonDocument doc = QJsonDocument::fromVariant(map);
    QByteArray arr = doc.toJson();

    return json2bson(arr.constData());
}

inline QVariantMap bsonToMap(bson* rec)
{
    char* buf;
    int length;

    bson2json(bson_data(rec), &buf, &length);

    QByteArray arr = QByteArray::fromRawData(buf, length);
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    QVariantMap map = doc.object().toVariantMap();

    free(buf);
    return map;
}
