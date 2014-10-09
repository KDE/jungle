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

#include "database.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

using namespace Jungle;

Database::Database(const QString& dbPath)
{
    m_jdb = ejdbnew();
    if (!ejdbopen(m_jdb, QFile::encodeName(dbPath).constData(), JBOWRITER | JBOCREAT)) {
        qDebug() << "Could not open db" << dbPath;
    }
    m_coll = ejdbcreatecoll(m_jdb, "default", 0);
}

Database::~Database()
{
    ejdbclose(m_jdb);
    ejdbdel(m_jdb);
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

QByteArray Database::add(const QVariantMap& map)
{
    bson rec;
    bson_init(&rec);

    QMapIterator<QString, QVariant> it(map);
    while (it.hasNext()) {
        it.next();

        const QByteArray key = it.key().toUtf8();
        const QVariant &var = it.value();

        switch (var.type()) {
            case QVariant::Double: {
                bson_append_double(&rec, key.constData(), var.toDouble());
                break;
            }

            case QVariant::String: {
                QByteArray val = var.toString().toUtf8();
                bson_append_string(&rec, key.constData(), val.constData());
                break;
            }

            case QVariant::Map: {
                Q_ASSERT(0);
            }

            case QVariant::List: {
                Q_ASSERT(0);
            }

            case QVariant::Bool: {
                bson_append_bool(&rec, key.constData(), var.toBool());
                break;
            }

            case QVariant::Date: {
                bson_date_t date = var.toDateTime().toTime_t();
                bson_append_date(&rec, key.constData(), date);
                break;
            }

            case QVariant::RegExp: {
                Q_ASSERT(0);
            }

            case QVariant::Int: {
                bson_append_int(&rec, key.constData(), var.toInt());
                break;
            }

            case QVariant::DateTime: {
                bson_date_t date = var.toDateTime().toTime_t();
                bson_append_time_t(&rec, key.constData(), date);
                break;
            }

            case QVariant::LongLong: {
                // FIXME: Data is being lost?
                bson_append_long(&rec, key.constData(), var.toLongLong());
                break;
            }

            default: {
                Q_ASSERT(0);
            }
        }
    }

    bson_finish(&rec);

    bson_oid_t oid;
    ejdbsavebson(m_coll, &rec, &oid);

    char str[26];
    bson_oid_to_string(&oid, str);
    QByteArray id(str);

    bson_destroy(&rec);

    return id;
}

QVariantMap Database::fetch(const QByteArray& id)
{
    bson_oid_t oid;
    bson_oid_from_string(&oid, id.constData());

    bson* rec = ejdbloadbson(m_coll, &oid);
    if (!rec) {
        qDebug() << "Record not found" << id;
        return QVariantMap();
    }

    char* buf;
    int length;

    bson2json(bson_data(rec), &buf, &length);

    QByteArray arr = QByteArray::fromRawData(buf, length);
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    QVariantMap map = doc.object().toVariantMap();

    free(buf);
    bson_del(rec);

    return map;
}
