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

#include "jsondatabase.h"
#include "jsoncollection.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

JsonDatabase::JsonDatabase()
{
    m_jdb = ejdbnew();
}

JsonDatabase::~JsonDatabase()
{
    ejdbclose(m_jdb);
    ejdbdel(m_jdb);
}

QString JsonDatabase::filePath() const
{
    return m_filePath;
}

void JsonDatabase::setPath(const QString& filePath)
{
    m_filePath = filePath;
}

bool JsonDatabase::open()
{
    QByteArray path = QFile::encodeName(m_filePath);
    if (!ejdbopen(m_jdb, path.constData(), JBOWRITER | JBOCREAT)) {
        qDebug() << "Could not open db" << m_filePath;
        return false;
    }

    return true;
}

JsonCollection JsonDatabase::collection(const QString& name)
{
    return JsonCollection(m_jdb, name);
}

/*
static void addVariant(bson &rec, const QVariant& variant)
{
    switch (variant.type()) {
        case QVariant::Double:

            return BSON_DOUBLE;
        case QVariant::String:
            return BSON_STRING;
        case QVariant::Map:
            return BSON_OBJECT;
        case QVariant::List:
            return BSON_ARRAY;
        case QVariant::Bool:
            return BSON_BOOL;
        case QVariant::Date:
            return BSON_DATE;
        case QVariant::RegExp:
            return BSON_REGEX;
        case QVariant::Int:
            return BSON_INT;
        case QVariant::DateTime:
            return BSON_TIMESTAMP;
        case QVariant::LongLong:
            return BSON_LONG;
    }

    return BSON_EOO;
}
*/

