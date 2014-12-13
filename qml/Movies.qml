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

import org.kde.jungle 0.1 as Jungle
import org.kde.plasma.components 2.0 as PlasmaComponents

AutomaticSpacingGrid {
    id: gridView
    signal play (string url)

    model: Jungle.SortModel {
        sourceModel: Jungle.MoviesModel {}
    }

    delegate: Item {
        width: cellWidth
        height: cellHeight

        MovieDelegate {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top

            subtext: model.year
            onClicked: gridView.play(url)

            Keys.onReturnPressed: gridView.play(url)
            Keys.onSpacePressed: gridView.play(url)
        }
    }

    // 342 x 512 is generally the size of the movie cover
    cellActualWidth: 342
    cellActualHeight: 600

    minRowSpacing: 5
}
