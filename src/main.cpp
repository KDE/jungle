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

#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include <QIcon>
#include <QStandardPaths>
#include <QDebug>

#include "processor.h"
#include "baloovideosfetcher.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Jungle");
    app.setWindowIcon(QIcon::fromTheme("nepomuk"));

    Jungle::Processor processor;

    Jungle::BalooVideosFetcher videoFetcher;
    QStringList videoList = videoFetcher.allVideos();

    for (const QString& filePath : videoList) {
        qDebug() << filePath;
        processor.addFile(filePath);
    }

    qDebug() << "Starting QML";
    QQmlEngine engine;
    QQmlContext* objectContext = engine.rootContext();

    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, "main.qml");
    QQmlComponent component(&engine, path);
    component.create(objectContext);

    return app.exec();
}
