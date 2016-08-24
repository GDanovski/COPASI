// Copyright (C) 2010 - 2016 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., University of Heidelberg, and The University 
// of Manchester. 
// All rights reserved. 

// Copyright (C) 2008 - 2009 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

// Copyright (C) 2006 - 2007 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc. and EML Research, gGmbH. 
// All rights reserved. 

#include <cmath>

#include "copasi.h"

#include "COptMethodPS.h"
#include "COptProblem.h"
#include "COptItem.h"
#include "COptTask.h"

#include "randomGenerator/CRandom.h"
#include "randomGenerator/CPermutation.h"
#include "utilities/CProcessReport.h"
#include "utilities/CSort.h"
#include "report/CCopasiObjectReference.h"

COptMethodPS::COptMethodPS(const CCopasiContainer * pParent,
                           const CTaskEnum::Method & methodType,
                           const CTaskEnum::Task & taskType):
  COptPopulationMethod(pParent, methodType, taskType),
  mVariance(0.0),
  mVelocities(),
  mBestValues(),
  mBestPositions(),
  mpPermutation(NULL),
  mInformants(),
  mNumInformedMin(0),
  mNumInformed(0),
  mBestIndex(0),
  mEvaluationValue(0),
  mContinue(true)
{
  addParameter("Iteration Limit", CCopasiParameter::UINT, (unsigned C_INT32) 2000);
  addParameter("Swarm Size", CCopasiParameter::UINT, (unsigned C_INT32) 50);
  addParameter("Std. Deviation", CCopasiParameter::UDOUBLE, (C_FLOAT64) 1.0e-6);
  addParameter("Random Number Generator", CCopasiParameter::UINT, (unsigned C_INT32) CRandom::mt19937);
  addParameter("Seed", CCopasiParameter::UINT, (unsigned C_INT32) 0);

  initObjects();
}

COptMethodPS::COptMethodPS(const COptMethodPS & src,
                           const CCopasiContainer * pParent):
  COptPopulationMethod(src, pParent),
  mVariance(0.0),
  mVelocities(),
  mBestValues(),
  mBestPositions(),
  mpPermutation(NULL),
  mInformants(),
  mNumInformedMin(0),
  mNumInformed(0),
  mBestIndex(0),
  mEvaluationValue(0),
  mContinue(true)
{initObjects();}

COptMethodPS::~COptMethodPS()
{cleanup();}

// evaluate the fitness of one individual
const C_FLOAT64 & COptMethodPS::evaluate()
{
  // We do not need to check whether the parametric constraints are fulfilled
  // since the parameters are created within the bounds.

  // evaluate the fitness
  mContinue &= mpOptProblem->calculate();

  // check whether the functional constraints are fulfilled
  if (!mpOptProblem->checkFunctionalConstraints())
    mEvaluationValue = std::numeric_limits<C_FLOAT64>::infinity();
  else
    mEvaluationValue = mpOptProblem->getCalculateValue();

  return mEvaluationValue;
}

// move an individual
bool COptMethodPS::move(const size_t & index)
{
  const C_FLOAT64 w = 1 / (2 * log(2.0));
  const C_FLOAT64 c = 0.5 + log(2.0);

  bool Improved = false;

  C_FLOAT64 * pIndividual = mIndividuals[index]->array();
  C_FLOAT64 * pEnd = pIndividual + mVariableSize;
  C_FLOAT64 * pVelocity = mVelocities[index];
  C_FLOAT64 * pBestPosition = mBestPositions[index];
  std::vector< COptItem * >::const_iterator itOptItem = mpOptItem->begin();
  C_FLOAT64 ** ppContainerVariable = mContainerVariables.array();

  C_FLOAT64 * pBestInformantPosition = mBestPositions[index];
  C_FLOAT64 BestInformantValue = mBestValues[index];

  std::set< size_t >::const_iterator itInformant = mInformants[index].begin();
  std::set< size_t >::const_iterator endInformant = mInformants[index].end();

  size_t i = mNumInformed + mNumInformedMin;

  for (; i && itInformant != endInformant; --i, ++itInformant)
    if (mBestValues[*itInformant] < BestInformantValue)
      {
        BestInformantValue = mBestValues[*itInformant];
        pBestInformantPosition = mBestPositions[*itInformant];
      }

  for (; pIndividual != pEnd;
       ++pIndividual, ++pVelocity, ++pBestPosition, ++itOptItem, ++ppContainerVariable, ++pBestInformantPosition)
    {
      *pVelocity *= w;
      *pVelocity += c * mpRandom->getRandomCC() * (*pBestPosition - *pIndividual);
      *pVelocity += c * mpRandom->getRandomCC() * (*pBestInformantPosition - *pIndividual);

      *pIndividual += *pVelocity;

      COptItem & OptItem = **itOptItem;

      // force it to be within the bounds
      switch (OptItem.checkConstraint(*pIndividual))
        {
          case - 1:
            *pIndividual = *OptItem.getLowerBoundValue();
            *pVelocity = 0.0;
            break;

          case 1:
            *pIndividual = *OptItem.getUpperBoundValue();
            *pVelocity = 0.0;
            break;
        }

      // We need to set the value here so that further checks take
      // account of the value.
      **ppContainerVariable = *pIndividual;
    }

  // calculate its fitness
  mValues[index] = evaluate();

  // Check if we improved individually
  if (mEvaluationValue < mBestValues[index])
    {
      Improved = true;

      // Save the individually best value;
      mBestValues[index] = mEvaluationValue;
      memcpy(mBestPositions[index], mIndividuals[index]->array(), sizeof(C_FLOAT64) * mVariableSize);

      // Check if we improved globally
      if (mEvaluationValue < mBestValues[mBestIndex])
        {
          // and store that value
          mBestIndex = index;
          mContinue &= mpOptProblem->setSolution(mBestValues[index], *mIndividuals[index]);

          // We found a new best value lets report it.
          mpParentTask->output(COutputInterface::DURING);
        }
    }

  return Improved;
}

// initialise an individual
bool COptMethodPS::create(const size_t & index)
{
  C_FLOAT64 * pIndividual = mIndividuals[index]->array();
  C_FLOAT64 * pEnd = pIndividual + mVariableSize;
  C_FLOAT64 * pVelocity = mVelocities[index];
  C_FLOAT64 * pBestPosition = mBestPositions[index];
  std::vector< COptItem * >::const_iterator itOptItem = mpOptItem->begin();
  C_FLOAT64 ** ppContainerVariable = mContainerVariables.array();

  C_FLOAT64 mn, mx, la;

  for (; pIndividual != pEnd;
       ++pIndividual, ++pVelocity, ++pBestPosition, ++itOptItem, ++ppContainerVariable)
    {
      COptItem & OptItem = **itOptItem;

      // calculate lower and upper bounds
      mn = *OptItem.getLowerBoundValue();
      mx = *OptItem.getUpperBoundValue();

      try
        {
          // First determine the location of the interval
          // Secondly determine whether to distribute the parameter linearly or not
          // depending on the location and act upon it.
          if (0.0 <= mn) // the interval [mn, mx) is in [0, inf)
            {
              la = log10(mx) - log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min()));

              if (la < 1.8 || !(mn > 0.0)) // linear
                {
                  *pIndividual = mn + mpRandom->getRandomCC() * (mx - mn);
                  *pVelocity = mn + mpRandom->getRandomCC() * (mx - mn) - *pIndividual;
                }
              else
                {
                  *pIndividual = pow(10.0, log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min())) + la * mpRandom->getRandomCC());
                  *pVelocity =
                    pow(10.0, log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min())) + la * mpRandom->getRandomCC()) - *pIndividual;
                }
            }
          else if (mx > 0) // 0 is in the interval (mn, mx)
            {
              la = log10(mx) + log10(-mn);

              if (la < 3.6) // linear
                {
                  *pIndividual = mn + mpRandom->getRandomCC() * (mx - mn);
                  *pVelocity = mn + mpRandom->getRandomCC() * (mx - mn) - *pIndividual;
                }
              else
                {
                  C_FLOAT64 mean = (mx + mn) * 0.5;
                  C_FLOAT64 sigma = std::min(std::numeric_limits< C_FLOAT64 >::max(), mx - mn) / 3.0;

                  do
                    {
                      *pIndividual = mpRandom->getRandomNormal(mean, sigma);
                    }
                  while ((*pIndividual < mn) || (*pIndividual > mx));

                  *pVelocity = mpRandom->getRandomNormal(mean, sigma) - *pIndividual;
                }
            }
          else // the interval (mn, mx] is in (-inf, 0]
            {
              // Switch lower and upper bound and change sign, i.e.,
              // we can treat it similarly as location 1:
              mx = - *OptItem.getLowerBoundValue();
              mn = - *OptItem.getUpperBoundValue();

              la = log10(mx) - log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min()));

              if (la < 1.8 || !(mn > 0.0)) // linear
                {
                  *pIndividual = - (mn + mpRandom->getRandomCC() * (mx - mn));
                  *pVelocity = - (mn + mpRandom->getRandomCC() * (mx - mn)) - *pIndividual;
                }
              else
                {
                  *pIndividual = - pow(10.0, log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min())) + la * mpRandom->getRandomCC());
                  *pVelocity =
                    - pow(10.0, log10(std::max(mn, std::numeric_limits< C_FLOAT64 >::min())) + la * mpRandom->getRandomCC()) - *pIndividual;
                }
            }
        }

      catch (...)
        {
          *pIndividual = (mx + mn) * 0.5;
          *pVelocity = 0.0;
        }

      // force it to be within the bounds
      switch (OptItem.checkConstraint(*pIndividual))
        {
          case - 1:
            *pIndividual = *OptItem.getLowerBoundValue();
            break;

          case 1:
            *pIndividual = *OptItem.getUpperBoundValue();
            break;
        }

      *pBestPosition = *pIndividual;

      // We need to set the value here so that further checks take
      // account of the value.
      **ppContainerVariable = *pIndividual;
    }

  // calculate its fitness
  mBestValues[index] = mValues[index] = evaluate();

  if (mBestValues[index] < mBestValues[mBestIndex])
    {
      // and store that value
      mBestIndex = index;
      mContinue &= mpOptProblem->setSolution(mBestValues[index], *mIndividuals[index]);

      // We found a new best value lets report it.
      mpParentTask->output(COutputInterface::DURING);
    }

  return mContinue;
}

void COptMethodPS::initObjects()
{
  addObjectReference("Current Iteration", mCurrentGeneration, CCopasiObject::ValueInt);
}

bool COptMethodPS::initialize()
{
  cleanup();

  if (!COptPopulationMethod::initialize()) return false;

  mGenerations = getValue< unsigned C_INT32 >("Iteration Limit");
  mCurrentGeneration = 0;

  if (mpCallBack)
    mhGenerations =
      mpCallBack->addItem("Iteration Limit",
                          mCurrentGeneration,
                          & mGenerations);

  mPopulationSize = getValue< unsigned C_INT32 >("Swarm Size");

  if (mPopulationSize < 5)
    {
    mPopulationSize = 5;
      setValue("Swarm Size", mPopulationSize);
    }

  mVariance = getValue< C_FLOAT64 >("Std. Deviation");
  mVariance *= mVariance;

  mIndividuals.resize(mPopulationSize);

  size_t i;

  for (i = 0; i < mPopulationSize; i++)
    mIndividuals[i] = new CVector< C_FLOAT64 >(mVariableSize);

  mValues.resize(mPopulationSize);
  mValues = std::numeric_limits<double>::infinity();

  mVelocities.resize(mPopulationSize, mVariableSize);
  mBestValues.resize(mPopulationSize);
  mBestPositions.resize(mPopulationSize, mVariableSize);

  mNumInformedMin = std::max<size_t>(mPopulationSize / 10, 5) - 1;
  mNumInformed = mNumInformedMin;

  mpPermutation = new CPermutation(mpRandom, mPopulationSize);

  mContinue = true;

  return mContinue;
}

bool COptMethodPS::cleanup()
{
  pdelete(mpPermutation);

  return COptPopulationMethod::cleanup();
}

void COptMethodPS::buildInformants()
{
  if (mNumInformed < mPopulationSize)
    mNumInformed++;
  else
    return;

  mInformants.clear();
  mInformants.resize(mPopulationSize);
  mpPermutation->shuffle();

  size_t i, j;
  size_t Informant;

  for (i = 0; i < mPopulationSize; i++)
    {
      mInformants[i].insert(i);

      Informant = mpPermutation->pick();

      for (j = 1; j < mNumInformed; j++, Informant = mpPermutation->next())
        {
          if (Informant == i)
            {
              Informant = mpPermutation->next();
            }

          mInformants[Informant].insert(i);
        }
    }

  return;
}

bool COptMethodPS::reachedStdDeviation()
{
  if (mNumInformed > mNumInformedMin + 1)
    mNumInformed--;

  // Check whether the swarm has settled
  C_FLOAT64 * pValue = mValues.array();
  C_FLOAT64 * pEnd = pValue + mPopulationSize;

  C_FLOAT64 Delta;

  C_FLOAT64 Mean = 0.0;
  C_FLOAT64 Variance = 0.0;
  size_t N = 0;

  for (; pValue != pEnd; ++pValue)
    {
      // We need to deal with infinity values since they indicate failure
      if (*pValue == std::numeric_limits<C_FLOAT64>::infinity())
        return false;

      Delta = *pValue - Mean;
      Mean += Delta / ++N;
      // This uses the new mean, i.e., not Delta * Delta
      Variance += Delta * (*pValue - Mean);
    }

  Variance /= (N - 1);

  if (Variance > mVariance)
    return false;

  // The variance of the function value is smaller than required. We now
  // Check the variance of the flock positions.
  CVector< C_FLOAT64 > FirstMoments(mVariableSize);
  CVector< C_FLOAT64 > SecondMoments(mVariableSize);
  FirstMoments = 0.0;
  SecondMoments = 0.0;

  std::vector<CVector< C_FLOAT64 > *>::iterator pIndividual = mIndividuals.begin();
  std::vector<CVector< C_FLOAT64 > *>::iterator pIndividualEnd = mIndividuals.end();

  C_FLOAT64 * pFirstMoment;
  C_FLOAT64 * pSecondMoment;
  pEnd = FirstMoments.array() + mVariableSize;

  for (; pIndividual != pIndividualEnd; ++pIndividual)
    {
      pFirstMoment = FirstMoments.array();
      pSecondMoment = SecondMoments.array();
      pValue = (*pIndividual)->array();

      for (; pFirstMoment != pEnd; ++pFirstMoment, ++pSecondMoment, ++pValue)
        {
          *pFirstMoment += *pValue;
          *pSecondMoment += *pValue **pValue;
        }
    }

  pFirstMoment = FirstMoments.array();
  pSecondMoment = SecondMoments.array();

  for (; pFirstMoment != pEnd; ++pFirstMoment, ++pSecondMoment)
    {
      Variance = (*pSecondMoment - *pFirstMoment **pFirstMoment / mPopulationSize) / (mPopulationSize - 1);

      if (Variance > mVariance) return false;
    }

  return true;
}

bool COptMethodPS::optimise()
{
  size_t i;

  if (!initialize())
    {
      if (mpCallBack)
        mpCallBack->finishItem(mhGenerations);

      return false;
    }

  C_FLOAT64 * pIndividual = mIndividuals[0]->array();
  C_FLOAT64 * pEnd = pIndividual + mVariableSize;
  C_FLOAT64 * pVelocity = mVelocities[0];
  C_FLOAT64 * pBestPosition = mBestPositions[0];
  std::vector< COptItem * >::const_iterator itOptItem = mpOptItem->begin();
  C_FLOAT64 ** ppContainerVariable = mContainerVariables.array();

  // initialise the population
  // first individual is the initial guess
  for (; pIndividual != pEnd;
       ++pIndividual, ++pVelocity, ++pBestPosition, ++itOptItem, ++ppContainerVariable)
    {
      COptItem & OptItem = **itOptItem;

      *pIndividual = OptItem.getStartValue();

      // force it to be within the bounds
      switch (OptItem.checkConstraint(*pIndividual))
        {
          case - 1:
            *pIndividual = *OptItem.getLowerBoundValue();
            break;

          case 1:
            *pIndividual = *OptItem.getUpperBoundValue();
            break;
        }

      *pBestPosition = *pIndividual;
      *pVelocity = 0.0;

      // We need to set the value here so that further checks take
      // account of the value.
      **ppContainerVariable = *pIndividual;
    }

  // calculate its fitness
  mBestValues[0] = mValues[0] = evaluate();

  // and store that value
  mBestIndex = 0;
  mContinue &= mpOptProblem->setSolution(mBestValues[0], *mIndividuals[0]);

  // We found a new best value lets report it.
  mpParentTask->output(COutputInterface::DURING);

  // the others are random
  for (i = 1; i < mPopulationSize && mContinue; i++)
    create(i);

  // create the informant list
  buildInformants();

  bool Improved;

  for (; mCurrentGeneration < mGenerations && mContinue; mCurrentGeneration++)
    {
      Improved = false;

      for (i = 0; i < mPopulationSize && mContinue; i++)
        Improved |= move(i);

      if (!Improved)
        buildInformants();
      else if (reachedStdDeviation())
        break;

      if (mpCallBack)
        mContinue &= mpCallBack->progressItem(mhGenerations);
    
//#define POP_DISPLAY
#ifdef POP_DISPLAY
      //use a different output channel. It will later get a proper enum name
      mpParentTask->output(COutputInterface::Activity(8));
#endif
    }

  if (mpCallBack)
    mpCallBack->finishItem(mhGenerations);

  cleanup();

  return true;
}
