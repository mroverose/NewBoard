#include "WBExportDocument.h"

#include "frameworks/WBPlatformUtils.h"

#include "core/WBDocumentManager.h"
#include "core/WBApplication.h"

#include "document/WBDocumentProxy.h"
#include "document/WBDocumentController.h"

#include "globals/WBGlobals.h"

THIRD_PARTY_WARNINGS_DISABLE
//#include "quazip.h"
//#include "quazipfile.h"
THIRD_PARTY_WARNINGS_ENABLE

#include "core/memcheck.h"

WBExportDocument::WBExportDocument(QObject *parent)
    : WBExportAdaptor(parent)
{
        WBExportDocument::tr("Page"); // dummy slot for translation
}

WBExportDocument::~WBExportDocument()
{
    // NOOP
}

void WBExportDocument::persist(WBDocumentProxy* pDocumentProxy)
{
    persistLocally(pDocumentProxy, tr("Export as UBZ File"));
}


bool WBExportDocument::persistsDocument(WBDocumentProxy* pDocumentProxy, const QString &filename)
{

    return true;
}


void WBExportDocument::processing(const QString& pObjectName, int pCurrent, int pTotal)
{
    QString localized = WBExportDocument::trUtf8(pObjectName.toUtf8());

    if (mIsVerbose)
        WBApplication::showMessage(tr("Exporting %1 %2 of %3").arg(localized).arg(pCurrent).arg(pTotal));
}



QString WBExportDocument::exportExtention()
{
    return QString(".ubz");
}

QString WBExportDocument::exportName()
{
    return tr("Export to WBoard Format");
}

bool WBExportDocument::associatedActionactionAvailableFor(const QModelIndex &selectedIndex)
{
    const WBDocumentTreeModel *docModel = qobject_cast<const WBDocumentTreeModel*>(selectedIndex.model());
    if (!selectedIndex.isValid() || docModel->isCatalog(selectedIndex)) {
        return false;
    }

    return true;
}
