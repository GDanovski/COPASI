// Begin CVS Header
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/MIRIAM/CRDFTriplet.h,v $
//   $Revision: 1.1 $
//   $Name:  $
//   $Author: shoops $
//   $Date: 2008/06/05 15:34:56 $
// End CVS Header

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

#ifndef COPASI_CRDFTriplet
#define COPASI_CRDFTriplet

#include "copasi/MIRIAM/CRDFPredicate.h"

class CRDFNode;

class CRDFTriplet
  {
  public:
    // Methods
    CRDFTriplet(CRDFNode * pSubject = NULL,
                const CRDFPredicate & predicate = CRDFPredicate::end,
                CRDFNode * pObject = NULL);

    ~CRDFTriplet();

    operator bool() const;

    bool operator == (const CRDFTriplet & rhs) const;

    bool operator < (const CRDFTriplet & rhs) const;

    // Attributes
    CRDFNode * pSubject;

    CRDFPredicate Predicate;

    CRDFNode * pObject;
  };

#endif /*CRDFTRIPLET_H_*/
