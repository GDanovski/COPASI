// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

// Copyright (C) 2015 - 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include "CQSpectogramWidget.h"

#include "copasi/UI/CCopasiSelectionDialog.h"
#include "CQPlotEditWidget.h"

#include "copasi/copasi.h"

#include "copasi/UI/qtUtilities.h"

#include "copasi/core/CRootContainer.h"
#include "copasi/utilities/CCopasiException.h"
#include "copasi/plot/CPlotItem.h"
#include "copasi/resourcesUI/CQIconResource.h"
#include "copasi/model/CModel.h"
#include "copasi/CopasiDataModel/CDataModel.h"

/**
 * In multiple edit mode, we don't want to edit name & channels
 */
void CQSpectogramWidget::setMultipleEditMode(bool mode)
{
  mpEditTitle->setEnabled(!mode);
  mpEditX->setEnabled(!mode);
  mpEditY->setEnabled(!mode);
  mpEditZ->setEnabled(!mode);
  mpBtnX->setEnabled(!mode);
  mpBtnY->setEnabled(!mode);
  mpBtnZ->setEnabled(!mode);
}

/*
 *  Constructs a CQSpectogramWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
CQSpectogramWidget::CQSpectogramWidget(QWidget* parent, const char* /* name */, Qt::WindowFlags fl):
  CQPlotEditWidget(parent, fl),
  mpObjectX(NULL),
  mpObjectY(NULL),
  mpObjectZ(NULL)
{
  setupUi(this);

  mpBtnX->setIcon(CQIconResource::icon(CQIconResource::copasi));
  mpBtnY->setIcon(CQIconResource::icon(CQIconResource::copasi));
  mpBtnZ->setIcon(CQIconResource::icon(CQIconResource::copasi));
}

/*
 *  Destroys the object and frees any allocated resources
 */
CQSpectogramWidget::~CQSpectogramWidget()
{
  // no need to delete child widgets, Qt does it all for us
}

bool
CQSpectogramWidget::LoadFromCurveSpec(const CPlotItem * pCurve)
{
  if (!pCurve)
    {
      // We need to reset the widget to defaults
      mpEditTitle->setText("");

      mpObjectX = mpObjectY = mpObjectZ = NULL;

      mpEditX->setText("");
      mpEditY->setText("");
      mpEditZ->setText("");

      mpCheckBefore->setChecked(false);
      mpCheckDuring->setChecked(true);
      mpCheckAfter->setChecked(false);

      return true;
    }

  if (pCurve->getType() != CPlotItem::spectogram) return false;

  //if (pCurve->getChannels().getSize != 3) return false;

  mpEditTitle->setText(FROM_UTF8(pCurve->getTitle()));

  //TODO: check if objects exist....
  CDataModel* pDataModel = mpModel->getObjectDataModel();
  assert(pDataModel != NULL);
  mpObjectX = mpObjectY = mpObjectZ = NULL;

  if (pCurve->getChannels().size() >= 1)
    mpObjectX = dynamic_cast<const CDataObject*>(pDataModel->getObject(pCurve->getChannels()[0]));

  if (pCurve->getChannels().size() >= 2)
    mpObjectY = dynamic_cast<const CDataObject*>(pDataModel->getObject(pCurve->getChannels()[1]));

  if (pCurve->getChannels().size() >= 3)
    {
      mpObjectZ = dynamic_cast<const CDataObject*>(pDataModel->getObject(pCurve->getChannels()[2]));

      if ((mpObjectZ == NULL || mpObjectZ->getObjectDisplayName() == "(CN)Root") && mpObjectY)
        mpObjectZ = mpObjectY;  // as long as we haven't a second Y-axis chooser, this has to suffice.
    }

  if (mpObjectX)
    mpEditX->setText(FROM_UTF8(mpObjectX->getObjectDisplayName()));
  else
    mpEditX->clear();

  if (mpObjectY)
    mpEditY->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()));
  else
    mpEditY->clear();

  if (mpObjectZ)
    mpEditZ->setText(FROM_UTF8(mpObjectZ->getObjectDisplayName()));
  else
    mpEditZ->clear();

  CPlotItem* pItem = const_cast<CPlotItem*>(pCurve);

  mpLogZ->setChecked(*pItem->assertParameter("logZ", CCopasiParameter::Type::BOOL, false));
  mpBilinear->setChecked(*pItem->assertParameter("bilinear", CCopasiParameter::Type::BOOL, true));
  std::string *contours = pItem->assertParameter("contours", CCopasiParameter::Type::STRING, std::string(""));
  mpContours->setText(FROM_UTF8(*contours));
  std::string *maxZ = pItem->assertParameter("maxZ", CCopasiParameter::Type::STRING, std::string(""));
  mpMaxZ->setText(FROM_UTF8(*maxZ));
  std::string *colorMap = pItem->assertParameter("colorMap", CCopasiParameter::Type::STRING, std::string("Default"));
  mpColorMap->setCurrentIndex(mpColorMap->findText(FROM_UTF8(*colorMap)));

  mpCheckBefore->setChecked(pCurve->getActivity() & COutputInterface::BEFORE);
  mpCheckDuring->setChecked(pCurve->getActivity() & COutputInterface::DURING);
  mpCheckAfter->setChecked(pCurve->getActivity() & COutputInterface::AFTER);

  return true;
}

bool
CQSpectogramWidget::SaveToCurveSpec(CPlotItem * curve, const CPlotItem *original /*= NULL*/) const
{
  curve->setType(CPlotItem::spectogram);

  std::string title = TO_UTF8(mpEditTitle->text());

  CCommonName xName = mpObjectX ? mpObjectX->getCN() : CCommonName("");
  CCommonName yName = mpObjectY ? mpObjectY->getCN() : CCommonName("");
  CCommonName zName = mpObjectZ ? mpObjectZ->getCN() : CCommonName("");

  C_INT32 Activity = 0;

  if (mpCheckBefore->isChecked()) Activity += COutputInterface::BEFORE;

  if (mpCheckDuring->isChecked()) Activity += COutputInterface::DURING;

  if (mpCheckAfter->isChecked()) Activity += COutputInterface::AFTER;

  bool thingsChanged = false;

  if (original != NULL)
    {
      // compare whether things changed
      if (original->getTitle() != title)
        thingsChanged = true;

      if (thingsChanged || original->getType() != CPlotItem::spectogram)
        thingsChanged = true;

      if (thingsChanged || original->getActivity() != Activity)
        thingsChanged = true;

      if (thingsChanged || original->getChannels().size() != 3)
        thingsChanged = true;

      if (thingsChanged || original->getChannels()[0] != xName)
        thingsChanged = true;

      if (thingsChanged || original->getChannels()[1] != yName)
        thingsChanged = true;

      if (thingsChanged || original->getChannels()[2] != zName)
        thingsChanged = true;

      if (thingsChanged || *curve->assertParameter("contours", CCopasiParameter::Type::STRING, std::string("")) != TO_UTF8(mpContours->text()))
        thingsChanged = true;

      if (thingsChanged || *curve->assertParameter("maxZ", CCopasiParameter::Type::STRING, std::string("")) != TO_UTF8(mpMaxZ->text()))
        thingsChanged = true;

      if (thingsChanged || *curve->assertParameter("colorMap", CCopasiParameter::Type::STRING, std::string("")) != TO_UTF8(mpColorMap->currentText()))
        thingsChanged = true;

      if (thingsChanged || *curve->assertParameter("logZ", CCopasiParameter::Type::BOOL, false) != mpLogZ->isChecked())
        thingsChanged = true;

      if (thingsChanged || *curve->assertParameter("bilinear", CCopasiParameter::Type::BOOL, true) != mpBilinear->isChecked())
        thingsChanged = true;
    }
  else thingsChanged = true;

  if (!thingsChanged)
    return false;

  //title
  curve->setTitle(title);

  //channels
  curve->getChannels().clear();
  curve->getChannels().push_back(CPlotDataChannelSpec(xName));
  curve->getChannels().push_back(CPlotDataChannelSpec(yName));
  curve->getChannels().push_back(CPlotDataChannelSpec(zName));

  curve->setActivity((COutputInterface::Activity) Activity);

  bool* pLogZ = curve->assertParameter("logZ", CCopasiParameter::Type::BOOL, false);
  bool* pBilinear = curve->assertParameter("bilinear", CCopasiParameter::Type::BOOL, true);
  std::string* pContours = curve->assertParameter("contours", CCopasiParameter::Type::STRING, std::string(""));
  std::string* pMaxZ = curve->assertParameter("maxZ", CCopasiParameter::Type::STRING, std::string(""));
  std::string* pColorMap = curve->assertParameter("colorMap", CCopasiParameter::Type::STRING, std::string("Default"));

  *pLogZ = mpLogZ->isChecked();
  *pBilinear = mpBilinear->isChecked();

  *pContours = TO_UTF8(mpContours->text());
  *pMaxZ = TO_UTF8(mpMaxZ->text());
  *pColorMap = TO_UTF8(mpColorMap->currentText());

  return true;
}

void
CQSpectogramWidget::buttonPressedX()
{
  if (!mpModel) return;

  mpObjectX = CCopasiSelectionDialog::getObjectSingle(this,
              CQSimpleSelectionTree::NumericValues,
              mpObjectX);

  if (mpObjectX)
    {
      mpEditX->setText(FROM_UTF8(mpObjectX->getObjectDisplayName()));

      if (mpObjectY)
        {
          if (mpObjectZ && (mpObjectZ != mpObjectY))
            mpEditTitle->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()
                                           + "/"
                                           + mpObjectZ->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
          else
            mpEditTitle->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
        }

      //TODO update tab title
    }
  else
    mpEditX->setText("");
}

void
CQSpectogramWidget::buttonPressedY()
{
  if (!mpModel) return;

  mpObjectY = CCopasiSelectionDialog::getObjectSingle(this,
              CQSimpleSelectionTree::NumericValues,
              mpObjectY);

  if (mpObjectY)
    {
      mpEditY->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()));

      if (mpObjectX)
        {
          if (mpObjectZ && (mpObjectZ != mpObjectY))
            mpEditTitle->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()
                                           + "/"
                                           + mpObjectZ->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
          else
            mpEditTitle->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
        }

      //TODO update tab title
    }
  else
    mpEditY->setText("");
}

void
CQSpectogramWidget::buttonPressedZ()
{
  if (!mpModel) return;

  mpObjectZ = CCopasiSelectionDialog::getObjectSingle(this,
              CQSimpleSelectionTree::NumericValues,
              mpObjectZ);

  if (mpObjectZ)
    {
      mpEditZ->setText(FROM_UTF8(mpObjectZ->getObjectDisplayName()));

      if (mpObjectX)
        {
          if (mpObjectY && (mpObjectY != mpObjectZ))
            mpEditTitle->setText(FROM_UTF8(mpObjectY->getObjectDisplayName()
                                           + "/"
                                           + mpObjectZ->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
          else
            mpEditTitle->setText(FROM_UTF8(mpObjectZ->getObjectDisplayName()
                                           + "|"
                                           + mpObjectX->getObjectDisplayName()));
        }

      //TODO update tab title
    }
  else
    mpEditZ->setText("");
}
