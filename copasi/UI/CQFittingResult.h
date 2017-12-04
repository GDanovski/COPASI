// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
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

// Copyright (C) 2006 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/*
 *  CQFittingResult.h
 *  Created by Paul on 4/2/10.
 */

#ifndef CQFITTINGRESULT_H
#define CQFITTINGRESULT_H

#include <QtCore/QVariant>

#include "copasi/UI/ui_CQFittingResult.h"

class CFitTask;
class CFitProblem;
class COptMethod;

class CQFittingResult : public CopasiWidget, public Ui::CQFittingResult
{
  Q_OBJECT

public:
  CQFittingResult(QWidget* parent = 0, const char* name = 0);
  ~CQFittingResult();

  virtual bool update(ListViews::ObjectType objectType, ListViews::Action action, const std::string & key);
  virtual bool leave();

protected:
  virtual bool enterProtected();
  void loadLog(const COptMethod * pMethod);

  void loadExperimentTab();

protected slots:

private:
  const CFitTask * mpTask;
  const CFitProblem * mpProblem;

  void init();

private slots:
  void slotSave(void);
  void loadTab(int);
  void slotUpdateModel();
};

#endif // CQFITTINGRESULT_H
