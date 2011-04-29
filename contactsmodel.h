/*
 * Copyright (C) 2011 Robin Burchell <viroteck@viroteck.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
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
        AvatarPathRole
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
};
