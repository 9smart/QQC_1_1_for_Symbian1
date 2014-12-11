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

#if defined(Q_COMPONENTS_SYMBIAN) && !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
#include "settingswindow.h"
#endif
#include "utils.h"
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>
#include <QDir>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<FileAccess>("FileAccess", 1, 0, "FileAccess");
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");
    qmlRegisterType<LayoutDirectionSetter>("LayoutDirectionSetter", 1, 0, "LayoutDirectionSetter");

    QDeclarativeView view;
    view.engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");

#ifndef Q_OS_SYMBIAN
    QDir::setCurrent(app.applicationDirPath());
#endif
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();

#if defined(Q_COMPONENTS_SYMBIAN) && !defined(Q_OS_SYMBIAN) && !defined(Q_WS_SIMULATOR)
    SettingsWindow settingsWindow(&view);
#endif
    return app.exec();
}
