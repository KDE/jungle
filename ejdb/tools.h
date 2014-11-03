/*
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
#include <QMapIterator>
#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>

// The object should be destroyed with bson_del
inline bson* mapToBson(const QVariantMap& map, bool query = false)
{
    bson* rec = new bson();
    if (query) {
        bson_init_as_query(rec);
    } else {
        bson_init(rec);
    }

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
                bson* obj = mapToBson(var.toMap());
                bson_append_bson(rec, key.constData(), obj);
                bson_del(obj);
                break;
            }

            case QVariant::StringList:
            case QVariant::List: {
                QList<QVariant> list = var.toList();
                bson_append_start_array(rec, key.constData());

                char buf[5];
                int c = 0;
                for (const QVariant& var : list) {
                    bson_numstrn(buf, 5, c++);
                    if (var.type() == QVariant::Int) {
                        bson_append_int(rec, buf, var.toInt());
                    } else {
                        QByteArray val = var.toString().toUtf8();
                        bson_append_string(rec, buf, val.constData());
                    }
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

            case QVariant::RegularExpression: {
                QRegularExpression regex = var.toRegularExpression();
                QByteArray patern = regex.pattern().toUtf8();
                QByteArray options;
                if (regex.patternOptions() & QRegularExpression::CaseInsensitiveOption)
                    options += "i";

                bson_append_regex(rec, key.constData(), patern.constData(), options.constData());
                break;
            }

            case QVariant::RegExp: {
                QRegExp regex = var.toRegExp();
                QByteArray patern = regex.pattern().toUtf8();
                QByteArray options;
                if (regex.caseSensitivity() == Qt::CaseInsensitive)
                    options += "i";

                bson_append_regex(rec, key.constData(), patern.constData(), options.constData());
                break;
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

            case QVariant::Invalid: {
                bson_append_null(rec, key.constData());
                break;
            }

            default: {
                qDebug() << var.typeName();
                Q_ASSERT(0);
            }
        }
    }

    bson_finish(rec);

    return rec;
}

inline QVariantMap bsonToMap(bson* rec)
{
    bson_iterator* it = bson_iterator_create();
    bson_iterator_init(it, rec);

    QVariantMap map;

    while (bson_iterator_more(it)) {
        bson_type type = bson_iterator_next(it);

        const QByteArray arr(bson_iterator_key(it));
        const QString key = QString::fromUtf8(arr);

        switch (type) {
            case BSON_DOUBLE: {
                double d = bson_iterator_double(it);
                map.insert(key, d);
                break;
            }

            case BSON_STRING: {
                const char* str = bson_iterator_string(it);
                int len = bson_iterator_string_len(it);

                QByteArray arr = QByteArray::fromRawData(str, len);
                QString val = QString::fromUtf8(arr);
                map.insert(key, val);
                break;
            }
            case BSON_OBJECT: {
                bson subObj;
                bson_iterator_subobject(it, &subObj);

                QVariantMap val = bsonToMap(&subObj);
                map.insert(key, val);
                break;
            }

            case BSON_ARRAY: {
                bson subObj;
                bson_iterator_subobject(it, &subObj);

                const QVariantMap arrayMap = bsonToMap(&subObj);
                QVariantList list;
                for (auto it = arrayMap.begin(); it != arrayMap.end(); it++) {
                    list << it.value();
                }

                map.insert(key, list);
                break;
            }

            case BSON_BINDATA:
                Q_ASSERT(0);
                break;

            case BSON_UNDEFINED:
                Q_ASSERT(0);
                break;

            case BSON_OID: {
                bson_oid_t* oid = bson_iterator_oid(it);
                char oid_s[50];
                bson_oid_to_string(oid, oid_s);

                QByteArray arr = QByteArray::fromRawData(oid_s, strlen(oid_s));
                QString val = QString::fromUtf8(arr);
                map.insert(key, val);
                break;
            }
            case BSON_BOOL: {
                bool val = bson_iterator_bool(it);
                map.insert(key, val);
                break;
            }
            case BSON_DATE: {
                bson_date_t dt = bson_iterator_date(it);
                QDate val = QDateTime::fromTime_t(dt).date();
                map.insert(key, val);
                break;
            }
            case BSON_NULL:
                Q_ASSERT(0);
            case BSON_REGEX: {
                const char* str = bson_iterator_regex(it);
                QByteArray arr = QByteArray::fromRawData(str, strlen(str));

                QRegularExpression val(QString::fromUtf8(arr));
                map.insert(key, val);
                // FIXME: What about the options?
                break;
            }
            case BSON_DBREF:
                Q_ASSERT(0);
            case BSON_CODE:
                Q_ASSERT(0);
            case BSON_SYMBOL:
                Q_ASSERT(0);
            case BSON_CODEWSCOPE:
                Q_ASSERT(0);
            case BSON_INT: {
                int val = bson_iterator_int(it);
                map.insert(key, val);
                break;
            }
            case BSON_TIMESTAMP: {
                Q_ASSERT(0);
                //bson_timestamp_t t = bson_iterator_timestamp(it);
            }
            case BSON_LONG: {
                long long val = bson_iterator_long(it);
                map.insert(key, val);
                break;
            }

            case BSON_EOO:
                // vHanda: I have no idea why we get this!
                continue;

            default:
                Q_ASSERT(0);
        }
    }

    bson_iterator_dispose(it);
    return map;
}
