// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2006 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef CQMODELVALUE_H
#define CQMODELVALUE_H

class UndoGlobalQuantityData;
#include <limits>
#include <copasi/undoFramework/CCopasiUndoCommand.h>

class CQExpressionWidget;
class CModelValue;
class CExpression;

#include "ui_CQModelValue.h"

#include <string>

#include "copasiWidget.h"

class CQModelValue : public CopasiWidget, public Ui::CQModelValue
{
  Q_OBJECT

  friend class DeleteGlobalQuantityCommand;
  friend class CreateNewGlobalQuantityCommand;

public:
  CQModelValue(QWidget* parent = 0, const char* name = 0);
  ~CQModelValue();

  virtual bool leave();
  virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);

protected:
  virtual bool enterProtected();

protected slots:

private:
  std::vector< int > mItemToType;
  CModelValue * mpModelValue;
  bool mChanged;
  bool mExpressionValid;
  bool mInitialExpressionValid;

  void init();
  void destroy();
  void load();
  void save();
  std::string mKeyToCopy;

private slots:
  void slotBtnNew();
  void slotBtnCopy();
  void slotBtnDelete();
  void slotTypeChanged(int type);
  void slotExpressionValid(bool valid);
  void slotInitialExpressionValid(bool valid);
  void slotInitialTypeChanged(bool useInitialAssignment);

  //additional functions for UNDO framework
  void deleteGlobalQuantity();
  void addGlobalQuantity(UndoGlobalQuantityData *pSData);
  void createNewGlobalQuantity();
  void deleteGlobalQuantity(UndoGlobalQuantityData *pSData);

public:
  bool changeValue(const std::string& key,
                   CCopasiUndoCommand::Type type,
                   const QVariant& newValue,
                   double iValue = std::numeric_limits<double>::quiet_NaN());
};

#endif // CQMODELVALUE_H
