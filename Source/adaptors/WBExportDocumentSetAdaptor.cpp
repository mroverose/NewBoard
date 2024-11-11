#include "WBExportDocumentSetAdaptor.h"
#include "WBExportDocument.h"

#include "frameworks/WBPlatformUtils.h"

#include "core/WBDocumentManager.h"
#include "core/WBApplication.h"

#include "document/WBDocumentProxy.h"
#include "document/WBDocumentController.h"

#include "globals/WBGlobals.h"
#include "core/WBPersistenceManager.h"
#include "core/WBForeignObjectsHandler.h"

THIRD_PARTY_WARNINGS_DISABLE
//#include "quazip.h"
//#include "quazipfile.h"
THIRD_PARTY_WARNINGS_ENABLE

#include "core/memcheck.h"

WBExportDocumentSetAdaptor::WBExportDocumentSetAdaptor(QObject *parent)
    : WBExportAdaptor(parent)
{

}

WBExportDocumentSetAdaptor::~WBExportDocumentSetAdaptor()
{
    // NOOP
}

void WBExportDocumentSetAdaptor::persist(WBDocumentProxy* pDocumentProxy)
{
    QModelIndex treeViewParentIndex;
    WBPersistenceManager *persistenceManager = WBPersistenceManager::persistenceManager();
    WBDocumentTreeModel *treeModel = persistenceManager->mDocumentTreeStructureModel;
    QString filename;

    if (pDocumentProxy) {
        treeViewParentIndex = treeModel->indexForProxy(pDocumentProxy);
        if (!treeViewParentIndex.isValid()) {
            qDebug() << "failed to export";
            WBApplication::showMessage(tr("Failed to export..."));
            return;
        }
        filename = askForFileName(pDocumentProxy, tr("Export as UBX File"));

    } else {
        treeViewParentIndex = WBApplication::documentController->firstSelectedTreeIndex();
        if (!treeViewParentIndex.isValid()) {
            qDebug() << "failed to export";
            WBApplication::showMessage(tr("Failed to export..."));
            return;
        }

        WBDocumentTreeNode* node = treeModel->nodeFromIndex(treeViewParentIndex);
        WBDocumentProxy proxy;
        proxy.setMetaData(WBSettings::documentName,node->displayName());
        filename = askForFileName(&proxy, tr("Export as UBX File"));
    }

    if (filename.length() > 0)
    {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        if (mIsVerbose)
            WBApplication::showMessage(tr("Exporting document..."));

         if (persistData(treeViewParentIndex, filename)) {
             if (mIsVerbose) {
                 WBApplication::showMessage(tr("Export successful."));
             }
         } else {
             if (mIsVerbose) {
                 WBApplication::showMessage(tr("Export failed."));
             }
         }

         QApplication::restoreOverrideCursor();
    }
}

bool WBExportDocumentSetAdaptor::persistData(const QModelIndex &pRootIndex, QString filename)
{

    return true;
}

QString WBExportDocumentSetAdaptor::exportExtention()
{
    return QString(".ubx");
}

QString WBExportDocumentSetAdaptor::exportName()
{
    return tr("Export to WBoard UBX Format");
}

bool WBExportDocumentSetAdaptor::addDocumentToZip(const QModelIndex &pIndex, WBDocumentTreeModel *model)
{

    return true;
}

bool WBExportDocumentSetAdaptor::associatedActionactionAvailableFor(const QModelIndex &selectedIndex)
{
    const WBDocumentTreeModel *docModel = qobject_cast<const WBDocumentTreeModel*>(selectedIndex.model());
    if (!selectedIndex.isValid() || docModel->isDocument(selectedIndex)) {
        return false;
    }

    return true;
}

