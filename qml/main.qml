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
import QtQuick.Controls 1.1
import QtQuick.Window 2.1

import org.kde.plasma.components 2.0 as PlasmaComponents

ApplicationWindow {
    id: applicationWindow
    title: "Jungle"

    SystemPalette { id: sysPal }
    color: sysPal.base

    width: 1800
    height: 1600
    visible: true // the default is not true

    MainView {
        id: mainView
        visible: false

        onVideoSelected: {
            globalView.push(videoPlayer)
            videoPlayer.playUrl(url)
        }
    }

    VideoPlayer {
        id: videoPlayer
        visible: false

        Layout.fillWidth: true
        Layout.fillHeight: true

        Keys.onEscapePressed: {
            videoPlayer.stop()
            globalView.pop()
        }
    }

    FirstRun {
        id: firstRun
        visible: false
        progress: jungleProcessor.initialProgress

        Layout.fillWidth: true
        Layout.fillHeight: true

        onFinished: {
            jungleConfig.initialRun = false
            finishInitialization();
        }
    }

    StackView {
        id: globalView
        Component.onCompleted: {
            if (jungleConfig.initialRun) {
                push(firstRun)
            }
            else {
                finishInitialization();
            }
        }
    }

    MouseArea {
        // Otherwise it intefers with the ToolBox buttons
        z: -1000
        anchors.fill: parent
        propagateComposedEvents: true
        onDoubleClicked: toggleFullScreen()
    }

    function toggleFullScreen() {
        if (applicationWindow.visibility != Window.FullScreen) {
            applicationWindow.visibility = Window.FullScreen
        } else {
            applicationWindow.visibility = Window.Maximized
        }
    }

    function finishInitialization() {
        if (jungleArg) {
            globalView.push(mainView)
            globalView.push(videoPlayer)
            videoPlayer.playUrl(jungleArg)
        }
        else {
            globalView.push(mainView)
        }
    }
}
