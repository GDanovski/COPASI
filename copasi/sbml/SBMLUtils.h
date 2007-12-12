// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/sbml/SBMLUtils.h,v $
//   $Revision: 1.3 $
//   $Name:  $
//   $Author: gauges $
//   $Date: 2007/12/12 09:10:54 $
// End CVS Header

// Copyright (C) 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef SBMLUTILS_H__
#define SBMLUTILS_H__

#include <string>
#include "copasi/copasi.h"

class SBase;

class SBMLUtils
  {
  public:

    /**
     * Return the sbml ID if the sbml object actually has one.
     * If not, it returns "".
     * It only handles core sbml objects. No layout objects yet.
     */
    static std::string getIdFromSBase(const SBase* s);

  protected:

    template<class CType>
    static std::string getId(const CType * pA, C_INT32 level = 2);
  };

#endif
