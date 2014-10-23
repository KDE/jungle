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
#include <QMapIterator>
#include <QDateTime>
#include <QDebug>

// The bson must be destroyed on your own
inline bson* mapToBson(const QVariantMap& map)
{
    /*
    QJsonDocument doc = QJsonDocument::fromVariant(map);
    bson* bs = json2bson(doc.toJson().constData());

    if (map.contains("_id")) {
        bson* bs2 = bson_dup(bs);
        bs2->finished = false;
        bson_del(bs);

        QByteArray id = map.value("_id").toString().toUtf8();
        bson_oid_t oid;
        bson_oid_from_string(&oid, id.constData());
        bson_append_oid(bs2, "_id", &oid);

        bson_finish(bs2);
        return bs2;
    }

    return bs;
    */
    bson* rec = new bson();
    bson_init(rec);

    QMapIterator<QString, QVariant> it(map);
    while (it.hasNext()) {
        it.next();

        const QByteArray key = it.key().toUtf8();
        const QVariant &var = it.value();

        switch (var.type()) {
            case QVariant::Double: {
                bson_append_double(rec, key.constData(), var.toDouble());
                break;
            }

            case QVariant::Url:
            case QVariant::String: {
                QByteArray val = var.toString().toUtf8();
                if (key == "_id") {
                    bson_oid_t oid;
                    bson_oid_from_string(&oid, val.constData());
                    bson_append_oid(rec, key.constData(), &oid);
                } else {
                    bson_append_string(rec, key.constData(), val.constData());
                }
                break;
            }

            case QVariant::Map: {
                Q_ASSERT(0);
            }

            case QVariant::StringList:
            case QVariant::List: {
                QList<QString> list = var.toStringList();
                bson_append_start_array(rec, key.constData());

                char buf[5];
                int c = 0;
                for (const QString& str : list) {
                    QByteArray val = str.toUtf8();
                    bson_numstrn(buf, 5, c++);
                    bson_append_string(rec, buf, val.constData());
                }
                bson_append_finish_array(rec);
                break;
            }

            case QVariant::Bool: {
                bson_append_bool(rec, key.constData(), var.toBool());
                break;
            }

            case QVariant::Date: {
                bson_date_t date = var.toDateTime().toTime_t();
                bson_append_date(rec, key.constData(), date);
                break;
            }

            case QVariant::RegExp: {
                Q_ASSERT(0);
            }

            case QVariant::Int: {
                bson_append_int(rec, key.constData(), var.toInt());
                break;
            }

            case QVariant::DateTime: {
                bson_date_t date = var.toDateTime().toTime_t();
                bson_append_time_t(rec, key.constData(), date);
                break;
            }

            case QVariant::LongLong: {
                // FIXME: Data is being lost?
                bson_append_long(rec, key.constData(), var.toLongLong());
                break;
            }

            default: {
                Q_ASSERT(0);
            }
        }
    }

    bson_finish(rec);

    return rec;
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
