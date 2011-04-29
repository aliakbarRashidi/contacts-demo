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

static bool contactsSort(const SObject &left, const SObject &right)
{
    QString lhs = left.value("firstName").toString() + " " + left.value("lastName").toString();
    QString rhs = right.value("firstName").toString() + " " + right.value("lastName").toString();
    return QString::localeAwareCompare(lhs, rhs) < 0;
}

void ContactsModel::addContacts(const QList<SObject> &objects, bool modelReset)
{
    QList<SObject>::Iterator it;

    if (modelReset) {
        beginResetModel();
        mObjects.clear();
    }

    foreach (const SObject &obj, objects) {
        it = qLowerBound(mObjects.begin(), mObjects.end(), obj, contactsSort);
        int dest;
        if (it == mObjects.end())
            dest = mObjects.count();
        else
            dest = it - mObjects.begin();
        
        if (!modelReset)
            beginInsertRows(QModelIndex(), dest, dest);
        mObjects.insert(dest, obj);
        if (!modelReset)
            endInsertRows();
    }
    
    if (modelReset) {
        endResetModel();
    }
}

void ContactsModel::onReadAllComplete()
{
    SObjectFetchRequest *req = qobject_cast<SObjectFetchRequest*>(sender());
    
    addContacts(req->objects(), true);

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

    const SObject &obj = mObjects.at(index.row());

    switch (role)
    {
    case FirstNameRole:
        return obj.value("firstName");
    case LastNameRole:
        return obj.value("lastName");
    case PhoneNumberRole:
        return obj.value("phoneNumber");
    case AvatarPathRole:
        return QImage();
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
    addContacts(req->objects(), false);
    endResetModel();

    sDebug() <<"Finished, " << mObjects.count() << " objects";
    req->deleteLater();
}

void ContactsModel::onObjectsRemoved(const QList<SObjectLocalId> &objects)
{
    // crazily inefficient, but hey, this is demo code!
    sDebug() << "Removing " << objects;
    foreach (const SObjectLocalId &id, objects) {
        for (int i = 0; i < mObjects.count(); ++i) {
            if (mObjects.at(i).id().localId() == id) {
                sDebug() << "Removing object at " << i;
                beginRemoveRows(QModelIndex(), i, i);
                mObjects.removeAt(i);
                endRemoveRows();
                i--; // so we check the next one above the one we just removed
            }
        }
    }
}

void ContactsModel::onObjectsUpdated(const QList<SObjectLocalId> &objects)
{
    // blow away the whole model, refetch everything. really, really inefficient.
    SObjectFetchRequest *fetchRequest = new SObjectFetchRequest;
    connect(fetchRequest, SIGNAL(finished()), SLOT(onReadUpdatesComplete()));
    SObjectLocalIdFilter localIdFilter;
    localIdFilter.setIds(objects);
    fetchRequest->setFilter(localIdFilter);
    fetchRequest->start(mManager);
}

void ContactsModel::onReadUpdatesComplete()
{
    SObjectFetchRequest *req = qobject_cast<SObjectFetchRequest*>(sender());

    QList<SObject> objects = req->objects();

    foreach (const SObject &object, objects) {
        for (int i = 0; i < mObjects.count(); ++i) {
            const SObject &oldObject = mObjects.at(i);
            if (oldObject.id().localId() != object.id().localId())
                continue;

            sDebug() << oldObject.value("firstName");
            sDebug() << object.value("firstName");

            // check if the firstName and lastName changed, if so, we need to
            // move them, if not, we need to just emit dataChanged
            if (oldObject.value("firstName") != object.value("firstName") ||
                oldObject.value("lastName") != object.value("lastName")) {
                // names differ, "move" (or at least simulate it by adding a
                // new contact and removing the old one)
                int source = i;
                
                QList<SObject>::Iterator it;
                it = qLowerBound(mObjects.begin(), mObjects.end(), object, contactsSort);
                int dest;
                if (it == mObjects.end())
                    dest = mObjects.count() - 1;
                else {
                    dest = it - mObjects.begin();

                    if (dest > source)
                        dest--;
                }

                if (source == dest) {
                    QModelIndex row = index(source, 0, QModelIndex());
                    mObjects.replace(source, object);
                    emit dataChanged(row, row);
                } else {
                    // do the actual move
                    beginMoveRows(QModelIndex(), source, source, QModelIndex(), (dest > source) ? (dest + 1) : dest);
                    mObjects.removeAt(source);
                    mObjects.insert(dest, object);
                    endMoveRows();
                }
            } else {
                QModelIndex row = index(i, 0, QModelIndex());
                mObjects.replace(i, object);
                emit dataChanged(row, row);
            }

            break;
        }
    }

    sDebug() <<"Finished, " << mObjects.count() << " objects";
    req->deleteLater();
}

QObject *ContactsModel::contactFor(int rowNumber)
{
    // XXX memory leak and ugly design
    Contact *contact = new Contact(mObjects.at(rowNumber), this);
    return contact;
}

QObject *ContactsModel::blankContact()
{
    /* XXX memory leak and ugly design */
    return new Contact(this);
}
