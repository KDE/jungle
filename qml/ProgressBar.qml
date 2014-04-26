import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

Item {
    id: progressBar

    property int minimum: 0
    property int maximum: 100
    property int value: 30
    property int radius: 0

    property string color: "#2874CC"

    property string backgroundColor: "black"
    property bool backgroundShown: false

    property string borderColor: "black"
    property bool borderShown: false

    signal clicked (double percent)

    Rectangle {
        id: border
        color: "transparent"
        radius: parent.radius

        anchors.fill: parent

        border.width: 2
        border.color: borderColor
        opacity: borderShown ? 1.0 : 0.0
    }

    Rectangle {
        id: main

        property int finalWidth: progressBar.width * value / maximum
        width: finalWidth

        color: parent.color
        height: parent.height
        radius: parent.radius
    }

    Rectangle {
        anchors.fill: parent
        color: backgroundColor

        radius: parent.radius
        opacity: backgroundShown ? 0.5 : 0.0
        z: -1
    }

    RectangularGlow {
        anchors.fill: main

        glowRadius: 4
        spread: 0.1
        color: parent.color
        z: -1
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            var percent = mouse.x / parent.width
            progressBar.clicked(percent)
        }
    }
}
