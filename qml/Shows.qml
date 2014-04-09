import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import org.kde.jungle 0.1 as Jungle

GridView {
    id: gridView
    signal play (string url)

    model: Jungle.VideosModel {}

    Component {
        id: del
        Item {
            width: cellWidth
            height: cellHeight

            Image {
                id: img
                source: cover

                width: cellWidth - (anchors.leftMargin + anchors.rightMargin)
                fillMode: Image.PreserveAspectFit

                anchors {
                    top: parent.top
                    leftMargin: 30
                    rightMargin: 30
                }
            }

            Text {
                text: title
                color: "white"
                width: img.width
                horizontalAlignment: Text.AlignHCenter

                font.pointSize: 22
                anchors {
                    top: img.bottom
                    bottom: parent.bottom
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    gridView.play(url)
                }
            }

            RectangularGlow {
                anchors.fill: img
                glowRadius: 7
                spread: 0.1
                color: "white"
                z: -1
            }
        }
    }

    delegate: del

    cellWidth: 400
    cellHeight: 600
}
