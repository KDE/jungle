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

#ifndef JUNGLE_FILESYSTEMTRACKER_H
#define JUNGLE_FILESYSTEMTRACKER_H

#include <QObject>
#include <QSet>

#include <KVariantStore/KVariantStore>
#include <KVariantStore/KVariantCollection>

namespace Jungle {

class FileSystemTracker : public QObject
{
    Q_OBJECT
public:
    explicit FileSystemTracker(QObject* parent = 0);
    virtual ~FileSystemTracker();

signals:
    void videoAdded(const QString& filePath);
    void videoRemoved(const QString& filePath);

private slots:
    void init();
    void slotVideoResult(const QString& filePath);
    void slotFetchFinished();

private:
    KVariantStore* m_db;
    KVariantCollection m_coll;

    QSet<QString> m_filePaths;
};

}

#endif // JUNGLE_FILESYSTEMTRACKER_H
