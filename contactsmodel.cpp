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
#include <QImage>

// saesu
#include "sobjectmanager.h"
#include "sobjectfetchrequest.h"
#include "sobjectlocalidfilter.h"
#include "sobjectdetailfilter.h"
#include "sobjectsaverequest.h"
#include "sobjectremoverequest.h"
#include "sobjectid.h"

// Us
#include "contactsmodel.h"
#include "contact.h"

ContactsModel::ContactsModel(SObjectManager *m, QObject *parent)
    : QAbstractItemModel(parent)
    , mManager(m)
{
    QHash<int,QByteArray> roles;
    roles[FirstNameRole] = "firstName";
    roles[LastNameRole] = "lastName";
    roles[PhoneNumberRole] = "phoneNumber";
    roles[AvatarPathRole] = "avatar";
    setRoleNames(roles);

    connect(mManager, SIGNAL(objectsAdded(QList<SObjectLocalId>)), SLOT(onObjectsAdded(QList<SObjectLocalId>)));
    connect(mManager, SIGNAL(objectsRemoved(QList<SObjectLocalId>)), SLOT(onObjectsRemoved(QList<SObjectLocalId>)));
    connect(mManager, SIGNAL(objectsUpdated(QList<SObjectLocalId>)), SLOT(onObjectsUpdated(QList<SObjectLocalId>)));
    SObjectFetchRequest *fetchRequest = new SObjectFetchRequest;
    connect(fetchRequest, SIGNAL(finished()), SLOT(onReadAllComplete()));
    fetchRequest->start(mManager);
}

static bool contactsSort(Contact *left, Contact *right)
{
    return QString::localeAwareCompare(left->displayLabel(), right->displayLabel()) < 0;
}

void ContactsModel::addContacts(const QList<SObject> &objects)
{
    QList<Contact*>::Iterator it;

    foreach (const SObject &obj, objects) {
        Contact *contact = new Contact(obj, this);
        it = qLowerBound(mObjects.begin(), mObjects.end(), contact, contactsSort);
        mObjects.insert(it, contact);
    }
}

void ContactsModel::onReadAllComplete()
{
    SObjectFetchRequest *req = qobject_cast<SObjectFetchRequest*>(sender());
    
    beginResetModel();
    mObjects.clear();
    addContacts(req->objects());
    endResetModel();

    sDebug() <<"Finished, " << mObjects.count() << " objects";
    req->deleteLater();
}

QModelIndex ContactsModel::index(int row, int col,  const QModelIndex&) const
{
    return createIndex(row, col);
}

int ContactsModel::rowCount(const QModelIndex &index) const
{
    return mObjects.count();
}

int ContactsModel::columnCount(const QModelIndex &index) const
{
    return 5;
}

QModelIndex ContactsModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

Qt::ItemFlags ContactsModel::flags ( const QModelIndex & index ) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= mObjects.count())
        return QVariant();

    switch (role)
    {
    case FirstNameRole: return mObjects[index.row()]->firstName();
    case LastNameRole: return mObjects[index.row()]->lastName();
    case PhoneNumberRole: return mObjects[index.row()]->phoneNumber();
    case AvatarPathRole: return QImage();
    }

    return QVariant();
}

void ContactsModel::onObjectsAdded(const QList<SObjectLocalId> &objects)
{
    SObjectFetchRequest *fetchRequest = new SObjectFetchRequest;
    connect(fetchRequest, SIGNAL(finished()), SLOT(onFetchedNewObjects()));
    SObjectLocalIdFilter localIdFilter;
    localIdFilter.setIds(objects);
    fetchRequest->setFilter(localIdFilter);
    fetchRequest->start(mManager);
}

void ContactsModel::onFetchedNewObjects()
{
    SObjectFetchRequest *req = qobject_cast<SObjectFetchRequest*>(sender());

    beginResetModel(); // this is *criminally* lazy
    addContacts(req->objects());
    endResetModel();

    sDebug() <<"Finished, " << mObjects.count() << " objects";
    req->deleteLater();
}

void ContactsModel::onObjectsRemoved(const QList<SObjectLocalId> &objects)
{
    // crazily inefficient, but hey, this is demo code!
    sDebug() << "Removing " << objects;
    beginResetModel();
    foreach (const SObjectLocalId &id, objects) {
        for (int i = 0; i < mObjects.count(); ++i) {
            if (mObjects.at(i)->data().id().localId() == id) {
                sDebug() << "Removing object at " << i;
                mObjects.removeAt(i);
                i--; // so we check the next one above the one we just removed
            }
        }
    }
    endResetModel();
}

void ContactsModel::onObjectsUpdated(const QList<SObjectLocalId> &objects)
{
    // blow away the whole model, refetch everything. really, really inefficient.
    SObjectFetchRequest *fetchRequest = new SObjectFetchRequest;
    connect(fetchRequest, SIGNAL(finished()), SLOT(onReadAllComplete()));
    fetchRequest->start(mManager);
}

QObject *ContactsModel::contactFor(int rowNumber)
{
    return mObjects.at(rowNumber);
}

QObject *ContactsModel::blankContact()
{
    /* XXX memory leak and ugly design */
    return new Contact(this);
}
