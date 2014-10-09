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

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtMultimedia 5.0

import QtQuick.Controls 1.1 as QtControls
import QtQuick.Controls.Styles 1.1

import org.kde.kcoreaddons 1.0 as KCoreAddons
import org.kde.plasma.components 2.0 as PlasmaComponents

ColumnLayout {
    id: root

    property MediaPlayer source: MediaPlayer {}

    ProgressBar {
        id: progressBar

        Layout.fillWidth: true
        Layout.minimumHeight: 20
        Layout.maximumHeight: 20

        value: source.position
        maximum: source.duration

        backgroundShown: true

        onClicked: source.seek(source.duration * percent)
    }

    RowLayout {
        PlasmaComponents.ToolButton {
            id: button

            iconName: source.playbackState == MediaPlayer.PlayingState ? "media-playback-pause" : "media-playback-start"

            onClicked: {
                if (source.playbackState == MediaPlayer.PlayingState) {
                    source.pause()
                } else {
                    source.play()
                }
            }
        }

        Text {
            text: KCoreAddons.Format.formatDuration(source.position)
            color: "white"
        }

        Text {
            text: " / "
            color: "white"
        }

        Text {
            text: KCoreAddons.Format.formatDuration(source.duration)
            color: "white"
        }

        // Spacer
        Item {
            Layout.fillWidth: true
        }

        PlasmaComponents.ToolButton {
            iconName: "audio-volume-high"
            property double previousVolume

            onClicked: {
                if (source.volume) {
                    previousVolume = source.volume
                    source.volume = 0
                }
                else
                    source.volume = previousVolume
            }
        }

        ProgressBar {
            id: volumeBar

            Layout.minimumWidth: 300
            Layout.minimumHeight: parent.height / 4

            value: source.volume * 100
            maximum: 100
            backgroundShown: true

            onClicked: {
                source.volume = percent
            }
        }

        PlasmaComponents.ToolButton {
            iconName: "view-fullscreen"
            onClicked: applicationWindow.toggleFullScreen()
        }
    }
}
