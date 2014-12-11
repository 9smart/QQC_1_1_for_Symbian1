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
import com.nokia.symbian 1.1
import "UIConstants.js" as UI

FlickrPage {
    id: details

    property string photoAuthor
    property string photoDate
    property string photoDescription
    property string photoTags
    property string photoTitle
    property int photoHeight
    property int photoWidth

    function setPhotoData(author, date, description, tags, title, photoWidth, photoHeight) {
        details.photoAuthor = author;
        details.photoDate = date;
        details.photoDescription = description;
        details.photoTags = tags;
        details.photoTitle = title;
        details.photoHeight = photoHeight;
        details.photoWidth = photoWidth;
    }

    Flickable {
        id: flickable

        width: parent.width; height: parent.height
        contentWidth: width
        contentHeight: content.childrenRect.height + UI.MARGIN_XLARGE * 2
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: content

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: UI.MARGIN_XLARGE
            spacing: UI.DETAILS_SPACING

            DetailText {
                text: "<big><b>" + details.photoTitle + "</b></big>"
            }

            DetailText {
                text: "<big><b>" + qsTr("Description") + "&nbsp;</b></big>" + details.photoDescription
            }

            DetailText {
                text: "<big><b>" + qsTr("Author") + "&nbsp;</b></big>" + details.photoAuthor
            }

            DetailText {
                text: "<big><b>" + qsTr("Published") + "&nbsp;</b></big>" + details.photoDate
            }

            DetailText {
                text: "<big><b>" + qsTr("Size") + "&nbsp;</b></big>" + details.photoWidth + qsTr("x")
                      + details.photoHeight
            }

            DetailText {
                text: "<big><b>" + qsTr("Tags") + "&nbsp;</b></big>" + details.photoTags
            }
        }
    }
}
