// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/Attic/CQModelValue.ui.h,v $
//   $Revision: 1.8.4.1 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2007/02/01 18:56:30 $
// End CVS Header

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you want to add, delete, or rename functions or slots, use
 ** Qt Designer to update this file, preserving your code.
 **
 ** You should not define a constructor or destructor in this file.
 ** Instead, write your code in functions called init() and destroy().
 ** These will automatically be called by the form's constructor and
 ** destructor.
 *****************************************************************************/

#include <qmessagebox.h>

#include "UI/qtUtilities.h"

#include "CopasiDataModel/CCopasiDataModel.h"
#include "model/CModel.h"
#include "model/CModelValue.h"
#include "function/CExpression.h"
#include "report/CKeyFactory.h"

void CQModelValue::slotBtnCommit()
{
  save();
  load();
}

void CQModelValue::slotBtnRevert()
{
  load();
}

void CQModelValue::slotBtnNew()
{
  save();

  std::string name = "quantity";
  int i = 0;

  while (!(mpModelValue = CCopasiDataModel::Global->getModel()->createModelValue(name)))
    {
      i++;
      name = "quantity_";
      name += (const char *) QString::number(i).utf8();
    }

  enter(mpModelValue->getKey());
  protectedNotify(ListViews::MODELVALUE, ListViews::ADD);
}

void CQModelValue::slotBtnDelete()
{
  CModel * pModel = CCopasiDataModel::Global->getModel();
  if (pModel == NULL)
    return;

  QString valueList = "Are you sure you want to delete listed MODEL VALUE(S) ?\n";
  QString effectedCompartmentList = "Following COMPARTMENT(S) reference above MODEL VALUE(S) and will be deleted -\n";
  QString effectedMetabList = "Following METABOLITE(S) reference above MODEL VALUE(S) and will be deleted -\n";
  QString effectedReacList = "Following REACTION(S) reference above MODEL VALUE(S) and will be deleted -\n";
  QString effectedValueList = "Following MODEL VALUE(S) reference above MODEL VALUE(S) and will be deleted -\n";

  bool compartmentFound = false;
  bool metabFound = false;
  bool reacFound = false;
  bool valueFound = false;

  valueList.append(FROM_UTF8(mpModelValue->getObjectName()));
  valueList.append(", ");

  std::set< const CCopasiObject * > Reactions;
  pModel->appendDependentReactions(mpModelValue->getDeletedObjects(), Reactions);

  if (Reactions.size() > 0)
    {
      reacFound = true;
      std::set< const CCopasiObject * >::const_iterator it, itEnd = Reactions.end();
      for (it = Reactions.begin(); it != itEnd; ++it)
        {
          effectedReacList.append(FROM_UTF8((*it)->getObjectName()));
          effectedReacList.append(", ");
        }

      effectedReacList.remove(effectedReacList.length() - 2, 2);
      effectedReacList.append("  ---> ");
      effectedReacList.append(FROM_UTF8(mpModelValue->getObjectName()));
      effectedReacList.append("\n");
    }

  std::set< const CCopasiObject * > Metabolites;
  pModel->appendDependentMetabolites(mpModelValue->getDeletedObjects(), Metabolites);

  if (Metabolites.size() > 0)
    {
      metabFound = true;
      std::set< const CCopasiObject * >::const_iterator it, itEnd = Metabolites.end();
      for (it = Metabolites.begin(); it != itEnd; ++it)
        {
          effectedMetabList.append(FROM_UTF8((*it)->getObjectName()));
          effectedMetabList.append(", ");
        }

      effectedMetabList.remove(effectedMetabList.length() - 2, 2);
      effectedMetabList.append("  ---> ");
      effectedMetabList.append(FROM_UTF8(mpModelValue->getObjectName()));
      effectedMetabList.append("\n");
    }

  std::set< const CCopasiObject * > Values;
  pModel->appendDependentModelValues(mpModelValue->getDeletedObjects(), Values);

  if (Values.size() > 0)
    {
      valueFound = true;
      std::set< const CCopasiObject * >::const_iterator it, itEnd = Values.end();
      for (it = Values.begin(); it != itEnd; ++it)
        {
          effectedValueList.append(FROM_UTF8((*it)->getObjectName()));
          effectedValueList.append(", ");
        }

      effectedValueList.remove(effectedValueList.length() - 2, 2);
      effectedValueList.append("  ---> ");
      effectedValueList.append(FROM_UTF8(mpModelValue->getObjectName()));
      effectedValueList.append("\n");
    }

  std::set< const CCopasiObject * > Compartments;
  pModel->appendDependentCompartments(mpModelValue->getDeletedObjects(), Compartments);

  if (Compartments.size() > 0)
    {
      compartmentFound = true;
      std::set< const CCopasiObject * >::const_iterator it, itEnd = Compartments.end();
      for (it = Compartments.begin(); it != itEnd; ++it)
        {
          effectedCompartmentList.append(FROM_UTF8((*it)->getObjectName()));
          effectedCompartmentList.append(", ");
        }

      effectedCompartmentList.remove(effectedCompartmentList.length() - 2, 2);
      effectedCompartmentList.append("  ---> ");
      effectedCompartmentList.append(FROM_UTF8(mpModelValue->getObjectName()));
      effectedCompartmentList.append("\n");
    }

  valueList.remove(valueList.length() - 2, 2);

  QString msg = valueList;

  if (compartmentFound)
    {
      msg.append("\n \n");
      msg.append(effectedCompartmentList);
    }

  if (metabFound)
    {
      msg.append("\n \n");
      msg.append(effectedMetabList);
    }

  if (reacFound)
    {
      msg.append("\n \n");
      msg.append(effectedReacList);
    }

  if (valueFound)
    {
      msg.append("\n \n");
      msg.append(effectedValueList);
    }

  C_INT32 choice;
  if (metabFound || reacFound || valueFound || valueFound)
    choice = QMessageBox::warning(this,
                                  "CONFIRM DELETE",
                                  msg,
                                  "Continue", "Cancel", 0, 0, 1);
  else
    choice = 0;

  switch (choice)
    {
    case 0:                                                     // Yes or Enter
      {
        unsigned C_INT32 index =
          static_cast<CCopasiVector< CModelValue > *>(&CCopasiDataModel::Global->getModel()->getModelValues())->getIndex(GlobalKeys.get(mKey));

        CCopasiDataModel::Global->getModel()->removeModelValue(mKey);
        unsigned C_INT32 size =
          CCopasiDataModel::Global->getModel()->getModelValues().size();

        mpModelValue = NULL;

        if (size > 0)
          enter(CCopasiDataModel::Global->getModel()->getModelValues()[std::min(index, size - 1)]->getKey());
        else
          enter("");

#undef DELETE
        protectedNotify(ListViews::MODELVALUE, ListViews::DELETE, mKey);
        break;
      }
    case 1:                                                     // No or Escape
      break;
    }
}

void CQModelValue::slotTypeChanged(int type)
{
  switch ((CModelEntity::Status) mItemToType[type])
    {
    case CModelEntity::FIXED:
      mpLblExpression->hide();
      mpEditExpression->hide();
      mpBtnObject->hide();
      mpEditInitialValue->setEnabled(true);

      mpEditInitialValue->setText(QString::number(mpModelValue->getInitialValue()));
      break;

    case CModelEntity::ASSIGNMENT:
      mpLblExpression->show();
      mpEditExpression->show();
      mpBtnObject->show();
      mpEditInitialValue->setEnabled(false);

      mpEditInitialValue->setText("nan");
      mpEditExpression->setExpression(mpModelValue->getExpression());
      break;

    case CModelEntity::ODE:
      mpLblExpression->show();
      mpEditExpression->show();
      mpBtnObject->show();
      mpEditInitialValue->setEnabled(true);

      mpEditInitialValue->setText(QString::number(mpModelValue->getInitialValue()));
      mpEditExpression->setExpression(mpModelValue->getExpression());
      break;
    }
}

void CQModelValue::slotExpressionValid(bool valid)
{
  mpBtnCommit->setEnabled(valid);
}

void CQModelValue::init()
{
  mpComboBoxType->insertItem(FROM_UTF8(CModelEntity::StatusName[CModelEntity::FIXED]));
  mpComboBoxType->insertItem(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ASSIGNMENT]));
  mpComboBoxType->insertItem(FROM_UTF8(CModelEntity::StatusName[CModelEntity::ODE]));

  mItemToType.push_back(CModelEntity::FIXED);
  mItemToType.push_back(CModelEntity::ASSIGNMENT);
  mItemToType.push_back(CModelEntity::ODE);
}

void CQModelValue::destroy()
{}

bool CQModelValue::update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key)
{
  return true;
}

bool CQModelValue::leave()
{
  if (mpBtnCommit->isEnabled())
    save();

  return true;
}

bool CQModelValue::enter(const std::string & key)
{
  mKey = key;
  mpModelValue = dynamic_cast< CModelValue * >(GlobalKeys.get(key));

  if (!mpModelValue) return false;

  load();
  return true;
}

void CQModelValue::load()
{
  if (mpModelValue == NULL) return;

  mpEditName->setText(FROM_UTF8(mpModelValue->getObjectName()));

  mpComboBoxType->setCurrentText(FROM_UTF8(CModelEntity::StatusName[mpModelValue->getStatus()]));
  slotTypeChanged(mpComboBoxType->currentItem());

  mpEditCurrentValue->setText(QString::number(mpModelValue->getValue()));

  mpEditExpression->setExpression(mpModelValue->getExpression());

  mChanged = false;
}

void CQModelValue::save()
{
  if (mpModelValue == NULL) return;

  if (mpModelValue->getObjectName() != (const char *) mpEditName->text().utf8())
    {
      if (!mpModelValue->setObjectName((const char *) mpEditName->text().utf8()))
        {
          QString msg;
          msg = "Unable to rename quantity '" + FROM_UTF8(mpModelValue->getObjectName()) + "'\n"
                + "to '" + mpEditName->text() + "' since a quantity with that name already exists.\n";

          QMessageBox::warning(this,
                               "Unable to rename Quantity",
                               msg,
                               QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

          mpEditName->setText(FROM_UTF8(mpModelValue->getObjectName()));
        }
      else
        {
          protectedNotify(ListViews::MODELVALUE, ListViews::RENAME, mKey);
          mChanged = true;
        }
    }

  if (mpModelValue->getStatus() != (CModelEntity::Status) mItemToType[mpComboBoxType->currentItem()])
    {
      mpModelValue->setStatus((CModelEntity::Status) mItemToType[mpComboBoxType->currentItem()]);
      mChanged = true;
    }

  if (QString::number(mpModelValue->getInitialValue()) != mpEditInitialValue->text() &&
      mpModelValue->getStatus() != CModelEntity::ASSIGNMENT)
    {
      mpModelValue->setInitialValue(mpEditInitialValue->text().toDouble());
      mChanged = true;
    }

  if (mpModelValue->getExpression() != mpEditExpression->getExpression())
    {
      mpModelValue->setExpression(mpEditExpression->getExpression());
      mChanged = true;
    }

  if (mChanged) CCopasiDataModel::Global->changed();

  mChanged = false;
}

void CQModelValue::slotNameLostFocus()
{
  if (mpEditName->text() != FROM_UTF8(mpModelValue->getObjectName()))
    mpEditExpression->currentObjectRenamed(mpModelValue, mpEditName->text());
}
