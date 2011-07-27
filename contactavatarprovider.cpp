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

#include "contactavatarprovider.h"
#include "contactsmodel.h"
#include "contact.h"
#include <QDebug>

/* main.cpp */
extern ContactsModel *contactsModel;

ContactAvatarProvider::ContactAvatarProvider()
    : QDeclarativeImageProvider(Pixmap)
{
}

QPixmap ContactAvatarProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << Q_FUNC_INFO << "Getting avatar for " << id << id.section('/', 0, 0);
    Contact *contact = contactsModel->contactFor(id.section('/', 0, 0));
    qDebug() << contact;
    if (!contact)
        return QPixmap();

    QPixmap re = QPixmap::fromImage(contact->avatar());
    if (re.isNull())
        return QPixmap();

    *size = re.size();
    if (requestedSize.isValid())
        re = re.scaled(requestedSize);
    return re;
}
