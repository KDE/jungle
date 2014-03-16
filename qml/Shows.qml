import QtQuick 2.1
import QtQuick.Layouts 1.0

GridView {
    id: gridView
    signal play (string url)

    model: ListModel {
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
        ListElement {
            cover: "/home/vishesh/lost.jpg"
            title: "Lost"
            url: "/home/vishesh/The.Big.Bang.Theory.S07E16.HDTV.x264-LOL.mp4"
        }
    }

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
        }
    }

    delegate: del

    cellWidth: 400
    cellHeight: 600
}
