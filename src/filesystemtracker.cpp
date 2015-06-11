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

#include "filesystemtracker.h"
#include "baloovideosfetcher.h"

#include <KDocumentStore/KDocumentQuery>

#include <QStandardPaths>
#include <QTimer>
#include <QVariantMap>
#include <QDir>

using namespace Jungle;

FileSystemTracker::FileSystemTracker(QObject* parent)
    : QObject(parent)
{
    static QString jungleDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/jungle/";
    QDir().mkpath(jungleDir);

    m_db = new KDocumentStore();
    m_db->setPath(jungleDir + QStringLiteral("fstracker"));
    if (!m_db->open()) {
        Q_ASSERT_X(0, "", "FileSystemTracker could not open database");
    }
    m_coll = m_db->collection("videos");

    QTimer::singleShot(0, this, SLOT(init()));
}

FileSystemTracker::~FileSystemTracker()
{
}

void FileSystemTracker::init()
{
    BalooVideosFetcher* videoFetcher = new BalooVideosFetcher();
    connect(videoFetcher, &BalooVideosFetcher::videoResult,
            this, &FileSystemTracker::slotVideoResult);
    connect(videoFetcher, &BalooVideosFetcher::finished,
            this, &FileSystemTracker::slotFetchFinished);

    videoFetcher->fetch();
}

void FileSystemTracker::slotVideoResult(const QString& filePath)
{
    QVariantMap map = {{"url", filePath}};
    if (m_coll.count(map) == 0) {
        m_coll.insert(map);
        emit videoAdded(filePath);
    }

    m_filePaths << filePath;
}

void FileSystemTracker::slotFetchFinished()
{
    KDocumentQuery q = m_coll.find(QVariantMap());
    while (q.next()) {
        QVariantMap map = q.result();
        QString filePath = map.value("url").toString();

        if (!m_filePaths.contains(filePath)) {
            emit videoRemoved(filePath);
        }
    }

    m_filePaths.clear();
}
