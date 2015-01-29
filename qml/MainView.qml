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

ColumnLayout {
    signal videoSelected(string url)

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

        initialItem: movies
    }

    Component {
        id: movies
        Movies {
            onPlay: videoSelected(url)
        }
    }

    Component {
        id: tvshows
        TvShows {
            onShowSelected: {
                stackView.push({
                    item: tvEpisodes,
                    properties: { showId: showId }
                });
            }
        }
    }

    Component {
        id: tvEpisodes
        TvEpisodes {
            onPlay: videoSelected(url)
        }
    }

    Component {
        id: videos
        Videos {
            onPlay: videoSelected(url)
        }
    }
}
