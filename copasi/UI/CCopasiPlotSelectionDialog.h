// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/CCopasiPlotSelectionDialog.h,v $
//   $Revision: 1.5 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2007/03/09 21:16:51 $
// End CVS Header

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef CCopasiPlotSelectionDialog_H__
#define CCopasiPlotSelectionDialog_H__

#include <vector>

#include <qdialog.h>

#include "UI/CCopasiSimpleSelectionTree.h"

class QWidget;
class QPushButton;
class QSplitter;
class QCheckBox;
class QVBox;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class CCopasiObject;
class CModel;
class CCopasiSelectionWidget;

class CCopasiPlotSelectionDialog: public QDialog
  {
    Q_OBJECT

  public:
    CCopasiPlotSelectionDialog(QWidget * parent = 0, const char * name = 0, bool modal = FALSE, WFlags f = 0);
    ~CCopasiPlotSelectionDialog();
    void setOutputVectors(std::vector<CCopasiObject*>* outputVector1, std::vector<CCopasiObject*>* outputVector2);
    void setModel(CModel* model,
                  const CCopasiSimpleSelectionTree::SelectionFlag & flag);

  protected slots:
    void slotOKButtonClicked();
    void slotCancelButtonClicked();
    void slotExpertCheckBoxToggled(bool checked);

  protected:
    void setTabOrder();

  protected:
    QPushButton* mpOKButton;
    QPushButton* mpCancelButton;
    QCheckBox* mpExpertCheckBox;
    CCopasiSelectionWidget* mpXAxisSelectionWidget;
    CCopasiSelectionWidget* mpYAxisSelectionWidget;
    QSplitter* mpSplitter;
    QHBoxLayout* mpButtonBox;
    QVBoxLayout* mpMainLayout;
    QLabel* mpXAxisLabel;
    QLabel* mpYAxisLabel;
    QVBox* mpXAxisSelectionBox;
    QVBox* mpYAxisSelectionBox;
    std::vector<CCopasiObject*>* mpXAxisOutputVector;
    std::vector<CCopasiObject*>* mpYAxisOutputVector;
  };

#endif // CPlotSelectionDialog_H__
