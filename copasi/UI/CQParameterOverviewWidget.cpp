// Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2012 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <QSortFilterProxyModel>

#include "CQParameterOverviewWidget.h"
#include "CQParameterOverviewDM.h"
#include "CQMessageBox.h"
#include "CopasiFileDialog.h"
#include "qtUtilities.h"
#include "CQNameSelectionDialog.h"
#include "CQPushButtonDelegate.h"
#include "CQComboDelegate.h"

#include "commandline/CLocaleString.h"
#include "CopasiDataModel/CDataModel.h"
#include "model/CModelParameterSet.h"
#include "model/CModel.h"
#include "copasi/core/CRootContainer.h"
#include "core/CDataString.h"

#include "copasiui3window.h"

CQParameterOverviewWidget::CQParameterOverviewWidget(QWidget* parent, const char* name):
  CopasiWidget(parent, name),
  mpParameterSet(NULL),
  mpParameterSetCopy(NULL),
  mpParameterSetDM(NULL),
  mpParameterSetSortDM(NULL),
  mGlobalQuantities()
{
  setupUi(this);
  mpBtnWidget->hide();

  // create a new QListview to be displayed on the screen..and set its property
  mpParameterSetDM = new CQParameterOverviewDM(this);
  mpParameterSetSortDM = new QSortFilterProxyModel(this);

  mpParameterSetSortDM->setSourceModel(mpParameterSetDM);
  mpParameterSetSortDM->setSortRole(Qt::EditRole);
  mpParameterSetSortDM->setSortCaseSensitivity(Qt::CaseInsensitive);
  // pSortModel->sort(0, Qt::AscendingOrder);

  mpTreeView->setModel(mpParameterSetSortDM);
  mpTreeView->sortByColumn(0, Qt::AscendingOrder);
  mpTreeView->setUniformRowHeights(false);

  CQPushButtonDelegate * pPushButtonDelegate = new CQPushButtonDelegate(CQPushButtonDelegate::PushButton, this);
  mpTreeView->setItemDelegateForColumn(1, pPushButtonDelegate);
  connect(pPushButtonDelegate, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotResolve(const QModelIndex &)));

  mpComboDelegate = new CQComboDelegate(this);
  mpTreeView->setItemDelegateForColumn(5, mpComboDelegate);

  connect(mpParameterSetDM, SIGNAL(signalNotifyChanges(const CUndoData::ChangeSet &)),
          this, SLOT(slotNotifyChanges(const CUndoData::ChangeSet &)));
  connect(mpParameterSetDM, SIGNAL(signalOpenEditor(const QModelIndex &)),
          this, SLOT(slotOpenEditor(const QModelIndex &)));
  connect(mpParameterSetDM, SIGNAL(signalCloseEditor(const QModelIndex &)),
          this, SLOT(slotCloseEditor(const QModelIndex &)));
}

CQParameterOverviewWidget::~CQParameterOverviewWidget()
{
  // TODO Auto-generated destructor stub
}

// virtual
bool CQParameterOverviewWidget::updateProtected(ListViews::ObjectType objectType, ListViews::Action action, const CCommonName & cn)
{
  if (!mIgnoreUpdates)
    {
      if (mpParameterSetCopy != NULL &&
          mpParameterSetCopy->isActive() &&
          (objectType == ListViews::REACTION ||
           ((action == ListViews::ADD ||
             action == ListViews::DELETE) &&
            (objectType == ListViews::COMPARTMENT ||
             objectType == ListViews::METABOLITE ||
             objectType == ListViews::MODELVALUE))))
        {
          mpParameterSetDM->beginResetModel();

          if (objectType == ListViews::MODELVALUE)
            {
              buildSelectionList();
            }

          mpParameterSetDM->endResetModel();

          mpTreeView->expandAll();

          for (int i = 0; i < 6; i++)
            {
              mpTreeView->resizeColumnToContents(i);
            }
        }
      else if (objectType == ListViews::MODELVALUE &&
               action == ListViews::CHANGE)
        {
          mpParameterSetDM->resetCache();

          if (mpParameterSet != NULL &&
              mpParameterSet->isActive())
            {
              mpParameterSet->compile();
            }
        }
    }

  if (mIgnoreUpdates || !isVisible())
    {
      if (objectType == ListViews::MODEL && action == ListViews::DELETE)
        {
          mObjectCN.clear();
          mpObject = NULL;

          mpParameterSetDM->setModelParameterSet(NULL);

          if (mpParameterSet != mpParameterSetCopy)
            {
              pdelete(mpParameterSetCopy);
            }

          mpParameterSet = NULL;
        }

      return true;
    }

  switch (objectType)
    {
      case ListViews::STATE:
        enterProtected();
        break;

      case ListViews::MODEL:

        if (action == ListViews::ADD ||
            action == ListViews::DELETE)
          {
            mObjectCN.clear();
            mpObject = NULL;

            enterProtected();
          }

        break;

      case ListViews::PARAMETEROVERVIEW:
      case ListViews::MODELPARAMETERSET:

        if (mObjectCN == cn)
          {
            switch (action)
              {
                case ListViews::DELETE:
                  mObjectCN.clear();
                  mpObject = NULL;

                  enterProtected();
                  break;

                case ListViews::CHANGE:
                  enterProtected();
                  break;

                default:
                  break;
              }
          }

        break;

      default:
        break;
    }

  return true;
}

CQBaseDataModel * CQParameterOverviewWidget::getCqDataModel()
{
  return mpParameterSetDM;
}

// virtual
bool CQParameterOverviewWidget::leaveProtected()
{
  if (mpParameterSet == NULL ||
      mpParameterSetCopy == NULL)
    {
      return true;
    }

  // The changes of the active parameter set are directly propagated to the model through mpParameterSetDM.
  if (!mpParameterSet->isActive() &&
      mpParameterSet->diff(*mpParameterSetCopy,
                           static_cast< CCore::Framework >(mFramework),
                           false) != CModelParameter::CompareResult::Identical)
    {
      CData OldData = mpParameterSet->toData();
      mpParameterSet->assignSetContent(*mpParameterSetCopy, false);

      CUndoData Data;
      mpParameterSet->createUndoData(Data, CUndoData::Type::CHANGE, OldData, static_cast< CCore::Framework >(mFramework));

      slotNotifyChanges(mpDataModel->recordData(Data));
    }

  return true;
}

// virtual
void CQParameterOverviewWidget::setFramework(int framework)
{
  CopasiWidget::setFramework(framework);
  mpParameterSetDM->setFramework(framework);

  if (mpParameterSet == NULL)
    {
      return;
    }

  if (!mpParameterSet->isActive())
    {
      mpParameterSetCopy->compareWithModel(static_cast< CCore::Framework >(mFramework));
    }

  mpTreeView->expandAll();
  mpTreeView->resizeColumnToContents(3);
}

// virtual
bool CQParameterOverviewWidget::enterProtected()
{
  CModelParameterSet * pOldParameterSet = (mpParameterSet != mpParameterSetCopy) ? mpParameterSetCopy : NULL;

  mpParameterSet = dynamic_cast< CModelParameterSet * >(mpObject);

  if (mpParameterSet == NULL)
    {
      mpParameterSetDM->setModelParameterSet(NULL);
      pdelete(pOldParameterSet);

      return false;
    }

  // We need to make sure the original is fully compiled.
  mpParameterSet->compile();

  mGlobalQuantities.clear();

  if (mpParameterSet->isActive())
    {
      mpParameterSetCopy = mpParameterSet;
      mpTreeView->header()->hideSection(1);
    }
  else
    {
      mpParameterSetCopy = new CModelParameterSet(*mpParameterSet, mpDataModel, false);
      mpParameterSetCopy->compareWithModel(static_cast< CCore::Framework >(mFramework));
      mpHeaderWidget->hide();
    }

  buildSelectionList();

  mpParameterSetDM->setModelParameterSet(mpParameterSetCopy);
  pdelete(pOldParameterSet);

  mpTreeView->expandAll();

  for (int i = 0; i < 6; i++)
    {
      mpTreeView->resizeColumnToContents(i);
    }

  return true;
}

void CQParameterOverviewWidget::buildSelectionList()
{
  // We build the selection for the global parameters for kinetic constants.
  const CModelParameterGroup *pGlobalQuantities =
    static_cast< const CModelParameterGroup * >(mpParameterSetCopy->getModelParameter(CDataString("Initial Global Quantities").getCN()));

  CModelParameterGroup::const_iterator it = pGlobalQuantities->begin();
  CModelParameterGroup::const_iterator end = pGlobalQuantities->end();

  mGlobalQuantities.clear();
  mGlobalQuantities.append("");

  for (; it != end; ++it)
    {
      if ((*it)->getCompareResult() != CModelParameter::CompareResult::Missing)
        {
          mGlobalQuantities.append(FROM_UTF8((*it)->getName()));
        }
    }

  mpComboDelegate->setItems(-1, mGlobalQuantities);
}

// virtual
void CQParameterOverviewWidget::slotBtnRevert()
{
  mpBtnRevert->setFocus();
  enterProtected();
}

// virtual
void CQParameterOverviewWidget::slotBtnCommit()
{
  mpBtnCommit->setFocus();
  leaveProtected();
  enterProtected();
}

// virtual
void CQParameterOverviewWidget::slotBtnDelete()
{
  if (mpParameterSet == NULL)
    {
      return;
    }

  CModel * pModel = mpParameterSet->getModel();

  if (pModel == NULL)
    {
      return;
    }

  mpParameterSetDM->setModelParameterSet(NULL);

  slotNotifyChanges(mpDataModel->applyData(CUndoData(CUndoData::Type::REMOVE, mpParameterSet)));
}

// virtual
void CQParameterOverviewWidget::slotBtnNew()
{
  // This button is shown as Apply and is used to assign the parameter values to the model.

  // commit all changes
  slotBtnCommit();

  // Sanity checks
  if (mpParameterSet == NULL)
    {
      return;
    }

  CModel * pModel = mpParameterSet->getModel();

  if (pModel == NULL)
    {
      return;
    }

  // We first asked whether the user wants to save the current model values
  QMessageBox::StandardButton answer = CQMessageBox::question(this, "Save current Model Parameters?",
                                       "You are about to overwrite the current model values.\n"
                                       "Do you want to save them?",
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                       QMessageBox::Save);

  if (answer == QMessageBox::Cancel)
    return;

  if (answer == QMessageBox::Save)
    {
      // Save the parameter set to a new or existing set
      saveParameterSet(&pModel->getActiveModelParameterSet());
    }

  // TODO CRITICAL We need to record all changes to the model
  mpParameterSet->updateModel();

  // Notify the GUI that the model state has changed.
  protectedNotify(ListViews::STATE, ListViews::CHANGE, pModel->getCN());

  enterProtected();
}

void CQParameterOverviewWidget::slotBtnCopy()
{
  // commit all changes
  slotBtnCommit();

  // Sanity checks
  if (mpParameterSet == NULL)
    {
      return;
    }

  CModel * pModel = mpParameterSet->getModel();

  if (pModel == NULL)
    {
      return;
    }

  CDataVectorN< CModelParameterSet > & Sets = pModel->getModelParameterSets();
  std::string Name = "Parameter Set";
  int i = 0;

  while (Sets.getIndex(Name) != C_INVALID_INDEX)
    {
      i++;
      Name = "Parameter Set ";
      Name += TO_UTF8(QString::number(i));
    }

  CModelParameterSet * pNew = new CModelParameterSet(pModel->getActiveModelParameterSet(), pModel, false);
  pNew->setObjectName(Name);
  Sets.add(pNew, true);

  slotNotifyChanges(mpDataModel->recordData(CUndoData(CUndoData::Type::INSERT, pNew)));

  mpListView->switchToOtherWidget(C_INVALID_INDEX, pNew->CDataObject::getCN());
}

// virtual
void CQParameterOverviewWidget::slotBtnSaveToFile()
{
  // Commit all changes
  slotBtnCommit();

  C_INT32 Answer = QMessageBox::No;
  QString fileName;

  while (Answer == QMessageBox::No)
    {
      fileName =
        CopasiFileDialog::getSaveFileName(this, "Save File Dialog",
                                          "untitled.tsv", "Tab Separated Files (*.tsv);;Comma Separated Files (*.csv);;TEXT Files (*.txt)", "Save as");

      if (fileName.isEmpty()) return;

      // Checks whether the file exists
      Answer = checkSelection(fileName);

      if (Answer == QMessageBox::Cancel) return;
    }

  std::ofstream file(CLocaleString::fromUtf8(TO_UTF8(fileName)).c_str());

  if (file.fail()) return;

  std::string mode;
  std::string separator;

  if (fileName.endsWith(".txt"))
    {
      mode = "report";
      separator = "\t";
    }
  else if (fileName.endsWith(".csv"))
    {
      mode = "table";
      separator = ",";
    }
  else
    {
      mode = "table";
      separator = "\t";
    }

  mpParameterSet->saveToStream(file, static_cast< CCore::Framework >(mFramework), mode, separator);

  file.close();
}

// virtual
void CQParameterOverviewWidget::slotBtnSaveAs()
{
  CModelParameterSet * pParameterSet = mpParameterSet;
  saveParameterSet(pParameterSet);
  return;
}

void CQParameterOverviewWidget::saveParameterSet(CModelParameterSet * pParameterSet)
{
  // commit all changes
  slotBtnCommit();

  if (pParameterSet == NULL)
    {
      return;
    }

  CModel * pModel = pParameterSet->getModel();

  if (pModel == NULL)
    {
      return;
    }

  // Save the parameter set to a new or existing set
  CQNameSelectionDialog Dialog(this);

  CDataVectorN< CModelParameterSet > & Sets = pModel->getModelParameterSets();

  CDataVectorN< CModelParameterSet >::const_iterator it = Sets.begin();
  CDataVectorN< CModelParameterSet >::const_iterator end = Sets.end();
  QStringList SelectionList;

  QString defaultName = QString("Parameter Set %1").arg(LocalTimeStamp().c_str());

  while (Sets.getIndex(TO_UTF8(defaultName)) != C_INVALID_INDEX)
    defaultName = QString("Parameter Set %1").arg(LocalTimeStamp().c_str());

  SelectionList << defaultName;

  for (; it != end; ++it)
    {
      SelectionList.append(FROM_UTF8(it->getName()));
    }

  Dialog.setSelectionList(SelectionList);

  if (Dialog.exec() == QDialog::Rejected)
    {
      return;
    }

  QString Name = Dialog.getSelection();

  if (Name == "")
    {
      return;
    }

  if (SelectionList.indexOf(Name) <= 0)
    {
      CModelParameterSet * pNew = new CModelParameterSet(pModel->getActiveModelParameterSet(), pModel, false);
      pNew->setObjectName(TO_UTF8(Name));

      // We are sure that a set with that name does not exist.
      Sets.add(pNew, true);

      slotNotifyChanges(mpDataModel->recordData(CUndoData(CUndoData::Type::INSERT, pNew)));
    }
  else
    {
      if (CQMessageBox::question(this, "Overwrite Parameter Set",
                                 QString("Are you sure you want to overwrite the parameter set %1").arg(Name),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
          CModelParameterSet * pExisting = &Sets[TO_UTF8(Name)];
          CData OldData = pExisting->toData();
          pExisting->assignSetContent(pModel->getActiveModelParameterSet(), false);

          CUndoData Data;
          pExisting->createUndoData(Data, CUndoData::Type::CHANGE, OldData, static_cast< CCore::Framework >(mFramework));

          slotNotifyChanges(mpDataModel->recordData(Data));
        }
    }
}

void CQParameterOverviewWidget::slotOpenEditor(const QModelIndex & index)
{
  QModelIndex Tmp = index;
  const QAbstractItemModel *pModel = Tmp.model();

  while (pModel->inherits("QSortFilterProxyModel"))
    {
      Tmp = static_cast< const QSortFilterProxyModel *>(pModel)->mapToSource(index);
      pModel = Tmp.model();
    }

  mpTreeView->openPersistentEditor(mpParameterSetSortDM->mapFromSource(Tmp));
}

void CQParameterOverviewWidget::slotCloseEditor(const QModelIndex & index)
{
  QModelIndex Tmp = index;
  const QAbstractItemModel *pModel = Tmp.model();

  while (pModel->inherits("QSortFilterProxyModel"))
    {
      Tmp = static_cast< const QSortFilterProxyModel *>(pModel)->mapToSource(index);
      pModel = Tmp.model();
    }

  mpTreeView->closePersistentEditor(Tmp);
}

void CQParameterOverviewWidget::slotResolve(const QModelIndex & index)
{
  CModelParameter * pModelParameter = mpParameterSetDM->nodeFromIndex(index);

  if (pModelParameter == NULL)
    {
      return;
    }

  pModelParameter->refreshFromModel(true);

  mpParameterSetCopy->compareWithModel(static_cast< CCore::Framework >(mFramework));

  buildSelectionList();

  mpParameterSetDM->setFramework(mFramework);

  mpTreeView->expandAll();
  mpTreeView->resizeColumnToContents(3);
}
