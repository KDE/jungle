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

AutomaticSpacingGrid {
    id: gridView
    signal play (string url)
    property alias showId: epModel.showId

    model: Jungle.SortModel {
        sourceModel: Jungle.TvEpisodeModel {
            id: epModel
        }
        sortRoleName: "episodeNumber"
    }

    delegate: Item {
        width: cellWidth
        height: cellHeight

        EpisodeDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            MouseArea {
                anchors.fill: parent
                onClicked: topElem.play(url)
            }
            Keys.onReturnPressed: gridView.play(url)
            Keys.onSpacePressed: gridView.play(url)
        }
    }

    cellActualWidth: 400
    cellActualHeight: 350

    minRowSpacing: 5
    boundsBehavior: Flickable.StopAtBounds
}
