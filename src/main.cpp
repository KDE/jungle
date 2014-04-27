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

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include <QStandardPaths>

#include "database.h"
#include "feeder.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    app.setApplicationDisplayName("Jungle");

    Jungle::Database* db = Jungle::Database::instance();
    if (!db->initialized()) {
        return 1;
    }

    Jungle::Feeder feeder(db);

    QQmlEngine engine;
    QScopedPointer<QQmlContext> objectContext(new QQmlContext(engine.rootContext()));

    QString path = QStandardPaths::locate(QStandardPaths::DataLocation, "main.qml");
    QQmlComponent component(&engine, path);
    QScopedPointer<QObject> object(component.create(objectContext.data()));

    return app.exec();
}
