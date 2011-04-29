#include "contact.h"
#include "saesu/sobjectmanager.h"
#include "saesu/sobjectremoverequest.h"
#include "saesu/sobjectsaverequest.h"

/* main.cpp. Hack. Fix it. */
extern SObjectManager *globalManager;

Contact::Contact(const SObject &object, QObject *parent)
    : QObject(parent)
    , mData(SObject(object))
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

QUrl Contact::avatar() const
{
    return mData.value("avatar").toUrl();
}

void Contact::setAvatar(const QUrl &avatar)
{
    mData.setValue("avatar", avatar);
    emit avatarChanged();
}

