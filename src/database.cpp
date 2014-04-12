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

#include "database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database(const QString& path)
    : m_path(path)
    , m_sqlDb(0)
{
}

bool Database::init()
{
    qDebug() << "PATH:" << m_path;
    m_sqlDb = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_sqlDb->setDatabaseName(m_path + "/webdata.sqlite3");

    if (!m_sqlDb->open()) {
        qDebug() << "Failed to open db" << m_sqlDb->lastError().text();
        return false;
    }

    QSqlQuery query(*m_sqlDb);
    query.exec("CREATE TABLE IF NOT EXISTS movies("
               "fid INTEGER PRIMARY KEY, "
               "mid INTEGER, "
               "title TEXT, "
               "releaseDate TEXT, "
               "posterPath TEXT)");

    if (query.lastError().isValid()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

QSqlDatabase* Database::sqlDatabase()
{
    return m_sqlDb;
}
