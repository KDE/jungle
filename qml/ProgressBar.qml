import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

Item {
    id: progressBar

    property int minimum: 0
    property int maximum: 100
    property int value: 30
    property int radius: 0
    property string color: "white"

    Rectangle {
        id: border
        color: "transparent"
        radius: parent.radius

        anchors.fill: parent

        border.width: 2
        border.color: parent.color
    }

    Rectangle {
        id: main

        property int finalWidth: progressBar.width * value / maximum
        width: finalWidth

        color: parent.color
        height: parent.height
        radius: parent.radius
    }

    RectangularGlow {
        anchors.fill: main

        glowRadius: 7
        spread: 0.1
        color: parent.color
        z: -1
    }
}
