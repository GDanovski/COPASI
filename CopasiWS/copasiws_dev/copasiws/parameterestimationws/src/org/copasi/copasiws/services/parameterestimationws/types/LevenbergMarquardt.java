// Begin CVS Header 
//   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/CopasiWS/copasiws_dev/copasiws/parameterestimationws/src/org/copasi/copasiws/services/parameterestimationws/types/LevenbergMarquardt.java,v $ 
//   $Revision: 1.1 $ 
//   $Name:  $ 
//   $Author: jdada $ 
//   $Date: 2008/11/11 12:19:52 $ 
// End CVS Header 

// Copyright (C) 2008 by Pedro Mendes, Virginia Tech Intellectual 
// Properties, Inc., EML Research, gGmbH, University of Heidelberg, 
// and The University of Manchester. 
// All rights reserved. 

/*
 * XML Type:  LevenbergMarquardt
 * Namespace: http://parameterestimationws.services.copasiws.copasi.org/types
 * Java type: org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt
 *
 * Automatically generated - do not modify.
 */
package org.copasi.copasiws.services.parameterestimationws.types;


/**
 * An XML LevenbergMarquardt(@http://parameterestimationws.services.copasiws.copasi.org/types).
 *
 * This is a complex type.
 */
public interface LevenbergMarquardt extends org.apache.xmlbeans.XmlObject {
    public static final org.apache.xmlbeans.SchemaType type = (org.apache.xmlbeans.SchemaType) org.apache.xmlbeans.XmlBeans.typeSystemForClassLoader(LevenbergMarquardt.class.getClassLoader(),
            "schemaorg_apache_xmlbeans.system.parameterestimationwsTypeSystem")
                                                                                                                           .resolveHandle("levenbergmarquardt1f20type");

    /**
     * Gets the "iterationLimit" element
     */
    java.math.BigInteger getIterationLimit();

    /**
     * Gets (as xml) the "iterationLimit" element
     */
    org.apache.xmlbeans.XmlNonNegativeInteger xgetIterationLimit();

    /**
     * Sets the "iterationLimit" element
     */
    void setIterationLimit(java.math.BigInteger iterationLimit);

    /**
     * Sets (as xml) the "iterationLimit" element
     */
    void xsetIterationLimit(
        org.apache.xmlbeans.XmlNonNegativeInteger iterationLimit);

    /**
     * Gets the "tolerance" element
     */
    float getTolerance();

    /**
     * Gets (as xml) the "tolerance" element
     */
    org.apache.xmlbeans.XmlFloat xgetTolerance();

    /**
     * Sets the "tolerance" element
     */
    void setTolerance(float tolerance);

    /**
     * Sets (as xml) the "tolerance" element
     */
    void xsetTolerance(org.apache.xmlbeans.XmlFloat tolerance);

    /**
     * A factory class with static methods for creating instances
     * of this type.
     */
    public static final class Factory {
        private Factory() {
        } // No instance of this class allowed

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt newInstance() {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .newInstance(type,
                null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt newInstance(
            org.apache.xmlbeans.XmlOptions options) {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .newInstance(type,
                options);
        }

        /** @param xmlAsString the string value to parse */
        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.lang.String xmlAsString)
            throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(xmlAsString,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.lang.String xmlAsString, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(xmlAsString,
                type, options);
        }

        /** @param file the file from which to load an xml document */
        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.File file)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(file,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.File file, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(file,
                type, options);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.net.URL u)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(u,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.net.URL u, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(u,
                type, options);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.InputStream is)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(is,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.InputStream is, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(is,
                type, options);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.Reader r)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(r,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            java.io.Reader r, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException, java.io.IOException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(r,
                type, options);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            javax.xml.stream.XMLStreamReader sr)
            throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(sr,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            javax.xml.stream.XMLStreamReader sr,
            org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(sr,
                type, options);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            org.w3c.dom.Node node) throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(node,
                type, null);
        }

        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            org.w3c.dom.Node node, org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(node,
                type, options);
        }

        /** @deprecated {@link XMLInputStream} */
        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            org.apache.xmlbeans.xml.stream.XMLInputStream xis)
            throws org.apache.xmlbeans.XmlException,
                org.apache.xmlbeans.xml.stream.XMLStreamException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(xis,
                type, null);
        }

        /** @deprecated {@link XMLInputStream} */
        public static org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt parse(
            org.apache.xmlbeans.xml.stream.XMLInputStream xis,
            org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException,
                org.apache.xmlbeans.xml.stream.XMLStreamException {
            return (org.copasi.copasiws.services.parameterestimationws.types.LevenbergMarquardt) org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                                                                                                             .parse(xis,
                type, options);
        }

        /** @deprecated {@link XMLInputStream} */
        public static org.apache.xmlbeans.xml.stream.XMLInputStream newValidatingXMLInputStream(
            org.apache.xmlbeans.xml.stream.XMLInputStream xis)
            throws org.apache.xmlbeans.XmlException,
                org.apache.xmlbeans.xml.stream.XMLStreamException {
            return org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                               .newValidatingXMLInputStream(xis,
                type, null);
        }

        /** @deprecated {@link XMLInputStream} */
        public static org.apache.xmlbeans.xml.stream.XMLInputStream newValidatingXMLInputStream(
            org.apache.xmlbeans.xml.stream.XMLInputStream xis,
            org.apache.xmlbeans.XmlOptions options)
            throws org.apache.xmlbeans.XmlException,
                org.apache.xmlbeans.xml.stream.XMLStreamException {
            return org.apache.xmlbeans.XmlBeans.getContextTypeLoader()
                                               .newValidatingXMLInputStream(xis,
                type, options);
        }
    }
}
