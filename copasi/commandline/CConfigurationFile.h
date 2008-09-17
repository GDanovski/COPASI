/* Begin CVS Header
  $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/commandline/CConfigurationFile.h,v $
  $Revision: 1.4 $
  $Name:  $
  $Author: aruff $
  $Date: 2008/09/17 17:26:29 $
  End CVS Header */

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., EML Research, gGmbH, University of Heidelberg,
// and The University of Manchester.
// All rights reserved.

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef COPASI_CConfigurationFile
#define COPASI_CConfigurationFile

#include "utilities/CCopasiParameterGroup.h"
#include "MIRIAM/CMIRIAMResource.h"
#include "xml/CCopasiXMLInterface.h"

#ifdef COPASI_LICENSE_COM
class CRegistration;
#endif // COPASI_LICENSE_COM
class CMIRIAMResources;

class CRecentFiles : public CCopasiParameterGroup
  {
    // Operations
  public:
    /**
     * Default constructor
     * @param const std::string & name (default: Recent Files)
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CRecentFiles(const std::string & name = "Recent Files",
                 const CCopasiContainer * pParent = NULL);

    /**
     * Copy constructor
     * @param const CRecentFiles & src
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CRecentFiles(const CRecentFiles & src,
                 const CCopasiContainer * pParent = NULL);

    /**
     * Specific constructor
     * @param const CCopasiParameterGroup & group
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CRecentFiles(const CCopasiParameterGroup & group,
                 const CCopasiContainer * pParent = NULL);

    /**
     * Destructor
     */
    virtual ~CRecentFiles();

    /**
     * Add a file to the list of recent files
     * @param const std::string & file
     */
    void addFile(const std::string & file);

  private:
    /**
     * Allocates all group parameters and assures that they are
     * properly initialized.
     */
    void initializeParameter();

    // Attributes
  private:
    /**
     * A pinter to the maximal number of recent files
     */
    unsigned C_INT32 * mpMaxFiles;

    /**
     * A pointer to the list of recent files.
     */
    CCopasiParameterGroup * mpRecentFiles;
  };

class CConfigurationFile : public CCopasiParameterGroup
  {
    // Operations
  public:
    /**
     * Default constructor
     * @param const std::string & name (default: Configuration)
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CConfigurationFile(const std::string & name = "Configuration",
                       const CCopasiContainer * pParent = NULL);

    /**
     * Copy constructor
     * @param const CConfigurationFile & src
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CConfigurationFile(const CConfigurationFile & src,
                       const CCopasiContainer * pParent = NULL);

    /**
     * Specific constructor
     * @param const CCopasiParameterGroup & group
     * @param const CCopasiContainer * pParent (default: NULL)
     */
    CConfigurationFile(const CCopasiParameterGroup & group,
                       const CCopasiContainer * pParent = NULL);

    /**
     * Destructor
     */
    virtual ~CConfigurationFile();

    /**
     * This methods must be called to elevate subgroups to
     * derived objects. The default implementation does nothing.
     * @return bool success
     */
    virtual bool elevateChildren();

    /**
     * Save the configuration file.
     * @return bool success
     */
    virtual bool save();

    /**
     * Load the configuration file.
     * @return bool success
     */
    virtual bool load();

    /**
     * Retreive the list of recent files
     * @return CRecentFiles & recentFiles
     */
    CRecentFiles & getRecentFiles();

    /**
     * Retreive the list of recent SBML files
     * @return CRecentFiles & recentSBMLFiles
     */
    CRecentFiles & getRecentSBMLFiles();

    /**
     * Retreive the list of recent MIRIAM Resources
     * @return CMIRIAMResources & recentMIRIAMResources
     */
    CMIRIAMResources & getRecentMIRIAMResources();

    void setRecentMIRIAMResources(const CMIRIAMResources & miriamResources);

  private:
    /**
     * Allocates all group parameters and assures that they are
     * properly initialized.
     */
    void initializeParameter();

    // Attributes
  private:
    /**
     * Class dealing with the loading and writing of the XML configuration
     * file.
     */
  class CXML : public CCopasiXMLInterface
      {
        // Operations
      public:
        /**
         * Constructor
         */
        CXML();

        /**
         * Destructor
         */
        ~CXML();

        /**
         * Save information to a given ostream.
         * @param std::ostream & os
         * @param const std::string & relativeTo
         * @return bool success
         */
        virtual bool save(std::ostream & os,
                          const std::string & relativeTo);

        /**
         * Load information from a given istream.
         * @param std::istream & is
         * @param const std::string & relativeTo
         * @return bool success
         */
        virtual bool load(std::istream & is,
                          const std::string & relativeTo);

        /**
         * Set the configuration contents
         * @param const CConfigurationFile & configuration
         */
        void setConfiguration(const CCopasiParameterGroup & configuration);

        /**
         * Retreive the configuration contents
         * @return const CConfigurationFile & configuration
         */
        const CCopasiParameterGroup & getConfiguration() const;

        // Attributes
      private:
        /**
         * A pointer to the CCopasiParameterGroup containing the information
         */
        CCopasiParameterGroup mConfiguration;
      };

    /**
     * A pointer to the list of recent files
     */
    CRecentFiles * mpRecentFiles;

    /**
     * A pointer to the list of recent files
     */
    CRecentFiles * mpRecentSBMLFiles;

    /**
     * A pointer to the list of MIRIAM Resources
     */
    CMIRIAMResources * mpRecentMIRIAMResources;

#ifdef COPASI_LICENSE_COM
    /**
     * Apointer to the registration information
     */
    CRegistration * mpRegistration;
#endif // COPASI_LICENSE_COM;
  };

#endif // COPASI_CConfigurationFile
