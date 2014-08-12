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

#include "databasetest.h"
#include "database.h"
#include "movie.h"

#include <QTest>
#include <QDebug>
#include <QSignalSpy>

#include <QSqlQuery>
#include <QSqlError>

#include <QTemporaryDir>

using namespace Jungle;

bool DatabaseTest::createFileMapDb(const QString& url)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "baloo_fileMap");
    db.setDatabaseName(url);

    if (!db.open()) {
        qDebug() << "Failed to open file db" << db.lastError();
        return false;
    }

    QSqlQuery query(db);
    bool ret = query.exec("CREATE TABLE IF NOT EXISTS files("
                          "id INTEGER PRIMARY KEY, "
                          "url TEXT NOT NULL UNIQUE)");

    if (!ret) {
        qDebug() << "Failed to create file db" << db.lastError();
        return false;
    }

    return true;
}

int DatabaseTest::createFile(const QString& url)
{
    QSqlDatabase db = QSqlDatabase::database("baloo_fileMap");

    QSqlQuery query(db);
    query.prepare("insert into files (url) VALUES (?)");
    query.addBindValue(url);

    if (!query.exec()) {
        qWarning() << query.lastError().text();
        return 0;
    }

    return query.lastInsertId().toUInt();
}

void DatabaseTest::testMovies()
{
    qRegisterMetaType<Movie>("Movie");

    QTemporaryDir dir;

    const QString fileMap = dir.path() + "/fileMap.sqlite3";
    QVERIFY(createFileMapDb(fileMap));

    Database db(dir.path(), fileMap);
    QVERIFY(db.init());

    const QString posterUrl = dir.path() + "/poster";
    const QString fileUrl = dir.path() + "/movieurl";
    int fileId = createFile(fileUrl);
    QVERIFY(fileId > 0);

    Movie movie;
    movie.setId(500);
    movie.setPosterUrl(posterUrl);
    movie.setReleaseDate(QDate(2000, 12, 2));
    movie.setTitle("Movie King");
    movie.setUrl(fileUrl);

    QSignalSpy spy(&db, SIGNAL(movieAdded(Movie)));
    db.addMovie(movie);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.value(0).size(), 1);
    QCOMPARE(spy.value(0).first().value<Movie>(), movie);

    // Check if the Movie has been properly inserted
    QSqlQuery query;
    query.prepare("select * from movies where fid = ?");
    query.addBindValue(fileId);
    QVERIFY(query.exec());
    QVERIFY(query.next());

    QCOMPARE(movie.posterUrl(), query.value("posterPath").toString());
    QCOMPARE(movie.id(), query.value("mid").toInt());
    QCOMPARE(movie.title(), query.value("title").toString());
    QCOMPARE(movie.releaseDate(), query.value("releaseDate").toDate());
    QCOMPARE(movie.title(), query.value("title").toString());

    QVERIFY(!query.next());

    //
    // Insert another movie
    //
    const QString posterUrl2 = dir.path() + "/poster2";
    const QString fileUrl2 = dir.path() + "/movieurl2";
    int fileId2 = createFile(fileUrl2);
    QVERIFY(fileId2 > 0);

    Movie movie2;
    movie2.setId(600);
    movie2.setPosterUrl(posterUrl2);
    movie2.setReleaseDate(QDate(2003, 12, 2));
    movie2.setTitle("Movie King 2");
    movie2.setUrl(fileUrl2);
    db.addMovie(movie2);

    // All Movies
    QList<Movie> movies = db.allMovies();
    QCOMPARE(movies.size(), 2);
    QCOMPARE(movies[0], movie);
    QCOMPARE(movies[1], movie2);
}

QTEST_MAIN(DatabaseTest)
