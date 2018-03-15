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

#include "copasi.h"

#include "CRDFGraph.h"
#include "CModelMIRIAMInfo.h"
#include "CReference.h"

#include "report/CKeyFactory.h"
#include "CopasiDataModel/CDataModel.h"
#include "model/CModel.h"
#include "copasi/core/CRootContainer.h"

// static
CReference * CReference::fromData(const CData & data)
{
  return new CReference(data.getProperty(CData::OBJECT_NAME).toString(),
                        NO_PARENT);
}

// virtual
CData CReference::toData() const
{
  CData Data = CDataContainer::toData();

  Data.addProperty(CData::MIRIAM_RESOURCE, getResource());
  Data.addProperty(CData::MIRIAM_ID, getId());

  return Data;
}

// virtual
bool CReference::applyData(const CData & data, CUndoData::ChangeSet & changes)
{
  bool success = CDataContainer::applyData(data, changes);

  if (data.isSetProperty(CData::MIRIAM_RESOURCE))
    {
      setResource(data.getProperty(CData::MIRIAM_RESOURCE).toString());
    }

  if (data.isSetProperty(CData::MIRIAM_ID))
    {
      setId(data.getProperty(CData::MIRIAM_ID).toString());
    }

  return success;
}

CReference::CReference(const std::string & objectName,
                       const CDataContainer * pParent):
  CDataContainer(objectName, pParent, "Reference"),
  mTriplet(),
  mNodePath(),
  mKey(CRootContainer::getKeyFactory()->add("Reference", this)),
  mIdTriplet(),
  mResource(NULL)
{}

CReference::CReference(const CRDFTriplet & triplet,
                       const std::string & objectName,
                       const CDataContainer * pParent):
  CDataContainer(objectName, pParent, "Reference"),
  mTriplet(triplet),
  mNodePath(),
  mKey(CRootContainer::getKeyFactory()->add("Creator", this)),
  mIdTriplet(),
  mResource(NULL)
{
  if (!mTriplet)
    return;

  mNodePath = mTriplet.pObject->getPath();

  CRDFPredicate::ePredicateType Predicates[] =
  {
    CRDFPredicate::copasi_isDescribedBy,
    CRDFPredicate::bqbiol_isDescribedBy,
    CRDFPredicate::bqmodel_isDescribedBy,
    CRDFPredicate::end
  };

  std::set< CRDFTriplet > Triples;

  CRDFPredicate::ePredicateType * pPredicate = Predicates;
  std::set< CRDFTriplet >::iterator it;

  for (; *pPredicate != CRDFPredicate::end; ++pPredicate)
    {
      Triples = mTriplet.pObject->getDescendantsWithPredicate(*pPredicate);
      it = Triples.begin();

      if (it != Triples.end())
        {
          mIdTriplet = *it;
          mResource.setNode(mIdTriplet.pObject);
        }
    }
}

CReference::CReference(const CReference & src,
                       const CDataContainer * pParent):
  CDataContainer(src, pParent),
  mTriplet(src.mTriplet),
  mNodePath(src.mNodePath),
  mKey(CRootContainer::getKeyFactory()->add("Creator", this)),
  mIdTriplet(src.mIdTriplet),
  mResource(src.mResource)
{}

CReference::~CReference()
{
  CRootContainer::getKeyFactory()->remove(mKey);
}

const CRDFTriplet & CReference::getTriplet() const
{return mTriplet;}
const CMIRIAMResourceObject & CReference::getMIRIAMResourceObject() const
{
  return mResource;
}

const std::string & CReference::getKey() const
{return mKey;}

std::string CReference::getResource() const
{return mResource.getDisplayName();}

void CReference::setResource(const std::string & resource)
{
  if (!mIdTriplet)
    {
      // We create an Id triplet
      // This only adds the triplet if the resource is valid, i.e., it does not work;

      mTriplet.pObject->setFieldValue("---", CRDFPredicate::copasi_isDescribedBy, mNodePath);

      std::set< CRDFTriplet > Triples;
      std::set< CRDFTriplet >::iterator it;

      Triples = mTriplet.pObject->getDescendantsWithPredicate(CRDFPredicate::copasi_isDescribedBy);
      it = Triples.begin();

      if (it != Triples.end())
        {
          mIdTriplet = *it;
          mResource.setNode(mIdTriplet.pObject);
        }
    }

  if (mResource.setDisplayName(resource))
    mIdTriplet.pObject->getObject().setResource(mResource.getURI(), false);
}

const std::string & CReference::getId() const
{return mResource.getId();}

void CReference::setId(const std::string & id)
{
  if (!mIdTriplet)
    {
      // We create an Id triplet
      mTriplet.pObject->setFieldValue("---", CRDFPredicate::copasi_isDescribedBy, mNodePath);

      std::set< CRDFTriplet > Triples;
      std::set< CRDFTriplet >::iterator it;

      Triples = mTriplet.pObject->getDescendantsWithPredicate(CRDFPredicate::copasi_isDescribedBy);
      it = Triples.begin();

      if (it != Triples.end())
        {
          mIdTriplet = *it;
          mResource.setNode(mIdTriplet.pObject);
        }
    }

  if (mResource.setId(id))
    mIdTriplet.pObject->getObject().setResource(mResource.getURI(), false);
}

std::string CReference::getURI() const
{return mResource.getURI();}

const std::string & CReference::getDescription() const
{return mTriplet.pObject->getFieldValue(CRDFPredicate::dcterms_description);}

void CReference::setDescription(const std::string & description)
{
  mTriplet.pObject->setFieldValue(description, CRDFPredicate::dcterms_description, mNodePath);
}

void CReference::clearInvalidEntries()
{
  // Empty descriptions are handled automatically since setFieldValue removes
  // a triplet if the value is an empty string.

  // Handle invalid resource
  if (!mResource.isValid() && mIdTriplet)
    {
      // We remove the Id triplet
      mTriplet.pObject->setFieldValue("", CRDFPredicate::copasi_isDescribedBy, mNodePath);
      mIdTriplet = CRDFTriplet(); // This makes it invalid.

      mResource.setURI("---");
    }
}
