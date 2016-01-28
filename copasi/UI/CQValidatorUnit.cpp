// Copyright (C) 2015 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include "sstream"
#include "qtUtilities.h"
#include "CQValidatorUnit.h"
#include "utilities/CCopasiException.h"

CQValidatorUnit::CQValidatorUnit(QLineEdit * parent, const char * name):
  CQValidator< QLineEdit >(parent, &QLineEdit::text, name)
{
}

QValidator::State CQValidatorUnit::validate(QString & input, int & pos) const
{
  if (input.isEmpty())
    return CQValidator< QLineEdit >::validate(input, pos);

  State CurrentState = Invalid;

  bool success = false;
  std::istringstream buffer(TO_UTF8(input));
  CUnitParser Parser(&buffer);

  try
  {
    success = (Parser.yyparse() == 0);
  }
  catch (CCopasiException & /*exception*/)
  {
    success = false;
  }

  if (success)
    {
//      setColor(Acceptable);
//      CurrentState = Acceptable;
      return CQValidator< QLineEdit >::validate(input, pos);
    }
  else
    {
      setColor(Invalid);
      CurrentState = Intermediate;
    }

  return CurrentState;
}
