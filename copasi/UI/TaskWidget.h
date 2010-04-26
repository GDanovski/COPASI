/* Begin CVS Header
 $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/TaskWidget.h,v $
 $Revision: 1.24 $
 $Name:  $
 $Author: aekamal $
 $Date: 2010/04/26 14:26:13 $
 End CVS Header */

// Copyright (C) 2010 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include "copasi.h"
#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
/*
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3Frame>
 */
#include <QGridLayout>
#include <QTableWidget>
#include <QLabel>

#include "utilities/CCopasiMethod.h"
#include "UI/copasiWidget.h"

class CQTaskThread;
//class Q3VBoxLayout;
//class Q3HBoxLayout;
//class Q3GridLayout;
class QCheckBox;
class QComboBox;
//class Q3Frame;
class QLabel;
class QLineEdit;
class QPushButton;
//class Q3Table;
class QSpacerItem;

class CQTaskHeaderWidget;
class CQTaskBtnWidget;

class CCopasiTask;

//class CModel;
class CProgressBar;

class CopasiPlot;
class CCopasiException;

class TaskWidget : public CopasiWidget
{
  Q_OBJECT

public:
  TaskWidget(QWidget* parent = 0,
             const char* name = 0,
             Qt::WFlags fl = 0);

  ~TaskWidget();

  //the TaskWidget interface
  virtual bool update(ListViews::ObjectType objectType,
                      ListViews::Action action,
                      const std::string & key);

  virtual bool leave();

  virtual bool runTask() = 0;

  CCopasiTask* getTask();

protected slots:
  void runBtnClicked();

  void revertBtnClicked();

  void reportBtnClicked();

  void assistantBtnClicked();

  void changeMethod(int);

public slots:

  void adjustTable();

  void slotExceptionOccured(CCopasiException* pException);

  void slotFinishThread();

protected:
  virtual bool enterProtected();
  CProgressBar * mProgressBar;

  virtual bool loadTask() = 0;

  virtual bool saveTask() = 0;

  virtual CCopasiMethod * createMethod(const CCopasiMethod::SubType & type) = 0;

  //these methods should be called by the loadTask() or saveTask() methods
  //of the derived classes. They handle the "executable" checkbox
  bool loadCommon();
  bool saveCommon();

  //these methods should be called by the constructor, loadTask(), or saveTask() methods
  //of the derived classes. They handle the method parameters table
  void addMethodSelectionBox(const unsigned C_INT32 * validMethods, unsigned int row);
  void addMethodParameterTable(unsigned int row);

  //convenience functions
  void addHeaderToGrid(unsigned int row);
//    bool addHLineToGrid(Q3GridLayout* grid, unsigned int row, unsigned int maxcol);
  bool addHLineToGrid(QGridLayout* grid, unsigned int row, unsigned int maxcol);

  bool loadMethod();
  bool saveMethod();

  //this method should be called at the beginning or the end of the runTask() method
  //of the derived classes, respectively
  bool commonBeforeRunTask();
  bool commonAfterRunTask();

  bool commonRunTask();
  void finishTask();

  CQTaskHeaderWidget * mpHeaderWidget;
  CQTaskBtnWidget * mpBtnWidget;

//    Q3GridLayout * mpMethodLayout;
  QGridLayout * mpMethodLayout;

  QLabel * mpLblParameter;
//    Q3Table * mpTblParameter;
  QTableWidget * mpTblParameter;
  QSpacerItem * mpSpacer1;
  QLabel * mpLblMethod;
  QComboBox * mpBoxMethod;
  QSpacerItem * mpSpacer2;

  CCopasiTask * mpTask;
  CCopasiMethod * mpMethod;

  bool mChanged;
  CQTaskThread *mpTaskThread;
};

#endif
