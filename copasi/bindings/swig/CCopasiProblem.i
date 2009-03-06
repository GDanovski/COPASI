// Begin CVS Header 
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/bindings/swig/CCopasiProblem.i,v $ 
//   $Revision: 1.6.20.1 $ 
//   $Name:  $ 
//   $Author: gauges $ 
//   $Date: 2009/03/06 13:36:26 $ 
// End CVS Header 

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc. and EML Research, gGmbH. 
// All rights reserved. 

// Copyright © 2005 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

%{

#include "utilities/CCopasiProblem.h"

%}

%ignore CCopasiProblem::CCopasiProblem(const CCopasiTask::Type& type, const CCopasiContainer* pParent);
%ignore CCopasiProblem::setCallback(CProcessReport* pCallBack);
%ignore CCopasiProblem::initialize();
%ignore CCopasiProblem::calculate();
%ignore CCopasiProblem::restore(const bool& updateModel);
%ignore CCopasiProblem::checkParametricConstraints();
%ignore CCopasiProblem::checkFunctionalConstraints();
%ignore CCopasiProblem::print(std::ostream* ostream) const;
%ignore CCopasiProblem::printResult(std::ostream* ostream) const;
%ignore operator<<(std::ostream& os,const CCopasiProblem& o);

%include "utilities/CCopasiProblem.h"

