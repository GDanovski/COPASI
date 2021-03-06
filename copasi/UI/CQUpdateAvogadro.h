// Copyright (C) 2016 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef COPASI_CQUpdateAvogadro
#define COPASI_CQUpdateAvogadro

#include "copasi/UI/ui_CQUpdateAvogadro.h"

class CQUpdateAvogadro : public QDialog, public Ui::CQUpdateAvogadro
{
  Q_OBJECT

public:
  CQUpdateAvogadro(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
  virtual ~CQUpdateAvogadro();
  int getSelection() const;
  void setSelection(int selection);
};

#endif // COPASI_CQUpdateAvogadro
