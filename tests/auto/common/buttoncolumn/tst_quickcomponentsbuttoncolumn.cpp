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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsbuttoncolumn : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void exclusive();
    void checkedButton();

private:
    QObject *componentObject;
};

void tst_quickcomponentsbuttoncolumn::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsbuttoncolumn.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsbuttoncolumn::exclusive()
{
    // make sure property is present and set initial value
    QVERIFY(componentObject->setProperty("exclusive", false));

    // test property has right initial value
    QCOMPARE(componentObject->property("exclusive").toBool(), false);

    // first we set the checkable property of all Buttons that are
    // children of ButtonRow to false.
    QObject *child;
    const QObjectList children = componentObject->children();
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            QVERIFY2(child->property("checked").isValid(), "Error, ButtonColumn's child must have checked property");
            if (child->property("checkable").isValid()) {
                child->setProperty("checkable", false);
                QVERIFY2(child->property("checkable").toBool() == false, QString("Error, could not set checkable property of %1").arg(child->metaObject()->className()).toAscii().data());
            }
        }
    }

    // change value to true and test again
    QVERIFY(componentObject->setProperty("exclusive",true) );
    QCOMPARE(componentObject->property("exclusive").toBool(), true);

    // According to the Common API: "when true, forces all its children to be checkable"
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            if (child->property("checkable").isValid())
                QVERIFY2(child->property("checkable").toBool() == true, QString("Error, checkable property was not set to true in %1").arg(child->metaObject()->className()).toAscii().data());
        }
    }
}

void tst_quickcomponentsbuttoncolumn::checkedButton()
{
    // get ButtonColumn first Button child
    QVariant variant;
    QObject *child;
    QDeclarativeItem  *tmp;
    const QObjectList children = componentObject->children();
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            QVERIFY2(QMetaObject::invokeMethod(child, "clicked"), "ButtonColumn's child, does not have clicked() signal");
            variant = componentObject->property("checkedButton");
            QVERIFY(variant.isValid());
            tmp = variant.value<QDeclarativeItem*>();
            QVERIFY2(child->objectName() == tmp->objectName(), "Error, the returned object is not the one that was previously set");
        }
    }
}

QTEST_MAIN(tst_quickcomponentsbuttoncolumn)

#include "tst_quickcomponentsbuttoncolumn.moc"
