/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/scan/CScanTask.h,v $
   $Revision: 1.18 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2003/11/12 16:51:08 $
   End CVS Header */

/**
 * CScanTask class.
 *
 * This class implements a scan task which is comprised of a
 * of a problem and a method.
 *  
 */

#ifndef COPASI_CScanTask
#define COPASI_CScanTask

#include "utilities/CReadConfig.h"
#include "utilities/CWriteConfig.h"
#include "CScanMethod.h"
#include "utilities/CVector.h"
#include "CScanProblem.h"

class CScanMethod;
class COutputEvent;
class CReport;

class CScanTask : public CCopasiContainer
  {
    //Attributes
  private:

    CReport * mReport;

    /**
     * Flag indicating whether the task shall be executed by CopasiSE
     */
    bool mRequested;

    /**
     * A pointer to the problem to be integrated.
     */
    CScanProblem *mpProblem;

    /**
     * A pointer to the method choosen for the integration.
     */
    CScanMethod * mpMethod;

    /**
     * Pointer to the output stream for reporting
     */
    std::ostream * mpOut;

    /**
     * End Phase Output Event
     */
    COutputEvent *mpOutEnd;

    /**
        * Unique Key
        */
    std::string mKey;

    /*
     Output value address
    */
    CVector<C_FLOAT64*>* pValueAddrMatrix;
    //Operations

  public:

    inline void setValueMatrixAddr(CVector<C_FLOAT64*>* pInputValueAddrMatrix)
    {
      //      mpProblem->setValueMatrixAddr(pInputValueAddrMatrix);
      pValueAddrMatrix = pInputValueAddrMatrix;
    }

    inline CReport* getReport() {return mReport;};

    inline std::string getKey() const {return mKey;}
    /**
     * default constructor
     */
    CScanTask();

    /**
     * Copy constructor
     * @param const CScanTask & src
     */
    CScanTask(const CScanTask & src);

    /**
     * Destructor
     */
    ~CScanTask();

    /**
     * cleanup()
     */
    void cleanup();

    /**
     * Initilize the reporting feature
     * @param ofstream & out
     */
    void initializeReporting(std::ostream & out);

    /**
     * Loads parameters for this solver with data coming from a
     * CReadConfig object. (CReadConfig object reads an input stream)
     * @param configbuffer reference to a CReadConfig object.
     */
    void load(CReadConfig & configBuffer);

    /**
     * Set wheter the execution of the task is requested.
     * @param const bool & execute
     */
    void setRequested(const bool & requested);

    /**
     * Retrieve whether the execution of the task is requested
     * @return bool isRequested
     */
    bool isRequested() const;

    /**
     * Retrieve the probel to be integrated.
     * @return CTrajectoryProblem * pProblem
     */
    CScanProblem * getProblem();

    /**
     * Set the problem to be integrated.
     * @param CTrajectoryProblem * pProblem
     */
    void setProblem(CScanProblem * pProblem);

    /**
     * Retrieve the method choosen for the integration.
     * @return CTrajectoryMethod * pMethod
     */
    CScanMethod * getMethod();

    /**
     * Set the method to be used for the integration.
     * @param CTrajectoryMethod * pMethod
     */
    void setMethod(CScanMethod * pMethod);

    /**
     * Do the integration
     */
    void process();
  };
#endif // COPASI_CScanTask
