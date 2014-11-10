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

    StackView {
        id: globalView
        initialItem: ColumnLayout {
            Sidebar {
                z: 1000
                Layout.fillWidth: true
                onMoviesActivated: {
                    stackView.pop(movies)
                }

                onTvShowsActivated: {
                    stackView.push(tvshows);
                }

                onVideosActivated: {
                    stackView.push(videos);
                }
            }

            StackView {
                id: stackView
                Layout.fillWidth: true
                Layout.fillHeight: true

                delegate: StackViewDelegate {
                    pushTransition: StackViewTransition {
                        PropertyAnimation {
                            target: enterItem
                            property: "y"
                            from: exitItem.height
                            to: 0
                        }
                    }
                    popTransition: StackViewTransition {
                        PropertyAnimation {
                            target: exitItem
                            property: "y"
                            from: 0
                            to: enterItem.height
                        }
                    }
                }

                initialItem: Movies {
                    id: movies

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    focus: true
                    onPlay: {
                        globalView.push(videoPlayer)
                        videoPlayer.playUrl(url)
                    }
                }
            }

            TvShows {
                id: tvshows
                visible: false

                onShowSelected: {
                    tvEpisodes.showId = showId
                    stackView.push(tvEpisodes)
                }
            }

            TvEpisodes {
                id: tvEpisodes
                visible: false

                onPlay: {
                    globalView.push(videoPlayer)
                    videoPlayer.playUrl(url)
                }
            }

            Videos {
                id: videos
                visible: false

                onPlay: {
                    globalView.push(videoPlayer)
                    videoPlayer.playUrl(url)
                }
            }
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
}
