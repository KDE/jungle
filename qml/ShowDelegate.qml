
import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle

ColumnLayout {
    Image {
        id: img
        source: cover

        width: cellWidth
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: title
        text: display
        color: "white"
        width: img.width
        horizontalAlignment: Text.AlignLeft

        elide: Text.ElideRight

        font.pointSize: 8
        font.bold: true
        anchors {
            topMargin: 10
        }
    }

    Text {
        id: year
        text: "2014"
        color: "white"
        width: img.width
        opacity: 0.5

        font.pointSize: 8

        anchors {
            topMargin: 5
        }
    }
}
