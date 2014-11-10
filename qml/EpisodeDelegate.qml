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
import org.kde.plasma.components 2.0 as PlasmaComponents

ColumnLayout {
    Image {
        id: img
        source: cover

        fillMode: Image.PreserveAspectFit
    }

    PlasmaComponents.Label {
        text: display

        Layout.fillWidth: true
        elide: Text.ElideRight

        font.bold: true
    }

    PlasmaComponents.Label {
        text: "Episode " + episodeNumber
        opacity: 0.5

        Layout.fillWidth: true
        elide: Text.ElideRight
    }

    PlasmaComponents.Label {
        text: date.toDateString()
        opacity: 0.5

        Layout.fillWidth: true
        elide: Text.ElideRight
    }
}
