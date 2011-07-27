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

#include <QObject>
#include <QUrl>
#include <QImage>

#include <sobject.h>

class Contact : public QObject
{
    Q_OBJECT

public:
    explicit Contact(QObject *parent = 0);
    explicit Contact(const SObject &object, QObject *parent);
    virtual ~Contact() { }

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged);
    QString firstName() const;
    void setFirstName(const QString &firstName);

    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged);
    QString lastName() const;
    void setLastName(const QString &lastName);

    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged);
    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    Q_PROPERTY(QImage avatar READ avatar WRITE setAvatar NOTIFY avatarChanged);
    QImage avatar() const;
    void setAvatar(const QImage &avatar);
    Q_INVOKABLE void setAvatar(const QUrl &avatarPath);

    Q_PROPERTY(int avatarSerial READ avatarSerial WRITE setAvatarSerial NOTIFY avatarSerialChanged)
    int avatarSerial() const;
    void setAvatarSerial(int avatarSerial);

    Q_PROPERTY(QString localId READ localId);
    QString localId() const;

    void setData(SObject mData);

public slots:
    void remove();
    void save();

signals:
    void firstNameChanged();
    void lastNameChanged();
    void phoneNumberChanged();
    void avatarChanged();
    void avatarSerialChanged();

private:
    SObject mData;
    int mAvatarSerial;
};

