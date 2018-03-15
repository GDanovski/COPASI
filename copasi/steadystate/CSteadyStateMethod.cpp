// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
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

// Copyright (C) 2002 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/**
 *  CSteadyStateMethod class.
 *  This class describes the interface to all steady state methods.
 *  The various method like Newton have to be derived from
 *  this class.
 *
 *  Created for COPASI by Stefan Hoops 2002
 */

#include "copasi.h"

#include "copasi/core/CDataVector.h"
#include "CSteadyStateMethod.h"
#include "CSteadyStateProblem.h"
#include "CSteadyStateTask.h"
#include "CEigen.h"

#include "math/CMathContainer.h"
#include "model/CModel.h"

/**
 *  Default constructor.
 */
CSteadyStateMethod::CSteadyStateMethod(const CDataContainer * pParent,
                                       const CTaskEnum::Method & methodType,
                                       const CTaskEnum::Task & taskType):
  CCopasiMethod(pParent, methodType, taskType),
  mpProblem(NULL),
  mContainerState(),
  mContainerStateReduced(),
  mpContainerStateTime(NULL),
  mCompartmentVolumes()
{
  initializeParameter();
  CONSTRUCTOR_TRACE;
}

/**
 *  Copy constructor.
 *  @param "const CSteadyStateMethod &" src
 */
CSteadyStateMethod::CSteadyStateMethod(const CSteadyStateMethod & src,
                                       const CDataContainer * pParent):
  CCopasiMethod(src, pParent),
  mpProblem(src.mpProblem),
  mContainerState(),
  mContainerStateReduced(),
  mpContainerStateTime(NULL),
  mCompartmentVolumes()
{
  initializeParameter();
  CONSTRUCTOR_TRACE;
}

/**
 *  Destructor.
 */
CSteadyStateMethod::~CSteadyStateMethod()
{DESTRUCTOR_TRACE;}

void CSteadyStateMethod::initializeParameter()
{
  CCopasiParameter *pParm;

  mpSSResolution = assertParameter("Resolution", CCopasiParameter::Type::UDOUBLE, (C_FLOAT64) 1.0e-009);
  mpDerivationResolution = mpSSResolution;

  mpDerivationFactor = assertParameter("Derivation Factor", CCopasiParameter::Type::UDOUBLE, (C_FLOAT64) 1.0e-003);

  // Check whether we have a method with the old parameter names
  if ((pParm = getParameter("Newton.DerivationFactor")) != NULL)
    {
      setValue("Derivation Factor", pParm->getValue< C_FLOAT64 >());
      removeParameter("Newton.DerivationFactor");
    }

  if ((pParm = getParameter("Newton.Resolution")) != NULL)
    {
      setValue("Resolution", pParm->getValue< C_FLOAT64 >());
      removeParameter("Newton.Resolution");
    }
}

bool CSteadyStateMethod::elevateChildren()
{
  initializeParameter();
  return true;
}

/**
 * This instructs the method to calculate a the steady state
 * starting with the initialState given.
 * The steady state is returned in the object pointed to by steadyState.
 * @param CState * steadyState
 * @param const CState * initialState
 * @param C_FLOAT64 * jacobian
 * @param CEigen * eigenValues
 * @return CSteadyStateMethod::ReturnCode returnCode
 */
CSteadyStateMethod::ReturnCode
CSteadyStateMethod::process(CVectorCore< C_FLOAT64 > & State,
                            CMatrix< C_FLOAT64 > & jacobianX,
                            CProcessReport * handler)
{
  mpParentTask = dynamic_cast<CSteadyStateTask *>(getObjectParent());
  assert(mpParentTask);

  mSteadyState.initialize(State);
  mpJacobian = & jacobianX;
  mpCallBack = handler;

  return processInternal();
}

/**
 * This function has to be called at the return of any implementation
 * of the protected function process
 * @param bool success
 * @param const C_FLOAT64 & factor
 * @param const C_FLOAT64 & resolution
 * @return CSteadyStateMethod::ReturnCode returnCode
 */
CSteadyStateMethod::ReturnCode
CSteadyStateMethod::returnProcess(bool steadyStateFound)
{
  mSteadyState = mpContainer->getState(true);

  if (!steadyStateFound)
    return CSteadyStateMethod::notFound;

  if (!allPositive())
    return CSteadyStateMethod::foundNegative;

  if (isEquilibrium(*mpSSResolution))
    return CSteadyStateMethod::foundEquilibrium;

  return CSteadyStateMethod::found;
}

/**
 * This instructs the method to calculate a the steady state
 * @return CSteadyStateMethod::ReturnCode returnCode
 */
CSteadyStateMethod::ReturnCode
CSteadyStateMethod::processInternal()
{return CSteadyStateMethod::notFound;}

bool CSteadyStateMethod::isEquilibrium(const C_FLOAT64 & resolution) const
{
  const CMathReaction * pReaction = mpContainer->getReactions().array();
  const CMathReaction * pReactionEnd = pReaction + mpContainer->getReactions().size();

  for (; pReaction != pReactionEnd; ++pReaction)
    {
      // We are checking the amount flux whether we have an equilibrium.
      // TODO CRITICAL Do we need to scale with the compartment size?
      if (* (C_FLOAT64 *) pReaction->getFluxObject()->getValuePointer() > resolution)
        {
          return false;
        }
    }

  return true;
}

bool CSteadyStateMethod::allPositive()
{
  mpContainer->updateSimulatedValues(true);

  const C_FLOAT64 * pValue = mContainerState.array();
  const C_FLOAT64 * pValueEnd = pValue + mContainerState.size();
  pValue += mpContainer->getCountFixedEventTargets() + 1; // + 1 for time

  const CMathObject * pValueObject = mpContainer->getMathObject(pValue);
  C_FLOAT64 ** ppCompartmentVolume = mCompartmentVolumes.array();

  // We need to check that all metabolites have positive particle numbers
  // with respect to the given resolution.
  C_FLOAT64 ParticleResolution = *mpDerivationResolution * mpContainer->getModel().getQuantity2NumberFactor();

  for (; pValue != pValueEnd; ++pValue, ++pValueObject, ++ppCompartmentVolume)
    {
      switch (pValueObject->getEntityType())
        {
          case CMath::EntityType::Compartment:

            if (*pValue < - *mpDerivationResolution)
              {
                return false;
              }

            break;

          case CMath::EntityType::Species:

            if (*pValue < - ParticleResolution ***ppCompartmentVolume)
              {
                return false;
              }

            break;

          // No restrictions on other values
          default:
            break;
        }
    }

  return true;
}

//virtual
bool CSteadyStateMethod::isValidProblem(const CCopasiProblem * pProblem)
{
  if (!CCopasiMethod::isValidProblem(pProblem)) return false;

  const CSteadyStateProblem * pP = dynamic_cast<const CSteadyStateProblem *>(pProblem);

  if (!pP)
    {
      //not a TrajectoryProblem
      CCopasiMessage(CCopasiMessage::EXCEPTION, "Problem is not a steady state problem.");
      return false;
    }

  return true;
}

bool CSteadyStateMethod::initialize(const CSteadyStateProblem * pProblem)
{
  mpProblem = pProblem;

  mContainerState.initialize(mpContainer->getState(false));
  mContainerStateReduced.initialize(mpContainer->getState(true));
  mpContainerStateTime = mContainerState.array() + mpContainer->getCountFixedEventTargets();

  return true;
}

void CSteadyStateMethod::doJacobian(CMatrix< C_FLOAT64 > & jacobian,
                                    CMatrix< C_FLOAT64 > & jacobianX)
{
  mpContainer->setState(mSteadyState);

  mpContainer->calculateJacobian(jacobian, *mpDerivationResolution, false);
  mpContainer->calculateJacobian(jacobianX, *mpDerivationResolution, true);
}

C_FLOAT64 CSteadyStateMethod::getStabilityResolution()
{
  C_FLOAT64* pTmp = &getValue< C_FLOAT64 >("Resolution");
  assert(pTmp);
  return *pTmp;
}

void CSteadyStateMethod::calculateJacobian(const C_FLOAT64 & oldMaxRate, const bool & reduced)
{
  if (reduced)
    {
      mpContainer->setState(mContainerStateReduced);
    }
  else
    {
      mpContainer->setState(mContainerState);
    }

  mpContainer->calculateJacobian(*mpJacobian, std::min(*mpDerivationFactor, oldMaxRate), reduced);
}

std::string CSteadyStateMethod::getMethodLog() const
{
  return mMethodLog.str();
}
