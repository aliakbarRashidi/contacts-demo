#include "contact.h"
#include "saesu/sobjectmanager.h"
#include "saesu/sobjectremoverequest.h"
#include "saesu/sobjectsaverequest.h"

Contact::Contact(const SObject &object, QObject *parent)
    : QObject(parent)
    , mData(object)
{

}

void Contact::remove()
{
    /* XXX Should not create a new manager for every one of these, probably..? */
    /* XXX I suspect that stack allocation may not be correct. */
    SObjectManager manager("saesu");
    SObjectRemoveRequest req;
    req.add(mData.id().localId());
    req.start(&manager);
}

void Contact::save()
{
    SObjectManager manager("saesu");
    SObjectSaveRequest req;
    req.add(mData);
    req.start(&manager);
}

QVariant Contact::firstName() const
{
    return mData.value("firstName");
}

void Contact::setFirstName(const QVariant &firstName)
{
    mData.setValue("firstName", firstName);
    emit firstNameChanged();
}

QVariant Contact::lastName() const
{
    return mData.value("lastName");
}

void Contact::setLastName(const QVariant &lastName)
{
    mData.setValue("lastName", lastName);
    emit lastNameChanged();
}

QVariant Contact::phoneNumber() const
{
    return mData.value("phoneNumber");
}

void Contact::setPhoneNumber(const QVariant &phoneNumber)
{
    mData.setValue("phoneNumber", phoneNumber);
    emit phoneNumberChanged();
}

