#include "contactavatarprovider.h"
#include "contactsmodel.h"
#include "contact.h"
#include <QDebug>

/* main.cpp */
extern ContactsModel *contactsModel;

ContactAvatarProvider::ContactAvatarProvider()
    : QDeclarativeImageProvider(Pixmap)
{
}

QPixmap ContactAvatarProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << Q_FUNC_INFO << "Getting avatar for " << id << id.section('/', 0, 0);
    Contact *contact = contactsModel->contactFor(id.section('/', 0, 0));
    qDebug() << contact;
    if (!contact)
        return QPixmap();

    QPixmap re = QPixmap::fromImage(contact->avatar());
    if (re.isNull())
        return QPixmap();

    *size = re.size();
    if (requestedSize.isValid())
        re = re.scaled(requestedSize);
    return re;
}
