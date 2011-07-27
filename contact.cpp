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
    
    // TODO: when we write a GalleryModel, we'll want some way to select part of
    // an image to use as an avatar, but this lazy hack will do for now
    QImage img(avatarPath.toLocalFile());

    // scale up or down
    img = img.scaled(QSize(128, 128), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    if (img.size().height() > 128 || img.size().width() > 128) {
        // scale down, and crop it to a square
        // TODO: would be nice to pick a region of the image to scale+crop.
        img = img.copy(QRect(0, 0, 128, 128));
    }
    setAvatar(img);
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

    // update avatar serial so QML reloads the image
    setAvatarSerial(avatarSerial() + 1);
}
