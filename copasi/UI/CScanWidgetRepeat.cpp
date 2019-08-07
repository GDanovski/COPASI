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

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2005 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#include <QValidator>

#include "copasi/copasi.h"

#include "CScanWidgetRepeat.h"
#include "qtUtilities.h"
#include "CCopasiSelectionDialog.h"

#include "copasi/core/CRegisteredCommonName.h"

/*
 *  Constructs a CScanWidgetRepeat as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CScanWidgetRepeat::CScanWidgetRepeat(QWidget* parent) :
  QWidget(parent),
  CScanItemData(CScanProblem::SCAN_REPEAT)
{
  setupUi(this);
  init();
}

CScanWidgetRepeat::CScanWidgetRepeat(const CScanWidgetRepeat & src, QWidget * parent) :
  QWidget(parent),
  CScanItemData(src)
{
  setupUi(this);
  init();
  load(mpData);
}

/*
 *  Destroys the object and frees any allocated resources
 */
CScanWidgetRepeat::~CScanWidgetRepeat()
{
  // no need to delete child widgets, Qt does it all for us
}

void CScanWidgetRepeat::init()
{
  lineEditNumber->setValidator(new QIntValidator(lineEditNumber));
}

// virtual
void CScanWidgetRepeat::load(const CCopasiParameterGroup * pItem)
{
  if (pItem == NULL) return;

  *mpData = *pItem;

  if (mpData->getValue< CScanProblem::Type >("Type") != CScanProblem::SCAN_REPEAT)
    return;

  lineEditNumber->setText(QString::number(mpData->getValue< C_INT32 >("Number of steps")));

  return;
}

// virtual
bool CScanWidgetRepeat::save(CCopasiParameterGroup * pItem) const
{
  mpData->setValue("Number of steps", lineEditNumber->text().toUInt());

  if (pItem != NULL)
    {
      if (*mpData == *pItem) return false;

      *pItem = *mpData;
      return true;
    }

  return false;
}
