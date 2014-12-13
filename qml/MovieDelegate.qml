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

import QtQuick 2.3
import QtQuick.Layouts 1.0
import org.kde.plasma.components 2.0 as PlasmaComponents

ColumnLayout {
    id: delegate
    property alias subtext: sub.text
    signal clicked

    Image {
        id: img
        source: cover
        fillMode: Image.PreserveAspectFit
        asynchronous: true
        mipmap: true

        Layout.fillWidth: true
        Layout.fillHeight: true

        width: gridView.cellActualWidth
        visible: (img.status == Image.Ready)
    }

    Rectangle {
        width: gridView.cellActualWidth
        height: width * 1.5
        color: "black"
        visible: (img.status != Image.Ready)
        Layout.alignment: Qt.AlignTop
    }

    PlasmaComponents.Label {
        text: display
        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
        Layout.maximumWidth: gridView.cellActualWidth

        font.bold: true
        elide: Text.ElideRight
        clip: true
    }

    PlasmaComponents.Label {
        id: sub
        Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
        Layout.maximumWidth: gridView.cellActualWidth

        opacity: 0.5
        elide: Text.ElideRight
    }

    MouseArea {
        anchors.fill: parent
        onClicked: delegate.clicked()
    }
}
