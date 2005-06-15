/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/function/CExpression.cpp,v $
   $Revision: 1.1 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2005/06/15 19:09:28 $
   End CVS Header */

#include "copasi.h"

#include "CExpression.h"

CExpression::CExpression(const std::string & name,
                         const CCopasiContainer * pParent):
    CCopasiContainer(name, pParent, "Expression"),
    CEvaluationTree(),
    mpListOfContainer(NULL)
{}

CExpression::CExpression(const CExpression & src,
                         const CCopasiContainer * pParent):
    CCopasiContainer(src, pParent),
    CEvaluationTree(src),
    mpListOfContainer(NULL)
{}

CExpression::~CExpression() {}

bool CExpression::compile(std::vector< CCopasiContainer * > listOfContainer)
{
  mpListOfContainer = & listOfContainer;
  return compileNodes();
}

C_FLOAT64 CExpression::calcValue()
{return mpRoot->value();}

C_FLOAT64 * CExpression::getObjectValue(const CCopasiObjectName & CN) const
  {return (C_FLOAT64 *) CCopasiContainer::ObjectFromName(*mpListOfContainer, CN)->getReference();}
