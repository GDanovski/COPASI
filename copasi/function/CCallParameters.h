/**
 * CFunctionParameterMap
 * 
 * Created for Copasi by Stefan Hoops
 * (C) Stefan Hoops 2002
 */

#ifndef COPASI_CCallParameters
#define COPASI_CCallParameters

#include <vector>
#include "CFunctionParameter.h"
#include "CFunctionParameters.h"
#include "report/CCopasiObject.h"

/**
 * CCallParameterPointers
 *
 * This is what is actually passed to a function. Some of the void* can be
 * pointers to vector<const CCopasiObject*>
 */
class CCallParameterPointers : public std::vector< const void * >
  {};

/**
 * CFunctionParameterMap         
 *
 * This class describes how a reaction fills the call parameters of a function.
 * It is generated from a CFunctionParameters object.
 * The reaction provides what exactly is to be passed as function parameters.
 */
class CFunctionParameterMap
  {
  public:

    CFunctionParameterMap();

    CFunctionParameterMap(const CFunctionParameterMap & src);

    /**
     * Destructor();
     */
    ~CFunctionParameterMap();

    /**
     * Sets a specific parameter. Works only if the parameter is no vector
     */
    void setCallParameter(const std::string paramName, const CCopasiObject* obj);

    /**
     * Adds an object to a specific parameter vector. Works only if the parameter is a vector
     */
    void addCallParameter(const std::string paramName, const CCopasiObject* obj);

    /**
     * Removes an object from a specific parameter vector. Works only if the parameter is a vector
     */
    void removeCallParameter(const std::string paramName, const CCopasiObject* obj);

    /**
     * Removes all objects from a specific parameter vector. Works only if the parameter is a vector.
     */
    void clearCallParameter(const std::string paramName);

    /**
     * Initializes a CallParameters object from a CFunctionParameters object.
     */
    void initializeFromFunctionParameters(const CFunctionParameters & src);

    unsigned C_INT32 findParameterByName(const std::string & name,
                                         CFunctionParameter::DataType & dataType) const;

  private:

    /**
     * Clear the mPointers and mObjects vectors.
     */
    void clearCallParameters();

    /**
     * Create the mPointers and mObjects vectors with
     * type information from the CFunctionParameters object.
     */
    void initCallParameters();

    /**
     * Check if all pointers are !=NULL.
     */
    void checkCallParameters() const;

    /**
     * This is a vector of pointers to the data that is passed to a function
     */
    CCallParameterPointers mPointers;

    /**
     * This is a vector of pointers to objects. Each objects needs to have a value
     * that can be passed to a function
     */
    CCallParameterPointers mObjects;

    /**
     * The CFunctionParameters object provides the data types of the call parameters
     */
    CFunctionParameters mFunctionParameters;

    // these are preliminary
    CCallParameterPointers & getObjects() {return mObjects;};
  public:
    const CCallParameterPointers & getObjects() const {return mObjects;};
    CCallParameterPointers & getPointers() {return mPointers;};
    const CFunctionParameters & getFunctionParameters() const {return mFunctionParameters;};
  };

#endif // COPASI_CCallParameters
