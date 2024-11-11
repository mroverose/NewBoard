#include "WBImportDocumentSetAdaptor.h"

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

WBImportDocumentSetAdaptor::WBImportDocumentSetAdaptor(QObject *parent)
    :WBImportAdaptor(parent)
{
    // NOOP
}

WBImportDocumentSetAdaptor::~WBImportDocumentSetAdaptor()
{
    // NOOP
}


QStringList WBImportDocumentSetAdaptor::supportedExtentions()
{
    return QStringList("ubx");
}


QString WBImportDocumentSetAdaptor::importFileFilter()
{
    return tr("Wboard (set of documents) (*.ubx)");
}

QFileInfoList WBImportDocumentSetAdaptor::importData(const QString &zipFile, const QString &destination)
{
    QString tmpDir;
    int i = 0;
    QFileInfoList result;
    do {
      tmpDir = QDir::tempPath() + "/Sankore_tmpImportUBX_" + QString::number(i++);
    } while (QFileInfo(tmpDir).exists());

    QDir(QDir::tempPath()).mkdir(tmpDir);

    QFile fZipFile(zipFile);

    if (!extractFileToDir(fZipFile, tmpDir)) {
        WBFileSystemUtils::deleteDir(tmpDir);
        return QFileInfoList();
    }

    QDir tDir(tmpDir);

    foreach(QFileInfo readDir, tDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden , QDir::Name)) {
        QString newFileName = readDir.fileName();
        if (QFileInfo(destination + "/" + readDir.fileName()).exists()) {
            newFileName = QFileInfo(WBPersistenceManager::persistenceManager()->generateUniqueDocumentPath(tmpDir)).fileName();
        }
        QString newFilePath = destination + "/" + newFileName;
        if (WBFileSystemUtils::copy(readDir.absoluteFilePath(), newFilePath)) {
            result.append(newFilePath);
        }
    }

    WBFileSystemUtils::deleteDir(tmpDir);

    return result;
}


bool WBImportDocumentSetAdaptor::extractFileToDir(const QFile& pZipFile, const QString& pDir)
{

    return true;
}
