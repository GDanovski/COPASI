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

// Copyright (C) 2004 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

/*!
    \file CAnnotatedMatrix.cpp
    \brief Implementation file of class CCopasiArray and CArrayAnnotation
 */

#include <sstream>
#include "CAnnotatedMatrix.h"
#include "CCopasiVector.h"

#include "report/CKeyFactory.h"
#include "report/CArrayElementReference.h"
#include "CopasiDataModel/CCopasiDataModel.h"

CArrayAnnotation::CArrayAnnotation(const std::string & name,
                                   const CCopasiContainer * pParent,
                                   CCopasiAbstractArray * array,
                                   const bool & adopt)
  : CCopasiContainer(name, pParent, "Array" , CCopasiObject::Array),
    mpArray(array),
    mDestructArray(adopt),
    mDefaultMode(OBJECTS)
{
  assert(mpArray);

  resize();

  //addObjectReference("Annotated Matrix", *this, CCopasiObject::ValueDbl);
}

CArrayAnnotation::~CArrayAnnotation()
{
  if (mDestructArray)
    pdelete(mpArray);
}

void CArrayAnnotation::setArray(CCopasiAbstractArray * a)
{
  mpArray = a;
}

void CArrayAnnotation::setMode(size_t d, Mode m)
{
  mModes[d] = m;
  resizeOneDimension(d);
}

void CArrayAnnotation::setMode(Mode m)
{
  mDefaultMode = m;

  size_t i;

  for (i = 0; i < mModes.size(); ++i)
    setMode(i, m);
}

size_t CArrayAnnotation::dimensionality() const
{
  return mModes.size();
}

void CArrayAnnotation::setAnnotation(size_t d, size_t i, const CCopasiObject * pObject)
{
  assert(d < dimensionality());

  resizeOneDimension(d);
  assert(i < mAnnotationsCN[d].size());

  if (pObject != NULL)
    {
      mAnnotationsCN[d][i] = pObject->getCN();
      mAnnotationsString[d][i] = pObject->getObjectDisplayName();
    }
  else
    {
      mAnnotationsCN[d][i] = std::string("");
      mAnnotationsString[d][i] = "";
    }
}

void CArrayAnnotation::setAnnotationString(size_t d, size_t i, const std::string s)
{
  assert(d < dimensionality());
  assert(i < mAnnotationsString[d].size());

  mAnnotationsCN[d][i] = "String=" + CCopasiObjectName::escape(s);
  mAnnotationsString[d][i] = s;
}

const std::vector<CRegisteredObjectName> & CArrayAnnotation::getAnnotationsCN(size_t d) const
{
  return mAnnotationsCN[d];
}

const std::vector<std::string> & CArrayAnnotation::getAnnotationsString(size_t d, bool display) const
{
  assert(d < mModes.size());

  std::vector< std::string >::iterator itName = mAnnotationsString[d].begin();
  std::vector< std::string >::iterator endName = mAnnotationsString[d].end();

  if (mModes[d] == NUMBERS)
    {
      size_t Index = 1;

      for (; itName != endName; ++Index, ++itName)
        {
          *itName = StringPrint("%d", Index);
        }
    }
  else
    {
      std::vector<CRegisteredObjectName>::const_iterator itCN = mAnnotationsCN[d].begin();

      for (; itName != endName; ++itCN, ++itName)
        {
          *itName = createDisplayName(*itCN);
        }
    }

  return mAnnotationsString[d];
}

const std::string & CArrayAnnotation::getDimensionDescription(size_t d) const
{
  assert(d < mDimensionDescriptions.size());
  return mDimensionDescriptions[d];
}

void CArrayAnnotation::setDimensionDescription(size_t d, const std::string & s)
{
  assert(d < mDimensionDescriptions.size());
  mDimensionDescriptions[d] = s;
}

const std::string & CArrayAnnotation::getDescription() const
{return mDescription;}

void CArrayAnnotation::setDescription(const std::string & s)
{mDescription = s;}

//private
void CArrayAnnotation::reDimensionalize(size_t d)
{
  mAnnotationsCN.resize(d);
  mAnnotationsString.resize(d);
  mDimensionDescriptions.resize(d);
  mModes.resize(d, mDefaultMode);
}

//private
void CArrayAnnotation::resizeOneDimension(size_t d)
{
  mAnnotationsCN[d].resize(mpArray->size()[d]);
  mAnnotationsString[d].resize(mpArray->size()[d]);
}

void CArrayAnnotation::resize()
{
  assert(mpArray);
  reDimensionalize(mpArray->dimensionality());

  size_t i, imax = mpArray->dimensionality();

  for (i = 0; i < imax; ++i)
    resizeOneDimension(i);
}

const CCopasiObject * CArrayAnnotation::addElementReference(const CArrayAnnotation::name_index_type & cnIndex) const
{
  return new CArrayElementReference(cnIndex, this);
}

const CCopasiObject * CArrayAnnotation::addElementReference(const CArrayAnnotation::index_type & index) const
{
  CArrayAnnotation::name_index_type CNIndex(index.size());
  CArrayAnnotation::name_index_type::iterator to = CNIndex.begin();
  CArrayAnnotation::index_type::const_iterator it = index.begin();
  CArrayAnnotation::index_type::const_iterator end = index.end();
  std::vector< std::vector<CRegisteredObjectName> >::const_iterator itCN = mAnnotationsCN.begin();

  for (; it != end; ++it, ++to, ++itCN)
    {
      *to = *it < itCN->size() ? itCN->operator [](*it) : CRegisteredObjectName("");

      if (to->empty())
        {
          *to = StringPrint("%d", *it);
        }
    }

  return addElementReference(CNIndex);
}

const CCopasiObject * CArrayAnnotation::addElementReference(C_INT32 u, C_INT32 v) const
{
  CArrayAnnotation::index_type CIndex(2);

  CIndex[0] = u;
  CIndex[1] = v;

  return addElementReference(CIndex);
}

const CCopasiObject * CArrayAnnotation::addElementReference(C_INT32 u) const
{
  CArrayAnnotation::index_type CIndex(1);

  CIndex[0] = u;

  return addElementReference(CIndex);
}

const CObjectInterface * CArrayAnnotation::getObject(const CCopasiObjectName & cn) const
{
  if (cn == "")
    {
      return this;
    }

  if (cn == "Property=DisplayName")
    {
      return CCopasiObject::getObject(cn);
    }

  //if there are no indices there could still be other children. This can be handled
  //by the container base class
  if (cn.getElementName(0, false) == "") //no indices
    return CCopasiContainer::getObject(cn);

  //first get the index string
  std::string tmp;
  std::string ObjectName;
  std::vector< std::string > DisplayNames;
  C_INT32 ii = 0;

  while ((tmp = cn.getElementName(ii, false)) != "")
    {
      ObjectName += "[" + CCopasiObjectName::escape(tmp) + "]";
      DisplayNames.push_back(tmp);

      ++ii;
    }

  const CCopasiObject* pObject = NULL; //this will contain the element reference

  //if the reference object already exists, its name will be identical to the index
  objectMap::range range = mObjects.equal_range(ObjectName);

  while (range.first != range.second && (*range.first)->getObjectType() != "ElementReference") ++range.first;

  if (range.first != range.second) //not found
    {
      pObject = *range.first;
    }
  else
    {
      pObject = addElementReference(displayNamesToCN(DisplayNames));
    }

  if (pObject)
    return pObject->getObject(cn.getRemainder());
  else
    return NULL;
}

void CArrayAnnotation::print(std::ostream * ostream) const
{*ostream << *this;}

#define SPC(level) std::string(level, ' ')

void CArrayAnnotation::printRecursively(std::ostream & ostream, size_t level,
                                        CCopasiAbstractArray::index_type & index,
                                        const std::vector<std::vector<std::string> > & display) const
{
  size_t indent = 2 * (dimensionality() - 1 - level);

  if (level == 0) //only vector
    {
      ostream << SPC(indent) << "Rows: " << getDimensionDescription(level) << "\n";

      size_t imax = size()[0];

      for (index[0] = 0; index[0] < imax; ++index[0])
        ostream << SPC(indent) << display[0][index[0]] << "\t" << (*array())[index] << "\n";
    }
  else if (level == 1) //matrix
    {
      ostream << SPC(indent) << "Rows:    " << getDimensionDescription(level - 1) << "\n";
      ostream << SPC(indent) << "Columns: " << getDimensionDescription(level) << "\n";

      size_t imax = size()[0];
      size_t jmax = size()[1];
      ostream << SPC(indent);

      for (index[1] = 0; index[1] < jmax; ++index[1])
        ostream << "\t" << display[1][index[1]];

      ostream << "\n";

      for (index[0] = 0; index[0] < imax; ++index[0])
        {
          ostream << SPC(indent) << display[0][index[0]];

          for (index[1] = 0; index[1] < jmax; ++index[1])
            ostream << "\t" << (*array())[index];

          ostream << "\n";
        }
    }
  else //dimensionality > 2
    {
      size_t i, imax = size()[level];

      for (i = 0; i < imax; ++i)
        {
          ostream << SPC(indent) << getDimensionDescription(level) << ": " << display[level][i] << "\n";
          index[level] = i;
          printRecursively(ostream, level - 1, index, display);
        }
    }
}

#undef SPC

std::ostream &operator<<(std::ostream &os, const CArrayAnnotation & o)
{
  if (!o.array()) return os;

  //cache the display names
  std::vector<std::vector<std::string> > displaynames;
  size_t i;

  for (i = 0; i < o.dimensionality(); ++i)
    displaynames.push_back(o.getAnnotationsString(i));

  os << o.getObjectName() << std::endl;
  os << o.getDescription() << std::endl;

  CCopasiAbstractArray::index_type arraysize = o.array()->size();

  if (o.dimensionality() == 0)
    {
      //only one scalar value
      os << (*o.array())[arraysize] << std::endl;
    }
  else
    {
      o.printRecursively(os, o.dimensionality() - 1, arraysize, displaynames);
    }

  return os;
}

bool CArrayAnnotation::isEmpty()
{
  // either the dimension itself ...
  size_t dim = dimensionality();

  if (dim == 0) return false;

  // ... or the size of each dimension should be greater than zero.
  size_t idx = 0;

  for (idx = 0; idx < dim; idx++)
    {
      if (!size()[idx])
        return true;
    }

  return false;
}

std::string CArrayAnnotation::getObjectDisplayName() const
{
  std::string part;

  if (getObjectParent() && getObjectParent()->getObjectType() != "Model")
    part = getObjectParent()->getObjectDisplayName() + ".";

  return part + getObjectName() + "[[]]";
}

CArrayAnnotation::data_type & CArrayAnnotation::operator[](const CArrayAnnotation::name_index_type & nameIndex)
{
  return mpArray->operator[](cnToIndex(nameIndex));
}

const CArrayAnnotation::data_type & CArrayAnnotation::operator[](const CArrayAnnotation::name_index_type & nameIndex) const
{
  return mpArray->operator[](cnToIndex(nameIndex));
}

CCopasiAbstractArray * CArrayAnnotation::getArray() const
{
  return mpArray;
}

CArrayAnnotation::name_index_type CArrayAnnotation::displayNamesToCN(const std::vector< std::string > & DisplayNames) const
{
  assert(DisplayNames.size() == dimensionality()
         || dimensionality() == 0);

  name_index_type CNIndex(dimensionality());
  name_index_type::iterator to = CNIndex.begin();
  std::vector< std::string >::const_iterator it = DisplayNames.begin();
  std::vector< std::string >::const_iterator end = DisplayNames.end();
  std::vector< std::vector<CRegisteredObjectName> >::const_iterator itCNs = mAnnotationsCN.begin();
  size_t dimension = 0;

  for (; it != end && dimension < dimensionality(); ++it, ++itCNs, ++dimension, ++to)
    {
      std::vector<CRegisteredObjectName>::const_iterator itCN = itCNs->begin();
      std::vector<CRegisteredObjectName>::const_iterator endCN = itCNs->end();

      for (; itCN != endCN; ++itCN)
        {
          if (CCopasiObjectName::unescape(*it) == createDisplayName(*itCN))
            {
              *to = *itCN;
              break;
            }
        }

      if (itCN == endCN)
        {
          size_t index = C_INVALID_INDEX;
          bool ValidIndex = strToIndex(*it, index);

          if (ValidIndex)
            {
              if (itCNs->empty())
                {
                  *to = *it;
                }
              else
                {
                  *to = itCNs->operator [](index);
                }
            }
          else
            {
              *to = std::string("not found");
            }
        }
    }

  return CNIndex;
}

CArrayAnnotation::index_type CArrayAnnotation::cnToIndex(const CArrayAnnotation::name_index_type & cnIndex) const
{
  index_type Index(dimensionality(), C_INVALID_INDEX);

  if (cnIndex.size() != Index.size())
    {
      return Index;
    }

  index_type::iterator to = Index.begin();
  std::vector< CRegisteredObjectName >::const_iterator it = cnIndex.begin();
  std::vector< CRegisteredObjectName >::const_iterator itEnd = cnIndex.end();
  std::vector< std::vector<CRegisteredObjectName> >::const_iterator itCNs = mAnnotationsCN.begin();
  size_t index = 0;

  for (; it != itEnd; ++it, ++itCNs, ++to)
    {
      if (!strToIndex(*it, index))
        {
          std::vector<CRegisteredObjectName>::const_iterator itCN = itCNs->begin();
          std::vector<CRegisteredObjectName>::const_iterator endCN = itCNs->end();

          for (index = 0; itCN != endCN; ++itCN, ++index)
            {
              if (*it == *itCN) break;
            }
        }

      *to = index;
    }

  return Index;
}

std::string CArrayAnnotation::createDisplayName(const std::string & cn) const
{
  const CCopasiObject * pObject = CObjectInterface::DataObject(getObjectFromCN(cn));

  if (pObject != NULL)
    {
      return pObject->isStaticString() ? pObject->getObjectName() : pObject->getObjectDisplayName();
    }

  return "not found";
}

void CArrayAnnotation::updateDisplayNames() const
{
  std::vector< std::vector<CRegisteredObjectName> >::const_iterator itCNs = mAnnotationsCN.begin();
  std::vector< std::vector<CRegisteredObjectName> >::const_iterator endCNs = mAnnotationsCN.end();
  std::vector< std::vector< std::string > >::iterator itNames = mAnnotationsString.begin();

  for (; itCNs != endCNs; ++itCNs, ++itNames)
    {
      std::vector<CRegisteredObjectName>::const_iterator itCN = itCNs->begin();
      std::vector<CRegisteredObjectName>::const_iterator endCN = itCNs->end();
      std::vector< std::string >::iterator itName = itNames->begin();

      for (; itCN != endCN; ++itCN, ++itName)
        {
          *itName = createDisplayName(*itCN);
        }
    }
}

// void CArrayAnnotation::printDebugLoop(std::ostream & out, CCopasiAbstractArray::index_type & index, size_t level) const
//   {
//     size_t i, imax = mArray->size()[level];
//     for (i = 0; i < imax; ++i)
//       {
//         index[level] = i;
//
//         if (level == dimensionality() - 1)
//           {
//             out << (*mArray)[index] << "  ";
//}
//         else
//           {
//             printDebugLoop(out, index, level + 1);
//             out << std::endl;
//}
//}
//}
//
// void CArrayAnnotation::printDebug(std::ostream & out) const
//   {
//     out << mDescription << std::endl;
//     out << "  Dimensionality: " << dimensionality() << std::endl;
//
//     size_t i, j;
//     for (i = 0; i < dimensionality(); ++i)
//       {
//         out << "   " << i << ": " << mDimensionDescriptions[i] << std::endl;
//
//         //std::vector<std::string> names = const_cast<CArrayAnnotation*>(this)->getAnnotationsDisplay(i);
//
//         for (j = 0; j < mAnnotations[i].size(); ++j)
//           out << "     " << mAnnotations[i][j] /*<< " :" << names[j]*/ << std::endl;
//
//         out << std::endl;
//}
//
//     CCopasiAbstractArray::index_type index; index.resize(dimensionality());
//     printDebugLoop(out, index, 0);
//}
