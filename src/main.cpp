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
#include <QFileInfo>

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <KDBusService>
#include <KLocalizedString>

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

    QCommandLineParser parser;
    parser.addPositionalArgument(i18n("url"), i18n("The url to play"));
    parser.addHelpOption();
    parser.process(app);

    if (parser.positionalArguments().size() > 1) {
        parser.showHelp(1);
    }

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

    QTimer::singleShot(0, &processor, SLOT(resume()));

    JungleConfig config;

    //
    // QML
    //
    qDebug() << "Starting QML";
    QQmlEngine engine;
    QQmlContext* objectContext = engine.rootContext();
    objectContext->setContextProperty("jungleProcessor", &processor);
    objectContext->setContextProperty("jungleConfig", &config);

    QString arg;
    if (!parser.positionalArguments().isEmpty()) {
        arg = parser.positionalArguments().first();
    }
    objectContext->setContextProperty("jungleArg", arg);

    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, "main.qml");
    QQmlComponent component(&engine, path);
    QObject* object = component.create(objectContext);

    //
    // Another instance of Jungle is run
    //
    QObject::connect(&service, &KDBusService::activateRequested, [&](const QStringList& args, const QString&) {
        if (args.size() <= 1) {
            return;
        }

        QString url = QFileInfo(args[1]).absoluteFilePath();

        qDebug() << "We should play" << url;
        QMetaObject::invokeMethod(object, "play", QGenericReturnArgument(), Q_ARG(QVariant, url));
    });

    return app.exec();
}
