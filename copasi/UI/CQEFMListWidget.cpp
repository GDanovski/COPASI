// Copyright (C) 2019 by Pedro Mendes, Rector and Visitors of the
// University of Virginia, University of Heidelberg, and University
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2017 - 2018 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

// Copyright (C) 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#include "CQEFMListWidget.h"

#include "copasi/elementaryFluxModes/CEFMTask.h"
#include <copasi/core/CRootContainer.h>
#include <copasi/commandline/CConfigurationFile.h>

CQEFMListWidget::CQEFMListWidget(QWidget *parent, const char *name) :
  QWidget(parent),
  mpTask(NULL),
  mpProxyModel(NULL),
  mpFluxModeDM(NULL)
{
  setObjectName(QString::fromUtf8(name));
  setupUi(this);
  mpEFMTable->verticalHeader()->hide();

  if (CRootContainer::getConfiguration()->resizeToContents())
    {
#if QT_VERSION >= 0x050000
      mpEFMTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
      mpEFMTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    }

  mpEFMTable->sortByColumn(COL_ROW_NUMBER, Qt::AscendingOrder);
  //Create Source Data Model.
  mpFluxModeDM = new CQFluxModeDM(this);
  //Create the Proxy Model for sorting/filtering and set its properties.
  mpProxyModel = new CQSortFilterProxyModel();
  mpProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
  mpProxyModel->setFilterKeyColumn(-1);
  mpProxyModel->setSourceModel(mpFluxModeDM);
  //Set Model for the TableView
  mpEFMTable->setModel(NULL);
  mpEFMTable->setModel(mpProxyModel);

  if (CRootContainer::getConfiguration()->resizeToContents())
    {
      mpEFMTable->resizeColumnsToContents();
    }

  connect(mpEditFilter, SIGNAL(textChanged(const QString &)), this, SLOT(slotFilterChanged()));
}

CQEFMListWidget::~CQEFMListWidget()
{
  // TODO Auto-generated destructor stub
  pdelete(mpProxyModel);
}

bool CQEFMListWidget::loadResult(const CEFMTask *pTask)
{
  mpTask = pTask;
  mpFluxModeDM->setTask(mpTask);
  mpProxyModel->setSourceModel(mpFluxModeDM);
  //Set Model for the TableView
  mpEFMTable->setModel(NULL);
  mpEFMTable->setModel(mpProxyModel);

  if (CRootContainer::getConfiguration()->resizeToContents())
    {
      mpEFMTable->resizeColumnsToContents();
    }

  return true;
}

void CQEFMListWidget::slotFilterChanged()
{
  QRegExp regExp(mpEditFilter->text(), Qt::CaseInsensitive, QRegExp::RegExp);
  mpProxyModel->setFilterRegExp(regExp);
}
