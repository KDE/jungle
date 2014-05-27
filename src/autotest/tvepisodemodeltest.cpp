/*
 * Copyright (C) 2014  Alejandro Fiestas Olivares <afiestas@kde.org>
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

#include "tvepisodemodel.h"

#include <QObject>
#include <QTest>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>

#include <QDebug>

using namespace Jungle;
class TvEpisodeModelTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testGettingEpisodes();

private:
    void createMockDatabase(const QString path, const QString &name, const QString &mockData);
};

void TvEpisodeModelTest::createMockDatabase(const QString path, const QString& name, const QString& mockData)
{
    const QString filePath(path + name);
    QDir().mkpath(path);
    QFile::remove(filePath);

    QProcess createSqlite;
    createSqlite.start("sh", QStringList() << "-c" <<
                                             QStringLiteral("sqlite3 ")
                                             + filePath
                                             + QStringLiteral(" < ")
                                             + mockData);
    createSqlite.waitForFinished();
}

void TvEpisodeModelTest::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    const QString location = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    createMockDatabase(location + QStringLiteral("/baloo/file/"),
                        QStringLiteral("fileMap.sqlite3"),
                       QStringLiteral(FILE_MAP_MOCK_DATA)
    );


    createMockDatabase(location + QStringLiteral("/jungle/"),
                       QStringLiteral("webdata.sqlite3"),
                       QStringLiteral(WEB_MOCK_DATA)
    );
}

void TvEpisodeModelTest::testGettingEpisodes()
{
    auto model = new TvEpisodeModel(this);
    model->setShowId(1399); // 1399 Game of thrones

    QCOMPARE(model->rowCount(), 5);
    QList<QPair<QString, QDate> > episodes;
    episodes << QPair<QString, QDate>(QStringLiteral("The Lion and the Rose"), QDate::fromString(QStringLiteral("2014-04-13"), Qt::ISODate))
    << QPair<QString, QDate>(QStringLiteral("First of His Name"), QDate::fromString(QStringLiteral("2014-05-04"), Qt::ISODate))
    << QPair<QString, QDate>(QStringLiteral("The Mountain and the Viper"), QDate::fromString(QStringLiteral("2014-06-01"), Qt::ISODate))
    << QPair<QString, QDate>(QStringLiteral("The Watchers on the Wall"), QDate::fromString(QStringLiteral("2014-06-08"), Qt::ISODate))
    << QPair<QString, QDate>(QStringLiteral("The Children"), QDate::fromString(QStringLiteral("2014-06-15"), Qt::ISODate));

    for (int x = 0; x < model->rowCount(); ++x) {
        auto index = model->index(x);
        QCOMPARE(model->data(index, TvEpisodeModel::NameRole).toString(), episodes.at(x).first);
        QCOMPARE(model->data(index, TvEpisodeModel::AirDateRole).toDate(), episodes.at(x).second);
    }

}

QTEST_MAIN(TvEpisodeModelTest);

#include "tvepisodemodeltest.moc"