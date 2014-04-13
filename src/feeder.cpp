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

#include "feeder.h"
#include "moviefetchjob.h"

#include <QTimer>
#include <QDebug>

#include <baloo/query.h>
#include <baloo/resultiterator.h>

#include <QSqlQuery>
#include <QSqlError>

using namespace Jungle;

Feeder::Feeder(Database* db, QObject* parent)
    : QObject(parent)
    , m_db(db)
{
    QTimer::singleShot(0, this, SLOT(fetchFiles()));
}

Feeder::~Feeder()
{

}

void Feeder::fetchFiles()
{
    Baloo::Query query;
    query.setType("Video");

    auto it = query.exec();
    while (it.next()) {
        m_files << it.url().toLocalFile();
    }

    if (!m_files.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(processNext()));
    }
}

void Feeder::processNext()
{
    const QString url = m_files.takeLast();

    auto job = new MovieFetchJob(url, this);
    connect(job, &MovieFetchJob::result, this, &Feeder::slotResult);
}

void Feeder::slotResult(MovieFetchJob* job)
{
    if (job->id() == 0) {
        if (!m_files.isEmpty())
            QTimer::singleShot(0, this, SLOT(processNext()));
        return;
    }
    job->deleteLater();

    Movie movie;
    movie.setId(job->id());
    movie.setUrl(job->url());
    movie.setTitle(job->title());
    movie.setReleaseDate(job->releaseDate());
    movie.setPosterUrl(job->posterUrl());

    m_db->addMovie(movie);

    if (!m_files.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(processNext()));
    }
}
