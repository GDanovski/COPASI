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

// Copyright (C) 2005 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef CREACTIONINTERFACE_H
#define CREACTIONINTERFACE_H

#include <vector>
#include <string>
#include "copasi.h"
//#include "model/CChemEq.h"
#include "model/CReaction.h"
#include "model/CChemEqInterface.h"
#include "function/CFunction.h"

//class CFunction;
class CModel;

/**
 * This class provides an interface for dealing with reactions.
 * For representing the parameter mappings it uses object names.
 * A CReactionInterface allows editing a reaction while trying to
 * keep everything consistent.
 *
 * Note: Many of the methods need to be provided with a model.
 * While handling of the metabolites is completely independent of the
 * model (they are handled by name, and they don't need to exist),
 * the mapping of Compartments and global parameters is done with
 * the entities that exist in the model. This may seem inconsistent,
 * but the idea is that metabolites can be created from the gui by entering
 * them into the reaction equation. Compartments and global parameters
 * can only be chosen from the list of existing objects.
 */
class CReactionInterface
{
private:
  CModel * mpModel;

  std::string emptyString;

  /**
   * This is the key that identifies the Reaction that is beeing edited
   */
  std::string mReactionReferenceKey;

  /**
   * A copy of the chemical equation of the reaction
   */
  CChemEqInterface mChemEqI;

  /**
   * A pointer to the kinetic function of the reaction
   */
  const CFunction* mpFunction;

  /**
   * A copy of the function parameters
   */
  CFunctionParameters * mpParameters;

  /**
   * what metabolite for what function parameter
   */
  std::vector< std::vector< std::string > > mNameMap;

  /**
   * values of the kinetic parameters
   */
  std::vector< C_FLOAT64 > mValues;

  /**
   * values of the kinetic parameters
   */
  std::vector<bool> mIsLocal;

  CReaction::KineticLawUnit mKineticLawUnitType;

  std::string mScalingCompartment;

private:
  CReactionInterface();

public:
  CReactionInterface(CModel * pModel);

  ~CReactionInterface();

  /**
   * set a new chemical equation.
   * newFunction suggests a new kinetic function which is only used if adequate.
   */
  void setChemEqString(const std::string & eq, const std::string & newFunction);

  /**
   * internal function that temporarily removes the equation
   */
  void clearChemEquation();

  std::string getChemEqString() const;

  const CChemEqInterface & getChemEqInterface() const;

  bool isReversible() const;

  /**
   * this method tries to find out if the REACTION involves several compartments
   * It only takes into account the metabs that
   * actually exist in the model. A non existing metabolite is assumed
   * not to be in a different compartment
   */
  bool isMulticompartment() const;

  /**
   * set the reversibility.
   * newFunction suggests a new kinetic function which is only used if adequate.
   */
  void setReversibility(bool rev, const std::string & newFunction);

  /**
   * reverse the reaction and set the reversibility.
   * newFunction suggests a new kinetic function which is only used if adequate.
   */
  void reverse(bool rev, const std::string & newFunction);

  /**
   * This produces a list of metab names (from the chem eq) for use in
   * the combo boxes. The role must be given like a usage, e.g. "SUBSTRATE".
   */
  const std::vector<std::string> & getListOfMetabs(CFunctionParameter::Role role) const;

  /**
   * set the function. an empty mapping is created
   */
  void setFunctionWithEmptyMapping(const std::string & fn);

  /**
   * set the function.
   * a new mapping is created that tries to preserve as much information as
   * possible from the old mapping. Then a default mapping is set from
   * the chemical equation and the model (if possible).
   */
  void setFunctionAndDoMapping(const std::string & fn);

  const std::string & getFunctionName() const;

  const std::string & getFunctionDescription() const;

  const CFunction * getFunction() const;

  std::vector< std::string > getListOfPossibleFunctions() const;

  //query information about the function variables

  size_t size() const;

  bool isVector(size_t index) const;

  CFunctionParameter::Role getUsage(size_t index) const;

  std::string getParameterName(size_t index) const;

  // set/get the mapping

  void setMapping(size_t index, std::string mn);

  void removeMapping(size_t index, std::string mn);

  const std::vector< std::string > & getMappings(size_t index) const;

  const std::string & getMapping(size_t index) const;

  void setLocalValue(size_t index, C_FLOAT64 value);

  void setLocal(size_t index);

  const C_FLOAT64 & getLocalValue(size_t index) const;

  bool isLocalValue(size_t index) const;

  /**
   *  associate the function parameter referenced by "index" with the global
   *  parameter named pn. Only valid if the role for this function parameter is "PARAMETER".
   *  returns success
   */
  //bool setGlobalParameter(size_t index, std::string pn);

  //const std::string & getGlobalParameter(size_t index) const;

  //bool setCompartment(size_t index, std::string pn);

  //const std::string & getCompartment(size_t index) const;

  void initFromReaction(const std::string & key);
  void initFromReaction(const C_INT32 index);
  void initFromReaction(const CReaction* rea);

  /**
   * writes the information back to a CReaction.
   * createMetabolites() and createOtherObjects() should be called before.
   * @param bool compile (default: true)
   */
  bool writeBackToReaction(CReaction * rea, bool compile = true);

  /**
   * create all metabolites that are needed by the reaction but do
   * not exist in the model yet.
   */
  bool createMetabolites();

  /**
   * create all metabolites that are needed by the reaction but do
   * not exist in the model yet.
   *
   * @param createdKeys vector, that will be filled with the keys
   *        of created elements
   *
   * @return true in case elements were created, false otherwise
   */
  bool createMetabolites(std::vector<std::string>& createdKeys);

  /**
   * create all other objects that are needed by the reaction but do
   * not exist in the model yet.
   */
  bool createOtherObjects() const;

  /**
   * @brief createOtherObjects
   *
   * @param createdKeys vector that will be filled with created keys
   *
   * @return true, in case elements were created, false otherwise
   */
  bool createOtherObjects(std::vector<std::string>& createdKeys) const;

  bool isValid() const;

  /**
   * Is the mapping of this parameter locked?
   * The bahaviour of this method is different for different roles:
   * SUBSTRATE, PRODUCT: according to the chemical equation
   * MODIFIER: always unlocked
   * TIME: always locked
   * VOLUME, PARAMETER: according to the model
   *
   * The idea is that in the reaction GUI new species can be entered
   * in the chemical equation that are then created automatically.
   * Compartments and global parameters can only be chosen from those
   * existing in the model.
   */
  bool isLocked(size_t index) const;
  bool isLocked(CFunctionParameter::Role usage) const;

  /**
   * Retrieve the list of parameters which will be deleted
   * @return std::set< const CCopasiObject * > DeletedParameters
   */
  std::set< const CCopasiObject * > getDeletedParameters() const;

  void setKineticLawUnitType(const CReaction::KineticLawUnit & kineticLawUnitType);
  const CReaction::KineticLawUnit & getKineticLawUnitType() const;
  CReaction::KineticLawUnit getEffectiveKineticLawUnitType() const;
  std::string getConcentrationRateUnit() const;
  std::string getAmountRateUnit() const;
  std::string getEffectiveKineticLawUnit() const;
  void setScalingCompartment(const std::string & scalingCompartment);
  const std::string & getScalingCompartment() const;
  std::string getDefaultScalingCompartment() const;

#ifdef COPASI_DEBUG
  void printDebug() const;
#endif // COPASI_DEBUG

private:

  /**
   * initialize mapping (resize vectors and set names to "unknown").
   * Also initializes mpParameters but doesn't delete the old mpParameters.
   */
  void initMapping();

  /**
   * create new mapping, try to keep as much information from the current
   * mapping. Calls initMapping().
   */
  void copyMapping();

  /**
   * guesses how to connect metabs with parameters for a specific usage
   */
  void connectFromScratch(CFunctionParameter::Role role);

  /**
   * tries to determine the mapping for PARAMETER, VOLUME, TIME
   * only if the current mapping is "unknown".
   * Is typically called after initMapping() and copyMapping()
   */
  void connectNonMetabolites();

  /**
   * updates the modifiers in the chemical equation according to
   * the parameter mapping
   */
  void updateModifiersInChemEq();

  void clearFunction();

  /**
   * checks if newFunction is an valid function for the reaction.
   * If it is not or if newFunction="" another function is chosen.
   */
  void findAndSetFunction(const std::string & newFunction);

  /**
   * returns a list of metabolites (from the chemical equation). Species can occur
   * several times according to theit multiplicity
   */
  std::vector<std::string> getExpandedMetabList(CFunctionParameter::Role role) const;

  bool loadMappingAndValues(const CReaction & rea);
};

#endif
