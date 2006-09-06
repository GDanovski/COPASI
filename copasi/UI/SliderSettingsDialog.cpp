/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/UI/SliderSettingsDialog.cpp,v $
   $Revision: 1.14 $
   $Name:  $
   $Author: gauges $
   $Date: 2006/09/06 09:04:47 $
   End CVS Header */

// Copyright � 2006 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/****************************************************************************
 ** Form implementation generated from reading ui file 'SliderSettingsDialog.ui'
 **
 ** Created: Wed Sep 6 10:57:58 2006
 **      by: The User Interface Compiler ($Id: SliderSettingsDialog.cpp,v 1.14 2006/09/06 09:04:47 gauges Exp $)
 **
 ** WARNING! All changes made in this file will be lost!
 ****************************************************************************/

#include "SliderSettingsDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "SliderSettingsDialog.ui.h"

/*
 *  Constructs a SliderSettingsDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SliderSettingsDialog::SliderSettingsDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
  if (!name)
    setName("SliderSettingsDialog");
  SliderSettingsDialogLayout = new QVBoxLayout(this, 11, 6, "SliderSettingsDialogLayout");
  SliderSettingsDialogLayout->setResizeMode(QLayout::Fixed);

  mpLayout11 = new QHBoxLayout(0, 0, 6, "mpLayout11");

  mpObjectLabel = new QLabel(this, "mpObjectLabel");
  mpObjectLabel->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, mpObjectLabel->sizePolicy().hasHeightForWidth()));
  mpLayout11->addWidget(mpObjectLabel);

  mpObjectNameLineEdit = new QLineEdit(this, "mpObjectNameLineEdit");
  mpObjectNameLineEdit->setEnabled(FALSE);
  mpObjectNameLineEdit->setFrameShape(QLineEdit::LineEditPanel);
  mpObjectNameLineEdit->setFrameShadow(QLineEdit::Sunken);
  mpObjectNameLineEdit->setReadOnly(TRUE);
  mpLayout11->addWidget(mpObjectNameLineEdit);

  mpObjectBrowseButton = new QPushButton(this, "mpObjectBrowseButton");
  mpObjectBrowseButton->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, mpObjectBrowseButton->sizePolicy().hasHeightForWidth()));
  mpObjectBrowseButton->setMinimumSize(QSize(40, 0));
  mpLayout11->addWidget(mpObjectBrowseButton);
  SliderSettingsDialogLayout->addLayout(mpLayout11);

  layout5 = new QHBoxLayout(0, 0, 6, "layout5");

  mpMinValueLabel = new QLabel(this, "mpMinValueLabel");
  layout5->addWidget(mpMinValueLabel);

  mpMinValueEdit = new QLineEdit(this, "mpMinValueEdit");
  mpMinValueEdit->setAlignment(int(QLineEdit::AlignRight));
  layout5->addWidget(mpMinValueEdit);
  mpSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
  layout5->addItem(mpSpacer1);

  mpMaxValueLabel = new QLabel(this, "mpMaxValueLabel");
  layout5->addWidget(mpMaxValueLabel);

  mpMaxValueEdit = new QLineEdit(this, "mpMaxValueEdit");
  mpMaxValueEdit->setAlignment(int(QLineEdit::AlignRight));
  layout5->addWidget(mpMaxValueEdit);
  SliderSettingsDialogLayout->addLayout(layout5);

  layout10 = new QHBoxLayout(0, 0, 6, "layout10");

  mpLogCheckBox = new QCheckBox(this, "mpLogCheckBox");
  layout10->addWidget(mpLogCheckBox);
  mpSpacer1_2_2 = new QSpacerItem(200, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
  layout10->addItem(mpSpacer1_2_2);
  SliderSettingsDialogLayout->addLayout(layout10);

  layout9 = new QHBoxLayout(0, 0, 6, "layout9");

  mpExtendedOptionsButton = new QPushButton(this, "mpExtendedOptionsButton");
  layout9->addWidget(mpExtendedOptionsButton);
  spacer6 = new QSpacerItem(300, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  layout9->addItem(spacer6);
  SliderSettingsDialogLayout->addLayout(layout9);

  mpExtendedOptionsFrame = new QFrame(this, "mpExtendedOptionsFrame");
  mpExtendedOptionsFrame->setFrameShape(QFrame::NoFrame);
  mpExtendedOptionsFrame->setFrameShadow(QFrame::Raised);
  mpExtendedOptionsFrameLayout = new QGridLayout(mpExtendedOptionsFrame, 1, 1, 11, 6, "mpExtendedOptionsFrameLayout");

  mpNumMinorTicksLabel = new QLabel(mpExtendedOptionsFrame, "mpNumMinorTicksLabel");

  mpExtendedOptionsFrameLayout->addWidget(mpNumMinorTicksLabel, 0, 0);

  mpMinorMajorFactorLabel = new QLabel(mpExtendedOptionsFrame, "mpMinorMajorFactorLabel");

  mpExtendedOptionsFrameLayout->addWidget(mpMinorMajorFactorLabel, 1, 0);

  mpObjectValueLabel = new QLabel(mpExtendedOptionsFrame, "mpObjectValueLabel");
  mpObjectValueLabel->setMargin(0);

  mpExtendedOptionsFrameLayout->addWidget(mpObjectValueLabel, 2, 0);

  mpNumMinorTicksEdit = new QLineEdit(mpExtendedOptionsFrame, "mpNumMinorTicksEdit");
  mpNumMinorTicksEdit->setAlignment(int(QLineEdit::AlignRight));

  mpExtendedOptionsFrameLayout->addWidget(mpNumMinorTicksEdit, 0, 1);

  mpObjectValueEdit = new QLineEdit(mpExtendedOptionsFrame, "mpObjectValueEdit");
  mpObjectValueEdit->setAlignment(int(QLineEdit::AlignRight));

  mpExtendedOptionsFrameLayout->addWidget(mpObjectValueEdit, 2, 1);
  mpSpacer1_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
  mpExtendedOptionsFrameLayout->addItem(mpSpacer1_3, 1, 2);
  mpSpacer1_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
  mpExtendedOptionsFrameLayout->addItem(mpSpacer1_2, 0, 2);
  mpSpacer1_3_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  mpExtendedOptionsFrameLayout->addItem(mpSpacer1_3_2, 2, 3);

  mpMinorMajorFactorEdit = new QLineEdit(mpExtendedOptionsFrame, "mpMinorMajorFactorEdit");
  mpMinorMajorFactorEdit->setAlignment(int(QLineEdit::AlignRight));

  mpExtendedOptionsFrameLayout->addWidget(mpMinorMajorFactorEdit, 1, 1);

  mpMinorTickSizeLabel = new QLabel(mpExtendedOptionsFrame, "mpMinorTickSizeLabel");

  mpExtendedOptionsFrameLayout->addWidget(mpMinorTickSizeLabel, 0, 3);

  mpOriginalValueEdit = new QLineEdit(mpExtendedOptionsFrame, "mpOriginalValueEdit");
  mpOriginalValueEdit->setAlignment(int(QLineEdit::AlignRight));

  mpExtendedOptionsFrameLayout->addWidget(mpOriginalValueEdit, 1, 4);

  mpMinorTickSizeEdit = new QLineEdit(mpExtendedOptionsFrame, "mpMinorTickSizeEdit");
  mpMinorTickSizeEdit->setAlignment(int(QLineEdit::AlignRight));

  mpExtendedOptionsFrameLayout->addWidget(mpMinorTickSizeEdit, 0, 4);

  mpOriginalValueLabel = new QLabel(mpExtendedOptionsFrame, "mpOriginalValueLabel");

  mpExtendedOptionsFrameLayout->addWidget(mpOriginalValueLabel, 1, 3);
  SliderSettingsDialogLayout->addWidget(mpExtendedOptionsFrame);

  mpLayout2 = new QHBoxLayout(0, 0, 6, "mpLayout2");

  mpOkButton = new QPushButton(this, "mpOkButton");
  mpOkButton->setDefault(TRUE);
  mpLayout2->addWidget(mpOkButton);

  mpCancelButton = new QPushButton(this, "mpCancelButton");
  mpLayout2->addWidget(mpCancelButton);
  SliderSettingsDialogLayout->addLayout(mpLayout2);
  languageChange();
  resize(QSize(728, 387).expandedTo(minimumSizeHint()));
  clearWState(WState_Polished);

  // signals and slots connections
  connect(mpCancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
  connect(mpOkButton, SIGNAL(clicked()), this, SLOT(okButtonPressed()));
  connect(mpObjectValueEdit, SIGNAL(returnPressed()), this, SLOT(objectValueChanged()));
  connect(mpObjectValueEdit, SIGNAL(lostFocus()), this, SLOT(objectValueChanged()));
  connect(mpObjectValueEdit, SIGNAL(textChanged(const QString&)), this, SLOT(valueTextChanged()));
  connect(mpMinorTickSizeEdit, SIGNAL(returnPressed()), this, SLOT(minorTickSizeChanged()));
  connect(mpMinorTickSizeEdit, SIGNAL(lostFocus()), this, SLOT(minorTickSizeChanged()));
  connect(mpMinorTickSizeEdit, SIGNAL(textChanged(const QString&)), this, SLOT(tickSizeTextChanged()));
  connect(mpMinorMajorFactorEdit, SIGNAL(textChanged(const QString&)), this, SLOT(tickFactorTextChanged()));
  connect(mpNumMinorTicksEdit, SIGNAL(returnPressed()), this, SLOT(numMinorTicksChanged()));
  connect(mpNumMinorTicksEdit, SIGNAL(lostFocus()), this, SLOT(numMinorTicksChanged()));
  connect(mpNumMinorTicksEdit, SIGNAL(textChanged(const QString&)), this, SLOT(numTicksTextChanged()));
  connect(mpMinValueEdit, SIGNAL(lostFocus()), this, SLOT(minValueChanged()));
  connect(mpMinValueEdit, SIGNAL(textChanged(const QString&)), this, SLOT(minValueTextChanged()));
  connect(mpMaxValueEdit, SIGNAL(lostFocus()), this, SLOT(maxValueChanged()));
  connect(mpMaxValueEdit, SIGNAL(textChanged(const QString&)), this, SLOT(maxValueTextChanged()));
  connect(mpObjectBrowseButton, SIGNAL(clicked()), this, SLOT(browseButtonPressed()));
  connect(mpExtendedOptionsButton, SIGNAL(clicked()), this, SLOT(extendedOptionsClicked()));
  connect(mpLogCheckBox, SIGNAL(toggled(bool)), this, SLOT(logCheckBoxToggled(bool)));
  connect(mpOriginalValueEdit, SIGNAL(textChanged(const QString&)), this, SLOT(origValueTextChanged()));
  connect(mpOriginalValueEdit, SIGNAL(lostFocus()), this, SLOT(originalValueChanged()));
  connect(mpMinValueEdit, SIGNAL(returnPressed()), this, SLOT(minValueChanged()));

  // tab order
  setTabOrder(mpObjectNameLineEdit, mpObjectBrowseButton);
  setTabOrder(mpObjectBrowseButton, mpMinValueEdit);
  setTabOrder(mpMinValueEdit, mpMaxValueEdit);
  setTabOrder(mpMaxValueEdit, mpLogCheckBox);
  setTabOrder(mpLogCheckBox, mpExtendedOptionsButton);
  setTabOrder(mpExtendedOptionsButton, mpNumMinorTicksEdit);
  setTabOrder(mpNumMinorTicksEdit, mpMinorTickSizeEdit);
  setTabOrder(mpMinorTickSizeEdit, mpMinorMajorFactorEdit);
  setTabOrder(mpMinorMajorFactorEdit, mpObjectValueEdit);
  setTabOrder(mpObjectValueEdit, mpOkButton);
  setTabOrder(mpOkButton, mpCancelButton);
  init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
SliderSettingsDialog::~SliderSettingsDialog()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SliderSettingsDialog::languageChange()
{
  setCaption(tr("Slider Settings"));
  mpObjectLabel->setText(tr("Object:"));
  mpObjectBrowseButton->setText(tr("choose object"));
  QToolTip::add(mpObjectBrowseButton, tr("Select an object for the slider"));
  QWhatsThis::add(mpObjectBrowseButton, tr("Clicking this button opens a dialog that\n"
                   "lets the user choose the object that the \n"
                   "slider will manipulate. The object has to \n"
                   "represent either an integer value or a float\n"
                   "value."));
  mpMinValueLabel->setText(tr("Minimum Value:"));
  QToolTip::add(mpMinValueLabel, tr("Lowest value the slider can have"));
  QWhatsThis::add(mpMinValueLabel, tr("Here the user can enter the lowest value the slider will take.\n"
                                        "For logarithmic sliders this has to be a positive value greater\n"
                                        "than 0.0."));
  mpMinValueEdit->setText(tr("0.0"));
  mpMaxValueLabel->setText(tr("Maximum Value:"));
  QToolTip::add(mpMaxValueLabel, tr("Maximum value the slider will take."));
  QWhatsThis::add(mpMaxValueLabel, tr("Here the user can enter the maximal value that\n"
                                        "the slider will take."));
  mpMaxValueEdit->setText(tr("100.0"));
  QWhatsThis::add(mpMaxValueEdit, QString::null);
  mpLogCheckBox->setText(tr("logarithmic"));
  QToolTip::add(mpLogCheckBox, tr("Wether slider has logarithmic scale."));
  QWhatsThis::add(mpLogCheckBox, tr("If checked, the slider will have a \n"
                                      "logarithmic scale. Minimum\n"
                                      "and maximum value must be \n"
                                      "greater than 0.0."));
  mpExtendedOptionsButton->setText(tr("more options"));
  QToolTip::add(mpExtendedOptionsButton, tr("Displays or hides an extended set of options."));
  QWhatsThis::add(mpExtendedOptionsButton, tr("Clicking this button either displays or hides\n"
                   "some more less often needed options."));
  mpNumMinorTicksLabel->setText(tr("Number of Minor Ticks:"));
  QToolTip::add(mpNumMinorTicksLabel, tr("Number of steps for the slider."));
  QWhatsThis::add(mpNumMinorTicksLabel, tr("This value determines how many steps\n"
                   "the slider makes from minimum to \n"
                   "maximum value."));
  mpMinorMajorFactorLabel->setText(tr("Ticksize factor:"));
  QToolTip::add(mpMinorMajorFactorLabel, tr("how many minor ticks make a major tick"));
  QWhatsThis::add(mpMinorMajorFactorLabel, tr("This value determines how many minor \n"
                   "ticks make up a major tick. Minor ticks\n"
                   "are small steps on the slider whereas major\n"
                   "ticks are larger steps.\n"
                   "The user can make a minor step via the cursor\n"
                   "keys and a major step via the page up/down keys."));
  mpObjectValueLabel->setText(tr("Object Value:"));
  QToolTip::add(mpObjectValueLabel, tr("The value of the underlying object."));
  QWhatsThis::add(mpObjectValueLabel, tr("Here the user can change the actual value of the object\n"
                   "when editing or creating a slider."));
  mpNumMinorTicksEdit->setText(tr("10.0"));
  mpObjectValueEdit->setText(tr("50.0"));
  mpMinorMajorFactorEdit->setText(tr("10.0"));
  mpMinorTickSizeLabel->setText(tr("Minor Ticksize:"));
  QToolTip::add(mpMinorTickSizeLabel, tr("Value change corresponding to a minor step"));
  QWhatsThis::add(mpMinorTickSizeLabel, tr("This value signifies the value change that\n"
                   "corresponds to one minor step on the slider.\n"
                   "The user can either set this directly or indirectly\n"
                   "when setting the \"Number of Minor Ticks\"."));
  mpOriginalValueEdit->setText(tr("1.0"));
  mpMinorTickSizeEdit->setText(tr("1.0"));
  mpOriginalValueLabel->setText(tr("Original Value"));
  QToolTip::add(mpOriginalValueLabel, tr("Value change corresponding to a minor step"));
  QWhatsThis::add(mpOriginalValueLabel, tr("This value signifies the value change that\n"
                   "corresponds to one minor step on the slider.\n"
                   "The user can either set this directly or indirectly\n"
                   "when setting the \"Number of Minor Ticks\"."));
  mpOkButton->setText(tr("OK"));
  mpCancelButton->setText(tr("Cancel"));
}
