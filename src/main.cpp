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
#include <QStandardPaths>
#include <QDir>

#include "database.h"
#include "feeder.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle";
    QDir().mkpath(dataDir);

    Jungle::Database db(dataDir);
    if (!db.init()) {
        return 1;
    }

    Jungle::Feeder feeder(&db);
    return app.exec();
}
