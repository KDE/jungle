/*
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "qmlplugins.h"
#include "moviesmodel.h"
#include "tvshowsmodel.h"
#include "tvepisodemodel.h"
#include "videosmodel.h"
#include "sortmodel.h"
#include "cursor.h"

#include <QtQml/qqml.h>

static QObject* cursor_singleton(QQmlEngine*, QJSEngine*)
{
    return new Jungle::Cursor();
}

void QmlPlugins::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.jungle"));

    qmlRegisterType<Jungle::MoviesModel> (uri, 0, 1, "MoviesModel");
    qmlRegisterType<Jungle::TvShowsModel> (uri, 0, 1, "TvShowsModel");
    qmlRegisterType<Jungle::TvEpisodeModel> (uri, 0, 1, "TvEpisodeModel");
    qmlRegisterType<Jungle::VideosModel> (uri, 0, 1, "VideosModel");
    qmlRegisterType<Jungle::SortModel> (uri, 0, 1, "SortModel");
    qmlRegisterSingletonType<Jungle::Cursor>(uri, 0, 1, "Cursor", cursor_singleton);
}

