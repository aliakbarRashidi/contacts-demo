/*
 * Copyright (C) 2010-2011 Robin Burchell
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "contactsmodel.h"
#include "contactavatarprovider.h"
#include <QDeclarativeContext>
#include <QDeclarativeEngine>

/* Ugly hack, used for contact.cpp. Robin should fix this. */
SObjectManager *globalManager;
/* Ugly hack again, used for contactavatarprovider.cpp. Also Robin's problem. */
ContactsModel *contactsModel;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;
    viewer.engine()->addImageProvider("contactavatar", new ContactAvatarProvider);

    globalManager = new SObjectManager("contacts");

    contactsModel = new ContactsModel(globalManager);
    QDeclarativeContext *context = viewer.rootContext();
    context->setContextProperty("contactsModel",  contactsModel);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/contactsdemo/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
