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

#include "settingswindow.h"
#include <QPointer>
#include <QComboBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDebug>
#include <qmath.h>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QShortcut>

struct DisplayProfile {
    QString resolutionName;
    int resolutionWidth;
    int resolutionHeight;
    qreal dpiValue;
};

// These are for desktop use only, do not alter the order of the array!
static const DisplayProfile displayProfilesArray[] = {
    // ~3.47" NHD
    {"NHD Portrait", 360, 640, 211.7},
    {"NHD Landscape", 640, 360, 211.7},
    // ~3.17" NHD
    {"NHD Portrait", 360, 640, 232.0},
    {"NHD Landscape", 640, 360, 232.0},
    // ~3.95" NHD
    {"NHD Portrait", 360, 640, 186.1},
    {"NHD Landscape", 640, 360, 186.1},
    // ~2.46" VGA
    {"VGA Portrait", 480, 640, 325.6},
    {"VGA Landscape", 640, 480, 325.6},
    // 3.2" HVGA
    {"HVGA Portrait", 320, 480, 180},
    {"HVGA Landscape", 480, 320, 180},
    // 4.0" WVGA
    {"WVGA Portrait", 480, 864, 247.1},
    {"WVGA Landscape", 864, 480, 247.1},
    // 7.0" WSVGA
    {"WSVGA Portrait", 600, 1024, 169},
    {"WSVGA Landscape", 1024, 600, 169},
    // 11" XGA
    {"XGA Portrait", 768, 1024, 132},
    {"XGA Landscape", 1024, 768, 132}
};

static const int displayProfilesCount = sizeof(displayProfilesArray) / sizeof(DisplayProfile);


SettingsWindow::SettingsWindow(QDeclarativeView *view)
    : QWidget(0), // no reason to have parent for settingswindow
      updatingValuesBasedOnDisplayChange(false),
      userValueChanged(false),
      declarativeView(view)
{

    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    QGroupBox *globalGroup = new QGroupBox(tr("Global debug drawing"), this);
    QFormLayout *globalLayout = new QFormLayout(globalGroup);

    bordersComboBox = new QComboBox(this);
    bordersComboBox->addItems(QStringList() << tr("Invisible") << tr("Visible"));
    connect(bordersComboBox, SIGNAL(currentIndexChanged(int)), SLOT(changeBordersVisibility(int)));
    globalLayout->addRow(tr("&Borders"), bordersComboBox);

    globalGroup->setLayout(globalLayout);
    boxLayout->addWidget(globalGroup);

    resolutionComboBox = new QComboBox(this);

    screen = qVariantValue<QObject *>(declarativeView->engine()->rootContext()->contextProperty("screen"));

    QStringList resolutionTexts;
    for (int i = 0; i < displayProfilesCount; i++) {
        const DisplayProfile &profile = displayProfilesArray[i];
        QString resolutionName = profile.resolutionName;
        qreal dpiValue = profile.dpiValue;

        QString displayProfileAsText;
        displayProfileAsText.append(QString::number(profile.resolutionWidth));
        displayProfileAsText.append("x");
        displayProfileAsText.append(QString::number(profile.resolutionHeight));
        displayProfileAsText.append(" ");
        displayProfileAsText.append(resolutionName);
        displayProfileAsText.append(" ");

        qreal diagonal = qSqrt(static_cast<qreal>(profile.resolutionWidth * profile.resolutionWidth + profile.resolutionHeight * profile.resolutionHeight)); // in "pixels"
        QString inchSizeStr = QString::number(diagonal / dpiValue, 'g', 2);
        displayProfileAsText.append(inchSizeStr);

        resolutionTexts <<  displayProfileAsText;
    }
    resolutionComboBox->addItems(resolutionTexts);
    resolutionComboBox->setCurrentIndex(activeDisplayProfile());
    connect(resolutionComboBox, SIGNAL(currentIndexChanged(int)), SLOT(changeResolution(int)));

    QGroupBox *resolutionGroup = new QGroupBox(tr("Screen(device) resolution"));
    QFormLayout *resolutionLayout = new QFormLayout();
    resolutionLayout->addRow(tr("&Resolution:"), resolutionComboBox);
    resolutionGroup->setLayout(resolutionLayout);
    boxLayout->addWidget(resolutionGroup);

    dpiLineEdit = new QLineEdit(this);
    connect(dpiLineEdit, SIGNAL(textEdited(QString)), this, SLOT(userEditedDisplayValues()));
    connect(dpiLineEdit, SIGNAL(editingFinished()), this, SLOT(userEditingFinished()));
    resolutionLayout->addRow(tr("&DPI:"), dpiLineEdit);

    inchLineEdit = new QLineEdit(this);
    connect(inchLineEdit, SIGNAL(textEdited(QString)), this, SLOT(userEditedDisplayValues()));
    connect(inchLineEdit, SIGNAL(editingFinished()), this, SLOT(userEditingFinished()));
    resolutionLayout->addRow(tr("&Inch:"), inchLineEdit);

    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(0, 2000);
    connect(widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(userChangedDisplayValues()));
    resolutionLayout->addRow(tr("&Width:"), widthSpinBox);

    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(0, 2000);
    connect(heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(userChangedDisplayValues()));
    resolutionLayout->addRow(tr("&Height:"), heightSpinBox);

    displayChanged();
    connect(screen, SIGNAL(displayChanged()), this, SLOT(displayChanged()), Qt::QueuedConnection);
    connect(screen, SIGNAL(widthChanged()), this, SLOT(displayChanged()), Qt::QueuedConnection);
    connect(screen, SIGNAL(heightChanged()), this, SLOT(displayChanged()), Qt::QueuedConnection);
    setObjectName("settingsWindow");

    qDebug() << "----- SettingsWindow: Enabling Ctrl+Alt+Shift+S shortcut -----";
    QShortcut *shortcut = new QShortcut(QKeySequence(tr("Ctrl+Alt+Shift+S")), declarativeView);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(show()));

}

SettingsWindow::~SettingsWindow()
{
}

int SettingsWindow::activeDisplayProfile()
{
    for (int i = 0; i < displayProfilesCount; i++) {
        DisplayProfile profile = displayProfilesArray[i];

        if (profile.resolutionWidth == screen->property("width").value<int>()
            && profile.resolutionHeight == screen->property("height").value<int>()
            && qFuzzyCompare(profile.dpiValue, screen->property("dpi").value<qreal>())) {
            return i;
        }
    }
    return -1;
}

void SettingsWindow::changeBordersVisibility(int index)
{
    bool enableBorders = index > 0;
    declarativeView->setOptimizationFlag(QGraphicsView::IndirectPainting, enableBorders);
}

void SettingsWindow::changeResolution(int index)
{
    if (index != -1) {
        DisplayProfile profile = displayProfilesArray[index];
        QMetaObject::invokeMethod(screen, "privateSetDisplay",
                                  Q_ARG(int, profile.resolutionWidth),
                                  Q_ARG(int, profile.resolutionHeight),
                                  Q_ARG(qreal, profile.dpiValue));
    }
}

void SettingsWindow::displayChanged()
{
    updatingValuesBasedOnDisplayChange = true;

    const int newWidthValue = screen->property("width").value<int>();
    const int newHeightValue = screen->property("height").value<int>();
    const qreal newDpiValue = screen->property("dpi").value<qreal>();

    resolutionComboBox->setCurrentIndex(activeDisplayProfile());

    dpiLineEdit->setText(QString::number(newDpiValue, 'f', 2));

    qreal diagonal = qSqrt(static_cast<qreal>(newWidthValue * newWidthValue + newHeightValue * newHeightValue)); // in "pixels"
    QString inchSizeStr = QString::number(diagonal / newDpiValue, 'f', 2);
    inchLineEdit->setText(inchSizeStr);

    widthSpinBox->setValue(newWidthValue);
    heightSpinBox->setValue(newHeightValue);

    updatingValuesBasedOnDisplayChange = false;
}

void SettingsWindow::userEditedDisplayValues()
{
    userValueChanged = true;
}

void SettingsWindow::userChangedDisplayValues()
{
    userValueChanged = true;
    userEditingFinished();
}

void SettingsWindow::userEditingFinished()
{
    if (!userValueChanged || updatingValuesBasedOnDisplayChange)
        return;
    userValueChanged = false;

    qreal dpi(0.0);
    if (QObject::sender() == inchLineEdit) {
        const qreal inches = qreal(inchLineEdit->text().toDouble());
        const qreal diagonal = qSqrt(static_cast<qreal>(widthSpinBox->value() * widthSpinBox->value() + heightSpinBox->value() * heightSpinBox->value())); // in "pixels"
        dpi = diagonal / inches;
    } else {
        dpi = dpiLineEdit->text().toDouble();
    }

    QMetaObject::invokeMethod(screen, "privateSetDisplay",
                              Q_ARG(int, widthSpinBox->value()),
                              Q_ARG(int, heightSpinBox->value()),
                              Q_ARG(qreal, dpi));

}
