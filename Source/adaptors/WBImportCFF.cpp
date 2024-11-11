#include <QDir>
#include <QList>

#include "core/WBApplication.h"
#include "core/WBPersistenceManager.h"
#include "core/WBDocumentManager.h"
#include "core/WBPersistenceManager.h"
#include "document/WBDocumentProxy.h"
#include "domain/WBGraphicsPDFItem.h"
#include "frameworks/WBFileSystemUtils.h"
#include "pdf/PDFRenderer.h"

#include "WBCFFSubsetAdaptor.h"
#include "WBImportCFF.h"

#include "globals/WBGlobals.h"

THIRD_PARTY_WARNINGS_DISABLE
//#include "quazip.h"
//#include "quazipfile.h"
//#include "quazipfileinfo.h"
THIRD_PARTY_WARNINGS_ENABLE

#include "core/memcheck.h"

WBImportCFF::WBImportCFF(QObject *parent)
    : WBDocumentBasedImportAdaptor(parent)
{
    // NOOP
}


WBImportCFF::~WBImportCFF()
{
    // NOOP
}


QStringList WBImportCFF::supportedExtentions()
{
    return QStringList("iwb");
}


QString WBImportCFF::importFileFilter()
{
    QString filter = tr("Common File Format (");
    QStringList formats = supportedExtentions();
    bool isFirst = true;

    foreach(QString format, formats)
    {
            if(isFirst)
                    isFirst = false;
            else
                    filter.append(" ");

        filter.append("*."+format);
    }

    filter.append(")");

    return filter;
}

bool WBImportCFF::addFileToDocument(WBDocumentProxy* pDocument, const QFile& pFile)
{
    QFileInfo fi(pFile);
    WBApplication::showMessage(tr("Importing file %1...").arg(fi.baseName()), true);

    // first unzip the file to the correct place
    //TODO create temporary path for iwb file content
    QString path = QDir::tempPath();

    QString documentRootFolder = expandFileToDir(pFile, path);
        QString contentFile;
    if (documentRootFolder.isEmpty()) //if file has failed to unzip it is probably just xml file
        contentFile = pFile.fileName();
    else //get path to content xml (according to iwbcff specification)
        contentFile = documentRootFolder.append("/content.xml");

    if(!contentFile.length()){
            WBApplication::showMessage(tr("Import of file %1 failed.").arg(fi.baseName()));
            return false;
    }
    else{
        //TODO convert expanded CFF file content to the destination document
        //create destination document proxy
        //fill metadata and save
        WBDocumentProxy* destDocument = new WBDocumentProxy(WBPersistenceManager::persistenceManager()->generateUniqueDocumentPath());
        QDir dir;
        dir.mkdir(destDocument->persistencePath());

        //try to import cff to document
        if (WBCFFSubsetAdaptor::ConvertCFFFileToWbz(contentFile, destDocument))
        {
            WBPersistenceManager::persistenceManager()->addDirectoryContentToDocument(destDocument->persistencePath(), pDocument);
            WBFileSystemUtils::deleteDir(destDocument->persistencePath());
            delete destDocument;
            WBApplication::showMessage(tr("Import successful."));
            return true;
        }
        else
        {
            WBFileSystemUtils::deleteDir(destDocument->persistencePath());
            delete destDocument;
            WBApplication::showMessage(tr("Import failed."));
            return false;
        }
    }
}

QString WBImportCFF::expandFileToDir(const QFile& pZipFile, const QString& pDir)
{


    //create unique cff document root fodler
    //use current date/time and temp number for folder name
    QString documentRootFolder = "/";





    return documentRootFolder;
}


WBDocumentProxy* WBImportCFF::importFile(const QFile& pFile, const QString& pGroup)
{
    Q_UNUSED(pGroup); // group is defined in the imported file

    QFileInfo fi(pFile);
    WBApplication::showMessage(tr("Importing file %1...").arg(fi.baseName()), true);

    // first unzip the file to the correct place
    //TODO create temporary path for iwb file content
    QString path = QDir::tempPath();

    QString documentRootFolder = expandFileToDir(pFile, path);
    QString contentFile;
    if (documentRootFolder.isEmpty())
        //if file has failed to umzip it is probably just xml file
        contentFile = pFile.fileName();
    else
        //get path to content xml
        contentFile = QString("%1/content.xml").arg(documentRootFolder);

    if(!contentFile.length()){
            WBApplication::showMessage(tr("Import of file %1 failed.").arg(fi.baseName()));
            return 0;
    }
    else{
        //create destination document proxy
        //fill metadata and save
        WBDocumentProxy* destDocument = new WBDocumentProxy(WBPersistenceManager::persistenceManager()->generateUniqueDocumentPath());
        QDir dir;
        dir.mkdir(destDocument->persistencePath());
        if (pGroup.length() > 0)
            destDocument->setMetaData(WBSettings::documentGroupName, pGroup);
        if (fi.baseName() > 0)
            destDocument->setMetaData(WBSettings::documentName, fi.baseName());

        destDocument->setMetaData(WBSettings::documentVersion, WBSettings::currentFileVersion);
        destDocument->setMetaData(WBSettings::documentUpdatedAt, WBStringUtils::toUtcIsoDateTime(QDateTime::currentDateTime()));

        WBDocumentProxy* newDocument = NULL;
        //try to import cff to document
        if (WBCFFSubsetAdaptor::ConvertCFFFileToWbz(contentFile, destDocument))
        {
            newDocument = WBPersistenceManager::persistenceManager()->createDocumentFromDir(destDocument->persistencePath()
                                                                                            ,""
                                                                                            ,""
                                                                                            ,false
                                                                                            ,false
                                                                                            ,true);

            WBApplication::showMessage(tr("Import successful."));
        }
        else
        {
            WBFileSystemUtils::deleteDir(destDocument->persistencePath());
            WBApplication::showMessage(tr("Import failed."));
        }
        delete destDocument;

        if (documentRootFolder.length() != 0)
            WBFileSystemUtils::deleteDir(documentRootFolder);
        return newDocument;
    }
}
