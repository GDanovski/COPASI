// Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2012 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <QtCore/QString>

#include "CQTabWidget.h"

#include "CQMessageBox.h"
#include "qtUtilities.h"
#include "CQNotes.h"

#include "MIRIAMUI/CQMiriamWidget.h"
#include "MIRIAMUI/CQRDFTreeView.h"

#include "CopasiDataModel/CDataModel.h"
#include "copasi/core/CRootContainer.h"
#include "function/CFunction.h"
#include "UI/CQCompartment.h"
#include "UI/CQSpeciesDetail.h"
#include "UI/ReactionsWidget1.h"
#include "UI/CQUnitDetail.h"

#include <copasi/UI/copasiui3window.h>

#ifdef COPASI_Provenance
#include "CEntityProvenanceDialog.h"
#include "../versioning/CModelVersionHierarchy.h"
#include "commandline/CConfigurationFile.h"
#endif

#include "copasi/undo/CUndoData.h"

CQTabWidget::CQTabWidget(const ListViews::ObjectType & objectType, CopasiWidget * pCopasiWidget,
                         QWidget * parent, Qt::WindowFlags f) :
  CopasiWidget(parent, NULL, f),
  mPages(),
  mObjectType(objectType),
  mIgnoreLeave(false)
{
  setupUi(this);

  mpLblName->setText("<h3>" + FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) + "</h3>");

  mpTabWidget->addTab(pCopasiWidget, "Details");
  mPages.push_back(pCopasiWidget);

  switch (mObjectType)
    {
      case  ListViews::MODEL:
        mpBtnNew->hide();
        mpBtnCopy->hide();
        mpBtnDelete->hide();
        break;

      case ListViews::MODELPARAMETERSET:
        mpBtnNew->setText("Apply");
        mpBtnNew->setToolTip("Apply the current parameters to the model.");

      // The break statement is intentionally missing

      default:
        CQNotes* pNotes = new CQNotes(mpTabWidget);
        mPages.push_back(pNotes);
        mpTabWidget->addTab(pNotes, "Notes");

        connect(this, SIGNAL(newClicked()), pCopasiWidget, SLOT(slotBtnNew()));
        connect(this, SIGNAL(copyClicked()), pCopasiWidget, SLOT(slotBtnCopy()));
        connect(this, SIGNAL(deleteClicked()), pCopasiWidget, SLOT(slotBtnDelete()));
        connect(this, SIGNAL(copyClicked()), pNotes, SLOT(slotBtnCopy()));
        break;
    }

  CQMiriamWidget* pMIRIAMWidget = new CQMiriamWidget(mpTabWidget);
  mPages.push_back(pMIRIAMWidget);
  mpTabWidget->addTab(pMIRIAMWidget, "Annotation");
  connect(this, SIGNAL(copyClicked()), pMIRIAMWidget, SLOT(slotBtnCopy()));

  CQRDFTreeView* pRDFTreeView = new CQRDFTreeView(mpTabWidget);
  mPages.push_back(pRDFTreeView);
  mpTabWidget->addTab(pRDFTreeView, "RDF Browser");

#ifdef COPASI_Provenance

  if ((FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) == "Species") || (FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) == "Compartment") || (FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) == "Reaction") || (FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) == "Event") || (FROM_UTF8(ListViews::ObjectTypeName[mObjectType]) == "Global Quantity"))
    {
      //FROM_UTF8(mpObject->getObjectName())
      //mpEntityProvenanceDialog = new CEntityProvenanceDialog(mpTabWidget, mpUndoStack, metaObject()->, pWindow->getVersionHierarchy()->getPathFile(), pWindow->getVersionHierarchy()->getVersionsPathToCurrentModel(),  pWindow->getProvenanceParentOfCurrentVersion(), pWindow->getVersionHierarchy()->getParentOfCurrentModel());
      mpEntityProvenanceDialog = new CEntityProvenanceDialog(mpTabWidget);
      mPathFile = pWindow->getVersionHierarchy()->getPathFile();
      mVersionPathToCurrentModel = pWindow->getVersionHierarchy()->getVersionsPathToCurrentModel();
      mPages.push_back(mpEntityProvenanceDialog);
      mpTabWidget->addTab(mpEntityProvenanceDialog, "Provenance");
      //connect(this, SIGNAL(activated()), this, SLOT(mpEntityProvenanceDialog->exec()));
      //connect(this, SIGNAL(),EntityProvenanceDialog ,SLOT(EntityProvenanceDialog->exec()));
    }

#endif
}

CQTabWidget::~CQTabWidget()
{
  // TODO Auto-generated destructor stub
}

bool CQTabWidget::leaveProtected()
{
  if (mIgnoreLeave) return true;

  bool success = save();

  std::vector< CopasiWidget * >::iterator it = mPages.begin();
  std::vector< CopasiWidget * >::iterator end = mPages.end();

  for (; it != end; ++it)
    success &= (*it)->leave();

  return true;
}

bool CQTabWidget::enterProtected()
{
  load();

  std::vector< CopasiWidget * >::iterator it = mPages.begin();
  std::vector< CopasiWidget * >::iterator end = mPages.end();

  for (; it != end; ++it)
    (*it)->enter(mObjectCN);

  return true;
}

bool CQTabWidget::updateProtected(ListViews::ObjectType objectType, ListViews::Action action, const CCommonName & cn)
{
  if (mIgnoreUpdates || !isVisible())
    {
      return true;
    }

  if (cn == mObjectCN)
    {
      if (objectType == mObjectType)
        {
          switch (action)
            {
              case ListViews::RENAME:
              case ListViews::CHANGE:
                load();
                break;

              case ListViews::DELETE:
                mpObject = NULL;
                mObjectCN.clear();
                break;

              default:
                break;
            }
        }
    }

  return true;
}

void CQTabWidget::selectTab(int index) const
{
  mpTabWidget->setCurrentIndex(index);
}

void CQTabWidget::load()
{
  if (mpObject != NULL)
    {
      mpEditName->setText(FROM_UTF8(mpObject->getObjectName()));

      if (mObjectType == ListViews::FUNCTION)
        {
          bool readOnly = static_cast< const CFunction * >(mpObject)->isReadOnly();

          mpEditName->setReadOnly(readOnly);
          mpBtnCommit->setEnabled(!readOnly);
          mpBtnRevert->setEnabled(!readOnly);
          mpBtnDelete->setEnabled(!readOnly);
        }
      else if (mObjectType == ListViews::UNIT)
        {
          bool readOnly = static_cast< const CUnitDefinition * >(mpObject)->isReadOnly();

          mpEditName->setReadOnly(readOnly);
          mpBtnCommit->setEnabled(!readOnly);
          mpBtnRevert->setEnabled(!readOnly);
          mpBtnDelete->setEnabled(!readOnly);
        }

#ifdef COPASI_Provenance

      if (mObjectType == ListViews::METABOLITE ||
          mObjectType == ListViews::COMPARTMENT ||
          mObjectType == ListViews::REACTION ||
          mObjectType == ListViews::EVENT ||
          mObjectType == ListViews::MODELVALUE)
        {
          CopasiUI3Window *  pWindow = dynamic_cast<CopasiUI3Window *>(CopasiUI3Window::getMainWindow());

          if (pWindow) //Probably could just assume the Main Window exists
            {
              QList<QString> VersioningPath = pWindow->getVersionHierarchy()->getVersionsPathToCurrentModel();
              mpEntityProvenanceDialog->load(mpUndoStack, FROM_UTF8(mpObject->getObjectName()), FROM_UTF8(CRootContainer::getConfiguration()->getWorkingDirectory()), VersioningPath);
            }
        }

#endif
    }
  else
    {
      mpEditName->setText("");
    }
}

bool CQTabWidget::save()
{
  if (mpObject == NULL) return false;

  bool success = true;

  // We need to tell the sub-widgets to ignore all notifications
  std::vector< CopasiWidget * >::iterator it = mPages.begin();
  std::vector< CopasiWidget * >::iterator end = mPages.end();

  for (; it != end; ++it)
    (*it)->setIgnoreUpdates(true);

  if (mpObject->getObjectName() != TO_UTF8(mpEditName->text()))
    {
      std::string NewName = TO_UTF8(mpEditName->text());
      CDataObject::sanitizeObjectName(NewName);
      CDataContainer * pParent = mpObject->getObjectParent();

      if (pParent != NULL &&
          pParent->hasFlag(CDataObject::NameVector) &&
          pParent->getObject("[" + CCommonName::escape(NewName) + "]") != NULL)
        {
          QString msg;
          msg = "Unable to rename " + FROM_UTF8(ListViews::ObjectTypeName[mObjectType]).toLower() + " '" + FROM_UTF8(mpObject->getObjectName()) + "'\n"
                + "to '" + FROM_UTF8(NewName) + "' since a " + FROM_UTF8(ListViews::ObjectTypeName[mObjectType]).toLower() + " with that name already exists.\n";

          CQMessageBox::information(this,
                                    "Unable to rename " + FROM_UTF8(ListViews::ObjectTypeName[mObjectType]),
                                    msg,
                                    QMessageBox::Ok, QMessageBox::Ok);

          mpEditName->setText(FROM_UTF8(mpObject->getObjectName()));
          success = false;
        }
      else
        {
          CUndoData Data(CUndoData::Type::CHANGE, mpObject->toData());
          Data.addProperty(CData::OBJECT_NAME, mpObject->getObjectName(), NewName);
          slotNotifyChanges(mpDataModel->applyData(Data));
        }
    }

  // We need to tell the sub-widgets to accept notifications again
  it = mPages.begin();
  end = mPages.end();

  for (; it != end; ++it)
    (*it)->setIgnoreUpdates(false);

  return success;
}

void CQTabWidget::slotBtnCommit()
{
  mpBtnCommit->setFocus();

  leaveProtected();
  enterProtected();
}

void CQTabWidget::slotBtnRevert()
{
  enterProtected();
}

void CQTabWidget::slotBtnDelete()
{
  mIgnoreLeave = true;
  emit deleteClicked();
  mIgnoreLeave = false;
}

void CQTabWidget::slotBtnNew()
{
  mpBtnNew->setFocus();

  leaveProtected();

  mIgnoreLeave = true;
  emit newClicked();
  mIgnoreLeave = false;
}

void CQTabWidget::slotBtnCopy()
{
  mpBtnCopy->setFocus();

  leaveProtected();

  mIgnoreLeave = true;

  // CQCompartments and CQSpecies have copy options, use CModelExpansion, and do their own switching.
  if (QString(mPages[0]->metaObject()->className()) == "CQCompartment")
    {
      CQCompartment * pQCompartment = dynamic_cast< CQCompartment * >(mPages[0]);
      pQCompartment->copy();
    }
  else if (QString(mPages[0]->metaObject()->className()) == "CQSpeciesDetail")
    {
      CQSpeciesDetail * pQSpeciesDetail = dynamic_cast< CQSpeciesDetail * >(mPages[0]);
      pQSpeciesDetail->copy();
    }
  else if (QString(mPages[0]->metaObject()->className()) == "ReactionsWidget1")
    {
      ReactionsWidget1 * pReactionsWidget1 = dynamic_cast< ReactionsWidget1 * >(mPages[0]);
      pReactionsWidget1->copy();
    }
  else if (QString(mPages[0]->metaObject()->className()) == "CQUnitDetail")
    {
      emit copyClicked();
    }
  else
    {
      emit copyClicked();
      emit newClicked();
    }

  mIgnoreLeave = false;
}

const int CQTabWidget::getNumTabs() const
{
  return (int)mPages.size();
}

CopasiWidget * CQTabWidget::getTab(int index)
{
  if (index < getNumTabs())
    return mPages[index];

  return NULL;
}
