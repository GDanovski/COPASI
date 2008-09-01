// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/CScanWidgetTask.h,v $
//   $Revision: 1.10 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2008/09/01 16:55:49 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

/****************************************************************************
 ** Form interface generated from reading ui file 'CScanWidgetTask.ui'
 **
 ** Created: Thu Jul 31 16:58:54 2008
 **      by: The User Interface Compiler ($Id: CScanWidgetTask.h,v 1.10 2008/09/01 16:55:49 shoops Exp $)
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/

#ifndef CSCANWIDGETTASK_H
#define CSCANWIDGETTASK_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QComboBox;
class QCheckBox;
class CModel;
class CCopasiObject;
class CScanProblem;

class CScanWidgetTask : public QWidget
  {
    Q_OBJECT

  public:
    CScanWidgetTask(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~CScanWidgetTask();

    QFrame* frame;
    QLabel* labelTitle;
    QComboBox* comboType;
    QCheckBox* checkInitialConditions;
    QCheckBox* checkOutput;

    virtual bool initFromScanProblem(CScanProblem * pg, const CModel * model);
    virtual bool saveToScanProblem(CScanProblem * pg) const;

  protected:
    CCopasiObject* mpObject;
    const CModel* mpModel;

    QVBoxLayout* CScanWidgetTaskLayout;
    QVBoxLayout* frameLayout;
    QHBoxLayout* layout1;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;
    QHBoxLayout* layout2;
    QSpacerItem* spacer3;
    QSpacerItem* spacer4;

  protected slots:
    virtual void languageChange();

  private:
    void init();

  private slots:
    virtual void typeChanged(int n);
  };

#endif // CSCANWIDGETTASK_H
