#ifndef CONTACTAVATARPROVIDER_H
#define CONTACTAVATARPROVIDER_H

#include <QDeclarativeImageProvider>

class ContactAvatarProvider : public QDeclarativeImageProvider
{
public:
    ContactAvatarProvider();

    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // CONTACTAVATARPROVIDER_H
