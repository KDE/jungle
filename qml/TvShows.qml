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
 */

import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.1 as QtControls

import org.kde.jungle 0.1 as Jungle

FocusScope {
    id: topElem
    signal showSelected (string showId)

    GridView {
        id: tvshows
        model: Jungle.TvShowsModel {}
        delegate: MovieDelegate {
            MouseArea {
                anchors.fill: parent
                onClicked: showSelected(model.showId)
            }
            Keys.onReturnPressed: showSelected(model.showId)
            Keys.onSpacePressed: showSelected(model.showId)
        }

        cellWidth: 400
        cellHeight: 600

        focus: true
        anchors.fill: parent
        highlightMoveDuration: 0
        highlight: Rectangle {
            color: "#2874CC"
        }
    }
}
