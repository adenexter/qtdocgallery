/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDocGallery module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=src/gallery

#include <qdocumentgallery.h>

#include <QtTest/QtTest>

Q_DECLARE_METATYPE(QT_ADDON_GELLERY_PREPEND_NAMESPACE(QGalleryProperty::Attributes))

QT_ADDON_USE_GALLERY_NAMESPACE

class tst_QDocumentGallery : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void isRequestSupported();
    void itemTypeProperties_data();
    void itemTypeProperties();
    void propertyAttributes_data();
    void propertyAttributes();

private:
    QDocumentGallery gallery;
};

void tst_QDocumentGallery::isRequestSupported()
{
#if (defined(Q_OS_LINUX) && (defined(QT_TRACKER_ENABLED) || defined(QT_SIMPLE_ENABLED)))
    const bool platformSupported = true;
#else
    const bool platformSupported = false;
#endif

    QCOMPARE(gallery.isRequestSupported(QGalleryAbstractRequest::QueryRequest), platformSupported);
    QCOMPARE(gallery.isRequestSupported(QGalleryAbstractRequest::ItemRequest), platformSupported);

#if defined(Q_OS_LINUX) && defined(QT_SIMPLE_ENABLED)
    QCOMPARE(gallery.isRequestSupported(QGalleryAbstractRequest::TypeRequest), false);
#else
    QCOMPARE(gallery.isRequestSupported(QGalleryAbstractRequest::TypeRequest), platformSupported);
#endif
    QCOMPARE(gallery.isRequestSupported(QGalleryAbstractRequest::RequestType(1000)), false);
}

void tst_QDocumentGallery::itemTypeProperties_data()
{
    QTest::addColumn<QString>("itemType");
    QTest::addColumn<QStringList>("propertyNames");

    QTest::newRow("null item type") << QString() << QStringList();
    QTest::newRow("non-existent item type") << QString::fromLatin1("Hello") << QStringList();

    const QStringList fileProperties = QStringList()
#if defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED)
            << QDocumentGallery::author
            << QDocumentGallery::comments
            << QDocumentGallery::copyright
            << QDocumentGallery::description
            << QDocumentGallery::fileExtension
            << QDocumentGallery::fileName
            << QDocumentGallery::filePath
            << QDocumentGallery::fileSize
            << QDocumentGallery::keywords
            << QDocumentGallery::language
            << QDocumentGallery::lastAccessed
            << QDocumentGallery::lastModified
            << QDocumentGallery::mimeType
            << QDocumentGallery::path
            << QDocumentGallery::rating
            << QDocumentGallery::subject
            << QDocumentGallery::title
            << QDocumentGallery::url;
#endif
            ;
    QTest::newRow("File") << QString(QDocumentGallery::File) << (QStringList(fileProperties));

    QTest::newRow("Audio") << QString(QDocumentGallery::Audio) << (QStringList(fileProperties)
#if defined(Q_OS_LINUX)
#if defined(QT_TRACKER_ENABLED)
            << QDocumentGallery::albumArtist
            << QDocumentGallery::albumTitle
            << QDocumentGallery::artist
            << QDocumentGallery::audioBitRate
            << QDocumentGallery::audioCodec
            << QDocumentGallery::channelCount
            << QDocumentGallery::discNumber
            << QDocumentGallery::duration
            << QDocumentGallery::genre
            << QDocumentGallery::lastPlayed
            << QDocumentGallery::lyrics
            << QDocumentGallery::playCount
            << QDocumentGallery::sampleRate
            << QDocumentGallery::trackNumber
            << QDocumentGallery::performer
            << QDocumentGallery::composer
#elif defined(QT_SIMPLE_ENABLED)
            << QDocumentGallery::url
            << QDocumentGallery::title
            << QDocumentGallery::artist
            << QDocumentGallery::albumTitle
            << QDocumentGallery::albumArtist
            << QDocumentGallery::trackNumber
            << QDocumentGallery::genre
            << QLatin1String("albumArt")
#endif
#endif
    );

    QTest::newRow("Album") << QString(QDocumentGallery::Album) << (QStringList()
#if defined(Q_OS_LINUX)
#if defined(QT_TRACKER_ENABLED)
            << QDocumentGallery::albumArtist
            << QDocumentGallery::albumTitle
            << QDocumentGallery::artist
            << QDocumentGallery::duration
            << QDocumentGallery::title
            << QDocumentGallery::trackCount
#elif defined(QT_SIMPLE_ENABLED)
            << QLatin1String("albumArt")
            << QDocumentGallery::title
            << QDocumentGallery::artist
            << QDocumentGallery::albumArtist
#endif
#endif
    );
    QTest::newRow("PhotoAlbum") << QString(QDocumentGallery::PhotoAlbum) << (QStringList()
#if defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED)
                << QDocumentGallery::count
                << QDocumentGallery::title
#endif
    );
}

void tst_QDocumentGallery::itemTypeProperties()
{
    QFETCH(QString, itemType);
    QFETCH(QStringList, propertyNames);

    QStringList galleryPropertyNames = gallery.itemTypePropertyNames(itemType);
    propertyNames.sort();
    galleryPropertyNames.sort();

    QCOMPARE(galleryPropertyNames, propertyNames);
}

void tst_QDocumentGallery::propertyAttributes_data()
{
    QTest::addColumn<QString>("itemType");
    QTest::addColumn<QString>("propertyName");
    QTest::addColumn<QGalleryProperty::Attributes>("propertyAttributes");

    QTest::newRow("Null itemType, propertyName")
            << QString()
            << QString()
            << QGalleryProperty::Attributes();
    QTest::newRow("Null itemType, invalid propertyName")
            << QString()
            << QString::fromLatin1("Goodbye")
            << QGalleryProperty::Attributes();
    QTest::newRow("Null itemType, valid propertyName")
            << QString()
            << QString(QDocumentGallery::fileName)
            << QGalleryProperty::Attributes();
    QTest::newRow("Invalid itemType, invalid propertyName")
            << QString::fromLatin1("Hello")
            << QString::fromLatin1("Goodbye")
            << QGalleryProperty::Attributes();
    QTest::newRow("Invalid itemType, valid propertyName")
            << QString::fromLatin1("Hello")
            << QString(QDocumentGallery::fileName)
            << QGalleryProperty::Attributes();
    QTest::newRow("Valid itemType, invalid propertyName")
            << QString(QDocumentGallery::File)
            << QString::fromLatin1("Goodbye")
            << QGalleryProperty::Attributes();
    QTest::newRow("File.fileName")
            << QString(QDocumentGallery::File)
            << QString(QDocumentGallery::fileName)
#if (defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED))
            << (QGalleryProperty::CanRead
                    | QGalleryProperty::CanFilter
                    | QGalleryProperty::CanSort);
#else
            << QGalleryProperty::Attributes();
#endif
    QTest::newRow("File.filePath")
                    << QString(QDocumentGallery::File)
                    << QString(QDocumentGallery::filePath)
#if (defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED))
                    << (QGalleryProperty::CanRead | QGalleryProperty::CanFilter);
#else
                    << QGalleryProperty::Attributes();
#endif

    QTest::newRow("Audio.albumTitle")
            << QString(QDocumentGallery::Audio)
            << QString(QDocumentGallery::albumTitle)
#if (defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED))
            << (QGalleryProperty::CanRead
                    | QGalleryProperty::CanFilter
                    | QGalleryProperty::CanSort);
#else
            << QGalleryProperty::Attributes();
#endif
    QTest::newRow("Album.duration")
            << QString(QDocumentGallery::Album)
            << QString(QDocumentGallery::duration)
#if defined(Q_OS_LINUX) && defined(QT_TRACKER_ENABLED)
            << (QGalleryProperty::CanRead
                    | QGalleryProperty::CanFilter
                    | QGalleryProperty::CanSort);
#else
            << QGalleryProperty::Attributes();
#endif
}

void tst_QDocumentGallery::propertyAttributes()
{
    QFETCH(QString, itemType);
    QFETCH(QString, propertyName);
    QFETCH(QGalleryProperty::Attributes, propertyAttributes);
#if !defined(QT_SIMPLE_ENABLED)
    QCOMPARE(int(gallery.propertyAttributes(propertyName, itemType)), int(propertyAttributes));
#endif
}

#include "tst_qdocumentgallery.moc"

QTEST_MAIN(tst_QDocumentGallery)
