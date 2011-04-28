#include "contact.h"

Contact::Contact(const SObject &object, QObject *parent)
    : QObject(parent)
    , mData(object)
{

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

