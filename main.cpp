#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "contactsmodel.h"
#include "contact.h"
#include <QDeclarativeContext>
#include <QtDeclarative>

/* Ugly hack, used for contact.cpp. Robin should fix this. */
SObjectManager *globalManager;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;

    globalManager = new SObjectManager("saesu");

    ContactsModel contactsModel(globalManager);
    QDeclarativeContext *context = viewer.rootContext();
    context->setContextProperty("contactsModel",  &contactsModel);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/contactsdemo/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
