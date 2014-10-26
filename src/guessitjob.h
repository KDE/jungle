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

#ifndef GUESSITJOB_H
#define GUESSITJOB_H

#include "job.h"
#include <QProcess>

namespace Jungle {

class GuessItJob : public Job
{
    Q_OBJECT
public:
    GuessItJob(const QString& filePath);

    QVariantMap data() const;

private slots:
    void slotProcessFinished(int exitCode);

private:
    QString m_filePath;
    QVariantMap m_data;
    QProcess* m_process;
};

}
#endif // GUESSITJOB_H
