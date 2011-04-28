#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "contactsmodel.h"
#include <QDeclarativeContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;

    ContactsModel contactsModel;
    QDeclarativeContext *context = viewer.rootContext();
    context->setContextProperty("contactsModel",  &contactsModel);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/contactsdemo/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
