// Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2003 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

// copasiWidget.cpp: implementation of the CopasiWidget class.
//
//////////////////////////////////////////////////////////////////////
#include <QApplication>

#include <QUndoStack>

#include "copasi/UI/copasiWidget.h"
#include "copasi/UI/listviews.h"
#include "copasi/UI/DataModelGUI.h"
#include "copasi/UI/copasiui3window.h"
#include "copasi/report/CKeyFactory.h"
#include "copasi/core/CRootContainer.h"

CopasiWidget::CopasiWidget(QWidget *parent, const char *name, Qt::WindowFlags f)
  : QWidget(parent, f),
    mpListView(NULL),
    mKey(),
    mpObject(NULL),
    mpDataModel(NULL),
    mIgnoreUpdates(false),
    mFramework(0)
{
  setObjectName(name);
  initContext();
}

bool CopasiWidget::update(ListViews::ObjectType C_UNUSED(objectType), ListViews::Action C_UNUSED(action), const std::string &C_UNUSED(key))
{return true;}

bool CopasiWidget::leave()
{return true;}

void CopasiWidget::refresh()
{
  leave();
  qApp->processEvents();
  enterProtected();
}

bool CopasiWidget::enter(const std::string &key)
{
  if (mpListView == NULL)
    {
      initContext();
    }

  mKey = key;
  mpObject = CRootContainer::getKeyFactory()->get(key);

  if (mpObject != NULL)
    {
      mpDataModel = mpObject->getObjectDataModel();
    }

  if (mpDataModel == NULL)
    {
      mpDataModel = mpListView->getDataModel();
    }

  return enterProtected();
}

const CDataObject * CopasiWidget::getObject() const
{
  return mpObject;
}

CQBaseDataModel * CopasiWidget::getCqDataModel()
{
  return NULL;
}

bool CopasiWidget::enterProtected()
{
  return true;
}

void CopasiWidget::setFramework(int framework)
{mFramework = framework;}

bool CopasiWidget::protectedNotify(ListViews::ObjectType objectType, ListViews::Action action, const std::string &key)
{
  bool notifyRun = false;

  if (!mIgnoreUpdates)
    {
      mIgnoreUpdates = true;
      mpListView->getDataModelGUI()->notify(objectType, action, key);
      notifyRun = true;
    }

  mIgnoreUpdates = false;
  return notifyRun;
}

bool CopasiWidget::getIgnoreUpdates()
{
  return mIgnoreUpdates;
}

void CopasiWidget::setIgnoreUpdates(bool v)
{
  mIgnoreUpdates = v;
}

CDataModel *CopasiWidget::getDataModel() const
{
  return mpDataModel;
}

void CopasiWidget::setUndoStack(QUndoStack *undoStack)
{
  mpUndoStack = undoStack;
}

QUndoStack *CopasiWidget::getUndoStack()
{
  return mpUndoStack;
}

const std::string& CopasiWidget::getKey() const
{
  return mKey;
}

void CopasiWidget::initContext()
{
  QObject *pParent = parent();

  if (pParent == NULL) return;

  if (mpListView == NULL)
    while (pParent != NULL &&
           (mpListView = dynamic_cast< ListViews * >(pParent)) == NULL)
      {
        pParent = pParent->parent();
      }

  if (mpDataModel == NULL &&
      mpListView != NULL)
    mpDataModel = mpListView->getDataModel();
}
