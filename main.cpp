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
