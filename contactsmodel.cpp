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

void ContactsModel::onReadAllComplete()
{
    SObjectFetchRequest *req = qobject_cast<SObjectFetchRequest*>(sender());
    beginResetModel();
    mObjects = req->objects();
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
    case FirstNameRole: return mObjects[index.row()].value("firstName");
    case LastNameRole: return mObjects[index.row()].value("lastName");
    case PhoneNumberRole: return mObjects[index.row()].value("phoneNumber");
    case AvatarPathRole: return mObjects[index.row()].value("avatar");
    }

    if (role != Qt::DisplayRole &&
        role != Qt::EditRole)
        return QVariant();

    switch (index.column()) {
        case 0:
            return mObjects[index.row()].id().localId().toString();
            break;
        case 1:
            return mObjects[index.row()].hash().toHex();
            break;
        case 2:
            return mObjects[index.row()].lastSaved();
            break;
        case 3:
            return mObjects[index.row()].value("name");
            break;
        case 4:
            return mObjects[index.row()].value("age");
            break;
    }

    return QVariant();
}

bool ContactsModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if (role != Qt::EditRole)
        return false;

    SObject newObject;
    SObject &object = newObject;

    if (index.row() < mObjects.count())
        object = mObjects[index.row()];

    bool hasChanged = false;

    switch (index.column()) {
        case 0:
        case 1:
        case 2:
            return false;
        case 3:
            object.setValue("name", value);
            hasChanged = true;
            break;
        case 4:
            object.setValue("age", value);
            hasChanged = true;
            break;
    }

    if (hasChanged) {
        emit dataChanged(index, index);
        SObjectSaveRequest *saveRequest = new SObjectSaveRequest;
        connect(saveRequest, SIGNAL(finished()), saveRequest, SLOT(deleteLater()));
        saveRequest->add(object);
        saveRequest->start(mManager);
        return true;
    }

    return hasChanged;
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
    mObjects.append(req->objects());
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
            if (mObjects.at(i).id().localId() == id) {
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

void ContactsModel::deleteRow(const QModelIndex &index)
{
    if (index.row() >= mObjects.count())
        return;

    beginResetModel();

    SObjectRemoveRequest *removeRequest = new SObjectRemoveRequest;
    connect(removeRequest, SIGNAL(finished()), removeRequest, SLOT(deleteLater()));
    removeRequest->add(mObjects.at(index.row()).id().localId());
    removeRequest->start(mManager);
    mObjects.removeAt(index.row());
    endResetModel();
}

QObject *ContactsModel::contactFor(int rowNumber)
{
    Contact *contact = new Contact(mObjects.at(rowNumber), this);
    return contact;
}
