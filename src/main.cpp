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

#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include <QIcon>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>

#include <KDBusService>

#include "processor.h"
#include "filesystemtracker.h"
#include "jungleconfig.h"

using namespace Jungle;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Jungle");
    app.setWindowIcon(QIcon::fromTheme("nepomuk"));

    KDBusService service(KDBusService::Unique);

    Processor processor;
    FileSystemTracker fsTracker;

    QObject::connect(&fsTracker, &FileSystemTracker::videoAdded,
            [&](const QString& filePath) {
        qDebug() << filePath;
        processor.addFile(filePath);
    });

    QObject::connect(&fsTracker, &FileSystemTracker::videoRemoved,
            [&](const QString& filePath) {
        qDebug() << "FILE DELETED" << filePath;
        processor.removeFile(filePath);
    });

    JungleConfig config;

    qDebug() << "Starting QML";
    QQmlEngine engine;
    QQmlContext* objectContext = engine.rootContext();
    objectContext->setContextProperty("jungleProcessor", &processor);
    objectContext->setContextProperty("jungleConfig", &config);

    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, "main.qml");
    QQmlComponent component(&engine, path);
    component.create(objectContext);

    QTimer::singleShot(0, &processor, SLOT(resume()));

    return app.exec();
}
