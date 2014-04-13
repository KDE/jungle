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

#ifndef SHOW_H
#define SHOW_H

#include <QString>
#include <QDate>

namespace Jungle {

class Show
{
public:
    Show();
    
    int id() const;
    void setId(int id);

    QString title() const;
    void setTitle(const QString& name);

    QString coverUrl() const;
    void setCoverUrl(const QString& url);

    QDate firstAired() const;
    void setFirstAired(const QDate& date);

    int numSeasons() const;
    void setNumSeasons(int num);

private:
    int m_id;
    QString m_title;
    QString m_coverUrl;
    QDate m_firstAired;
    int m_numSeasons;
};
}

#endif // SHOW_H
