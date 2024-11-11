#include "WBImportDocument.h"
#include "document/WBDocumentProxy.h"

#include "frameworks/WBFileSystemUtils.h"

#include "core/WBApplication.h"
#include "core/WBSettings.h"
#include "core/WBPersistenceManager.h"

#include "globals/WBGlobals.h"

THIRD_PARTY_WARNINGS_DISABLE
//#include "quazip.h"
//#include "quazipfile.h"
//#include "quazipfileinfo.h"
THIRD_PARTY_WARNINGS_ENABLE

#include "core/memcheck.h"

WBImportDocument::WBImportDocument(QObject *parent)
    :WBDocumentBasedImportAdaptor(parent)
{
    // NOOP
}

WBImportDocument::~WBImportDocument()
{
    // NOOP
}


QStringList WBImportDocument::supportedExtentions()
{
    return QStringList("ubz");
}


QString WBImportDocument::importFileFilter()
{
    return tr("WBoard (*.ubz)");
}


bool WBImportDocument::extractFileToDir(const QFile& pZipFile, const QString& pDir, QString& documentRoot)
{

    return true;
}

WBDocumentProxy* WBImportDocument::importFile(const QFile& pFile, const QString& pGroup)
{
    Q_UNUSED(pGroup); 

    QFileInfo fi(pFile);
    WBApplication::showMessage(tr("Importing file %1...").arg(fi.baseName()), true);

    QString path = WBSettings::userDocumentDirectory();

    QString documentRootFolder;

    if(!extractFileToDir(pFile, path, documentRootFolder)){
        WBApplication::showMessage(tr("Import of file %1 failed.").arg(fi.baseName()));
        return NULL;
    }

    WBDocumentProxy* newDocument = WBPersistenceManager::persistenceManager()->createDocumentFromDir(documentRootFolder, pGroup, "", false, false, true);
    WBApplication::showMessage(tr("Import successful."));
    return newDocument;
}

bool WBImportDocument::addFileToDocument(WBDocumentProxy* pDocument, const QFile& pFile)
{
    QFileInfo fi(pFile);
    WBApplication::showMessage(tr("Importing file %1...").arg(fi.baseName()), true);

    QString path = WBFileSystemUtils::createTempDir();

    QString documentRootFolder;
    if (!extractFileToDir(pFile, path, documentRootFolder))
    {
        WBApplication::showMessage(tr("Import of file %1 failed.").arg(fi.baseName()));
        return false;
    }

    if (!WBPersistenceManager::persistenceManager()->addDirectoryContentToDocument(documentRootFolder, pDocument))
    {
        WBApplication::showMessage(tr("Import of file %1 failed.").arg(fi.baseName()));
        return false;
    }

    WBFileSystemUtils::deleteDir(path);

    WBApplication::showMessage(tr("Import successful."));

    return true;
}


