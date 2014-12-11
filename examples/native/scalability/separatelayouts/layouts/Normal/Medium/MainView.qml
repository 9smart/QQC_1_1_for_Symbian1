/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import "../../../" as Root

Rectangle {
    id: rect

    property int buttonWidth: rect.width / 3
    property int buttonHeight: 150
    property string sourceInfo: "layouts/Normal/Medium/MainView.qml,"
    property string displayInfo: Math.max(rect.width, rect.height) + "x"
                                 + Math.min(rect.width, rect.height) + ", "
                                 + Math.round(screen.dpi) + " DPI"

    anchors.fill: parent
    color: "black"

    Root.GradientRectangle {
        id: output

        color1: "white"
        color2: "blue"
        width: rect.width
        height: rect.height - buttonHeight
        anchors { top: rect.top; bottom: button1.top }

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            text: rect.sourceInfo + "\n" + rect.displayInfo
        }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = "blue" }
    }

    Root.GradientRectangle {
        id: button1

        color1: "lightgreen"
        color2: "green"
        width: rect.buttonWidth
        height: rect.buttonHeight
        anchors { bottom: rect.bottom; left: rect.left }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    Root.GradientRectangle {
        id: button2

        color1: "white"
        color2: "yellow"

        width: rect.buttonWidth
        height: rect.buttonHeight
        anchors { bottom: rect.bottom; horizontalCenter: rect.horizontalCenter }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    Root.GradientRectangle {
        id: button3

        color1: "white"
        color2: "red"
        width: rect.buttonWidth
        height: rect.buttonHeight
        anchors { bottom: rect.bottom; right: rect.right }

        MouseArea { anchors.fill: parent; onClicked: output.color2 = parent.color2 }
    }

    states: State {
        name: "Landscape"
        when: screen.width > screen.height

        PropertyChanges {
            target: output
            width: rect.width
            height: rect.height - buttonHeight
        }

        PropertyChanges {
            target: rect
            buttonWidth: rect.width / 3
            buttonHeight: 100
        }
    }
}
