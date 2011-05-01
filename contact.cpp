#include "contact.h"
#include "saesu/sobjectmanager.h"
#include "saesu/sobjectremoverequest.h"
#include "saesu/sobjectsaverequest.h"

/* main.cpp. Hack. Fix it. */
extern SObjectManager *globalManager;

Contact::Contact(const SObject &object, QObject *parent)
    : QObject(parent)
    , mData(SObject(object))
    , mAvatarSerial(0)
{
}

Contact::Contact(QObject *parent)
    : QObject(parent)
{
}

void Contact::remove()
{
    /* XXX I suspect that stack allocation may not be correct... is that true? */
    SObjectRemoveRequest *req = new SObjectRemoveRequest(this);
    connect(req, SIGNAL(finished()), req, SLOT(deleteLater()));
    req->add(mData.id().localId());
    req->start(globalManager);
}

void Contact::save()
{
    SObjectSaveRequest *req = new SObjectSaveRequest(this);
    connect(req, SIGNAL(finished()), req, SLOT(deleteLater()));
    req->add(mData);
    req->start(globalManager);
}

QString Contact::firstName() const
{
    return mData.value("firstName").toString();
}

void Contact::setFirstName(const QString &firstName)
{
    mData.setValue("firstName", firstName);
    emit firstNameChanged();
}

QString Contact::lastName() const
{
    return mData.value("lastName").toString();
}

void Contact::setLastName(const QString &lastName)
{
    mData.setValue("lastName", lastName);
    emit lastNameChanged();
}

QString Contact::phoneNumber() const
{
    return mData.value("phoneNumber").toString();
}

void Contact::setPhoneNumber(const QString &phoneNumber)
{
    mData.setValue("phoneNumber", phoneNumber);
    emit phoneNumberChanged();
}

QImage Contact::avatar() const
{
    qDebug() << mData.value("avatar").toByteArray().toHex();
    return mData.value("avatar").value<QImage>();
}

void Contact::setAvatar(const QImage &avatar)
{
    mAvatarSerial++;
    qDebug() << avatar << QVariant::fromValue(avatar).toByteArray().toHex();
    mData.setValue("avatar", QVariant::fromValue(avatar));
    emit avatarChanged();
}

void Contact::setAvatarSerial(int avatarSerial)
{
    mAvatarSerial = avatarSerial;
    emit avatarSerialChanged();
}

int Contact::avatarSerial() const
{
    return mAvatarSerial;
}

void Contact::setAvatar(const QUrl &avatarPath)
{
    qDebug() << avatarPath;
    
    // TODO: when we write a GalleryModel, move thumbnailing to there, and
    // don't block the UI thread while doing it. kthx.
    QImage img(avatarPath.toLocalFile());
    setAvatar(img.scaled(QSize(128, 128), Qt::KeepAspectRatio, SmoothTransformation));
}

QString Contact::localId() const
{
    return mData.id().localId().toString();
}

void Contact::setData(SObject data)
{
    SObject oldData = mData;
    mData = data;

    if (firstName() != oldData.value("firstName"))
        emit firstNameChanged();
    
    if (lastName() != oldData.value("lastName"))
        emit lastNameChanged();

    if (phoneNumber() != oldData.value("phoneNumber"))
        emit phoneNumberChanged();
}
