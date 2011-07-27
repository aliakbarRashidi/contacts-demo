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

// Qt
#include <QAbstractItemModel>

// saesu
#include <sobject.h>
#include <sobjectmanager.h>

class Contact;

class ContactsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum {
        FirstNameRole = Qt::UserRole,
        LastNameRole,
        PhoneNumberRole,
        ContactIdRole,
        AvatarSerialRole
    };

    ContactsModel(SObjectManager *manager, QObject *parent = 0);

    virtual QModelIndex index(int,  int,  const QModelIndex&) const;
    virtual int rowCount(const QModelIndex&) const;
    virtual int columnCount(const QModelIndex&) const;
    virtual QModelIndex parent(const QModelIndex&) const;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data(const QModelIndex&,  int) const;
    Q_INVOKABLE QObject *contactFor(int rowNumber);
    Q_INVOKABLE QObject *blankContact();

    Contact *contactFor(const QString &uuid) const;

private slots:
    void onReadAllComplete();
    void onObjectsAdded(const QList<SObjectLocalId> &objects);
    void onFetchedNewObjects();
    void onObjectsRemoved(const QList<SObjectLocalId> &objects);
    void onObjectsUpdated(const QList<SObjectLocalId> &objects);
    void onReadUpdatesComplete();

private:
    void addContacts(const QList<SObject> &objects, bool modelReset);

    SObjectManager *mManager;
    QList<SObject> mObjects;
    QHash<QString, Contact *> mContactHash;
};
