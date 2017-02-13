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

// Copyright (C) 2001 - 2007 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc. and EML Research, gGmbH.
// All rights reserved.

#ifndef COPASI_CCopasiVector
#define COPASI_CCopasiVector

#ifdef WIN32
# pragma warning (disable: 4786)
#endif  // WIN32

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <cstddef>

#include "copasi/utilities/CCopasiMessage.h"
#include "copasi/utilities/utility.h"

#include "copasi/report/CCopasiObjectName.h"
#include "copasi/report/CCopasiContainer.h"

#include "../undo/CData.h"
#undef min
#undef max
#undef ERROR

class CReadConfig;

template <class CType> class CCopasiVector;

template <class CType>
std::ostream &operator<<(std::ostream &os, const CCopasiVector<CType> & d);

template < class CType > class CCopasiVector:
  protected std::vector< CType * >, public CCopasiContainer
{
public:
  // typedef typename std::vector< CType * >::value_type value_type;
  // typedef typename std::vector< CType * >::iterator iterator;
  // typedef typename std::vector< CType * >::const_iterator const_iterator;

public:
#ifndef SWIG
  class iterator: public std::vector< CType * >::iterator
  {
  public:
    iterator(): std::vector< CType * >::iterator() {}

    iterator(const iterator & src): std::vector< CType * >::iterator(src) {}

    iterator(const typename std::vector< CType * >::iterator & src): std::vector< CType * >::iterator(src) {}

    ~iterator() {}

    CType & operator*() const
    {
      return *std::vector< CType * >::iterator::operator*();
    }

    CType * operator->() const
    {
      return std::vector< CType * >::iterator::operator*();
    }

    operator CType * () const
    {
      return std::vector< CType * >::iterator::operator*();
    }

    iterator & operator=(CType * pType)
    {
      std::vector< CType * >::iterator::operator*() = pType;
      return *this;
    }

    iterator & operator++()
    {
      std::vector< CType * >::iterator::operator++();
      return *this;
    }

    iterator operator++(int)
    {
      return iterator(std::vector< CType * >::iterator::operator++());
    }

    iterator & operator--()
    {
      std::vector< CType * >::iterator::operator--();
      return *this;
    }

    iterator operator--(int)
    {
      return iterator(std::vector< CType * >::iterator::operator--());
    }

    iterator & operator+=(const typename std::vector< CType * >::iterator::difference_type & n)
    {
      std::vector< CType * >::iterator::operator+=(n);
      return *this;
    }

    iterator operator+(const typename std::vector< CType * >::iterator::difference_type & n) const
    {
      return iterator(std::vector< CType * >::iterator::operator+(n));
    }

    iterator & operator-=(const typename std::vector< CType * >::iterator::difference_type & n)
    {
      std::vector< CType * >::iterator::operator-=(n);
      return *this;
    }

    iterator operator-(const typename std::vector< CType * >::iterator::difference_type & n) const
    {
      return iterator(std::vector< CType * >::iterator::operator-(n));
    }
  };

  class const_iterator: public std::vector< CType * >::const_iterator
  {
  public:
    const_iterator(): std::vector< CType * >::const_iterator() {}

    const_iterator(const const_iterator & src): std::vector< CType * >::const_iterator(src) {}

    const_iterator(const iterator & src): std::vector< CType * >::const_iterator(src) {}

    const_iterator(const typename std::vector< CType * >::const_iterator & src): std::vector< CType * >::const_iterator(src) {}

    ~const_iterator() {}

    const CType & operator*() const
    {
      return *std::vector< CType * >::const_iterator::operator*();
    }

    const CType * operator->() const
    {
      return std::vector< CType * >::const_iterator::operator*();
    }

    operator const CType * () const
    {
      return std::vector< CType * >::const_iterator::operator*();
    }

    const_iterator & operator++()
    {
      std::vector< CType * >::const_iterator::operator++();
      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator Tmp(*this);
      operator++();
      return Tmp;
    }

    const_iterator & operator--()
    {
      std::vector< CType * >::const_iterator::operator--();
      return *this;
    }

    const_iterator operator--(int)
    {
      const_iterator Tmp(*this);
      operator--();
      return Tmp;
    }

    const_iterator & operator+=(const typename std::vector< CType * >::const_iterator::difference_type & n)
    {
      std::vector< CType * >::const_iterator::operator+=(n);
      return *this;
    }

    const_iterator operator+(const typename std::vector< CType * >::const_iterator::difference_type & n) const
    {
      return const_iterator(std::vector< CType * >::const_iterator::operator+(n));
    }

    const_iterator & operator-=(const typename std::vector< CType * >::const_iterator::difference_type & n)
    {
      std::vector< CType * >::const_iterator::operator-=(n);
      return *this;
    }

    const_iterator operator-(const typename std::vector< CType * >::const_iterator::difference_type & n) const
    {
      return const_iterator(std::vector< CType * >::const_iterator::operator-(n));
    }

    CType * constCast()
    {
      return const_cast< CType * >(std::vector< CType * >::const_iterator::operator*());
    }
  };

#endif // SWIG
  // Operations
protected:
  CCopasiVector(const CCopasiVector < CType > & src);

public:
  /**
   * Default constructor
   * @param const const std::string & name (Default: "NoName")
   * @param const CCopasiContainer * pParent (Default: NULL)
   * @param const size_t & flag (Default: flag | CCopasiObject::Vector)
   */
  CCopasiVector(const std::string & name = "NoName",
                const CCopasiContainer * pParent = NO_PARENT,
                const unsigned C_INT32 & flag = CCopasiObject::Vector):
    std::vector< CType * >(),
    CCopasiContainer(name, pParent, "Vector", flag | CCopasiObject::Vector)
  {CONSTRUCTOR_TRACE;}

  /**
   * Copy constructor
   * @param const CCopasiVector < CType > & src
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVector(const CCopasiVector < CType > & src,
                const CCopasiContainer * pParent):
    std::vector< CType * >(src),
    CCopasiContainer(src, pParent)
  {
    CONSTRUCTOR_TRACE;

    size_t i, imax = size();
    iterator Target = begin();
    const_iterator Source = src.begin();

    for (i = 0; i < imax; i++, Target++, Source++)
      {
        try
          {
            Target = new CType(*Source, this);
          }
        catch (...)
          {
            Target = NULL;
          }

        if (Target == NULL)
          CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, imax * sizeof(CType));
      }
  }

  /**
   *  Destructor
   */
  virtual ~CCopasiVector()
  {
    cleanup();
    DESTRUCTOR_TRACE;
  }

  /**
   * Assignment operator. The effect of this operator is that both vectors will
   * share the same objects. However, the parentship of the objects is not affected
   * This means that the assigned vector must be used with some care.
   * @param const CCopasiVector< CType > & rhs
   * @return CCopasiVector< CType > & lhs
   */
  CCopasiVector< CType > & operator = (const CCopasiVector< CType > & rhs)
  {
    cleanup();

    typename std::vector< CType * >::const_iterator it = rhs.begin();
    typename std::vector< CType * >::const_iterator end = rhs.end();

    for (; it != end; ++it)
      add(*it, false);

    return *this;
  }

  /**
   * Assignment operator.
   * @param const std::vector< CType * > & rhs
   * @return CCopasiVector< CType > & lhs
   */
  CCopasiVector< CType > & operator = (const std::vector< CType * > & rhs)
  {
    cleanup();

    typename std::vector< CType * >::const_iterator it = rhs.begin();
    typename std::vector< CType * >::const_iterator end = rhs.end();

    for (; it != end; ++it)
      add(*it, false);

    return *this;
  }

  /**
   * This creates a deep copy of the source, i.e., all objects are copied and
   * the parent of these copies is the current vector.
   * @param const CCopasiVector< CType > & source
   */
  void deepCopy(const CCopasiVector< CType > & source)
  {
    cleanup();
    resize(source.size());

    size_t i, imax = size();
    iterator Target = begin();
    const_iterator Source = source.begin();

    for (i = 0; i < imax; i++, Target++, Source++)
      {
        try
          {
            Target = new CType(*Source, this);
          }
        catch (...)
          {
            Target = NULL;
          }

        if (Target == NULL)
          CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, imax * sizeof(CType));
      }
  }

  iterator begin() {return iterator(std::vector< CType * >::begin());}

  const_iterator begin() const  {return const_iterator(std::vector< CType * >::begin());}

  iterator end()  {return iterator(std::vector< CType * >::end());}

  const_iterator end() const  {return const_iterator(std::vector< CType * >::end());}

  /**
   *  Cleanup
   */
  virtual void cleanup()
  {
    typename std::vector< CType * >::iterator it = std::vector< CType * >::begin();
    typename std::vector< CType * >::iterator End = std::vector< CType * >::end();

    for (; it != End; it++)
      if (*it != NULL &&
          (*it)->getObjectParent() == this)
        {
          CCopasiContainer::remove(*it);
          (*it)->setObjectParent(NULL);
          delete(*it);
          *it = NULL;
        }

    CCopasiVector< CType >::clear();
  }

  /**
   * Add a copy of the object to the end of the vector.
   * @param const CType & src
   * @return bool success.
   */
  virtual bool add(const CType & src)
  {
    CType * pCopy = NULL;

    try
      {
        pCopy = new CType(src, this);
      }

    catch (...)
      {
        pCopy = NULL;
      }

    if (pCopy == NULL)
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, sizeof(CType));

    // This is not very efficient !!!
    // It results in a lot of resizing of the vector !!!
    std::vector< CType * >::push_back(pCopy);
    return CCopasiContainer::add(pCopy, true);
  }

  /**
   * Swap two objects in the vector.
   * @param const size_t & indexFrom
   * @param const size_t & indexTo
   */
  virtual void swap(const size_t & indexFrom, const size_t & indexTo)
  {
    size_t Size = size();

    if (!(indexFrom < Size))
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCCopasiVector + 3, indexFrom, Size - 1);

    if (!(indexTo < Size))
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCCopasiVector + 3, indexTo, Size - 1);

    typename std::vector< CType * >::iterator from = std::vector< CType * >::begin() + indexFrom;
    typename std::vector< CType * >::iterator to = std::vector< CType * >::begin() + indexTo;

    typename std::vector< CType * >::value_type tmp = *from;
    *from = *to;
    *to = tmp;
  }

  /**
   * Add an object to the end of the vector, if adobt is true the vector.
   * becomes the parent of the object.
   * @param CType * src
   * @param const bool & adopt (Default: false)
   * @return bool success
   */
  virtual bool add(CCopasiObject * pObject, const bool & adopt = true)
  {
    // This is not very efficient !!!
    // It results in a lot of resizing of the vector !!!

    CType * pNew = dynamic_cast< CType * >(pObject);

    if (pNew != NULL)
      {
        std::vector< CType * >::push_back(pNew);
      }

    return CCopasiContainer::add(pObject, adopt);
  }

  /**
   *  Removes the index-th element from the vector
   *  @param const size_t & index
   */
  virtual void remove(const size_t & index)
  {
    if (!(index < size()))
      return;

    typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin() + index;

    if (*Target)
      {
        if ((*Target)->getObjectParent() == this)
          {
            //                (*Target)->cleanup();
            delete *Target;
          }
        else
          {
            CCopasiContainer::remove(*Target);
            std::vector< CType * >::erase(Target, Target + 1);
          }
      }
  }

  /**
   * Removes the pointer to the object, from the vector
   * @param CCopasiObject * pObject
   * @return bool success
   */
  virtual bool remove(CCopasiObject * pObject)
  {
    const size_t index = getIndex(pObject);

    bool success = true;

    if (index != C_INVALID_INDEX)
      {
        typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin() + index;
        // Note: erase does not delete pointed to objects
        std::vector< CType * >::erase(Target, Target + 1);
      }
    else
      success = false;

    success &= CCopasiContainer::remove(pObject);

    return success;
  }

  /**
   * Retrieve the indexed object.
   * @param const size_t & index
   * @return const value_type & object
   */
  const CType & operator[](const size_t & index) const
  {
    if (!(index < size()))
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCCopasiVector + 3, index, size() - 1);

    return **(std::vector< CType *>::begin() + index);
  }

  /**
   * Retrieve the indexed object.
   * @param const size_t & index
   * @return value_type & object
   */
  CType & operator[](const size_t & index)
  {
    if (!(index < size()))
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCCopasiVector + 3, index, size() - 1);

    return **(std::vector< CType *>::begin() + index);
  }

  /**
   * Retrieve a the CCopasiObject with the given name.
   * @param const CCopasiObjectName &name
   * @return const CObjectInterface * object
   */
  virtual const CObjectInterface * getObject(const CCopasiObjectName &name) const
  {
    size_t Index = name.getElementIndex();

    if (Index < size())
      {
        CCopasiObject * pObject = *(std::vector< CType * >::begin() + Index);

        if (name.getObjectType() == pObject->getObjectType())
          return pObject; //exact match of type and name

        if (name.getObjectName() == "")
          return pObject; //cn contains no "="; type cannot be checked
      }

    return NULL;
  }

  /**
   *  Retrieves the size of the vector
   *  @return size_t size
   */
  virtual size_t size() const
  {return std::vector< CType * >::size();}

  /**
   *  Resizes the vector but does not create new member objects
   *  @param const size_t & newSize
   */
  virtual void resize(const size_t & newSize)
  {
    size_t OldSize = size();

    if (OldSize == newSize) return; // Nothing to do.

    if (OldSize < newSize)
      {
        std::vector< CType * >::resize(newSize);

        size_t i;
        typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin() + OldSize;

        for (i = OldSize; i < newSize; i++, Target++)
          *Target = NULL;
      }
    else
      {
        typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin() + newSize;
        typename std::vector< CType * >::iterator End = std::vector< CType * >::end();

        for (; Target != End; Target++)
          if (*Target)
            {
              if ((*Target)->getObjectParent() == this)
                {
                  CCopasiContainer::remove(*Target);
                  (*Target)->setObjectParent(NULL);
                  delete *Target;
                }
              else
                CCopasiContainer::remove(*Target);
            }

        std::vector< CType * >::resize(newSize);
      }
  }

  virtual void clear()
  {
    size_t OldSize = size();

    if (OldSize == 0) return; // Nothing to do.

    typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin();
    typename std::vector< CType * >::iterator End = std::vector< CType * >::end();

    for (; Target != End; Target++)
      if (*Target)
        {
          if ((*Target)->getObjectParent() == this)
            {
              CCopasiContainer::remove(*Target);
              (*Target)->setObjectParent(NULL);
              delete *Target;
            }
          else
            CCopasiContainer::remove(*Target);
        }

    std::vector< CType * >::resize(0);
  }

  /**
   * Retrieve the index of the pointed to object in the vector. If the object
   * is not found C_INVALID_INDEX is returned.
   * @param const CCopasiObject * pObject
   * @return size_t index
   */
  virtual size_t getIndex(const CCopasiObject * pObject) const
  {
    size_t i, imax = size();
    typename std::vector< CType * >::const_iterator Target = std::vector< CType * >::begin();

    for (i = 0; i < imax; i++, Target++)
      {
        const CCopasiObject * pTarget = static_cast< const CCopasiObject * >(*Target);

        if (pTarget == pObject)
          {
            return i;
          }
      }

    return C_INVALID_INDEX;
  }

  virtual CCopasiObject * insert(const CData & data)
  {
    CType * pNew = static_cast<CType *>(CType::create(data));

    if (pNew->getObjectType() == data.getProperty(CData::OBJECT_TYPE).toString())
      {
        size_t Index = data.getProperty(CData::OBJECT_INDEX).toUint();
        std::vector< CType * >::insert(std::vector< CType * >::begin() + Index, pNew);

        CCopasiContainer::add(pNew, true);
      }
    else
      {
        delete pNew;
        pNew = NULL;
      }

    return pNew;
  }

  /**
   * ostream operator
   * @param std::ostream & os
   * @param const CCopasiVector<CType> & d
   * @return std::ostream & os
   */
#if defined SWIG
  friend std::ostream &operator << (std::ostream &os,
                                    const CCopasiVector<CType> & d);
#else
#if defined _MSC_VER && _MSC_VER < 1201 // 1200 Identifies Visual C++ 6.0
  friend std::ostream &operator << (std::ostream &os,
                                    const CCopasiVector<CType> & d);
#else
  friend std::ostream &operator << <>(std::ostream &os,
                                      const CCopasiVector<CType> & d);
#endif // WIN32
#endif // SWIG
};

template < class CType > class CCopasiVectorS: public CCopasiVector < CType >
{
public:
  typedef typename std::vector< CType * >::value_type value_type;
  typedef typename std::vector< CType * >::iterator iterator;
  typedef typename std::vector< CType * >::const_iterator const_iterator;

  // Operations
public:
  /**
   * Default constructor
   * @param const std::string & name (Default: "NoName")
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorS(const std::string & name = "NoName",
                 const CCopasiContainer * pParent = NO_PARENT):
    CCopasiVector< CType >(name, pParent) {}

  /**
   * Copy constructor
   * @param const CCopasiVectorS < CType > & src
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorS(const CCopasiVectorS < CType > & src,
                 const CCopasiContainer * pParent) :
    CCopasiVector < CType > (src, pParent) {}

  /**
   *  Destructor
   */
  virtual ~CCopasiVectorS() {}

  /**
   * Loads an object with data coming from a CReadConfig object.
   * (CReadConfig object reads an input stream)
   * @param CReadConfig & configbuffer
   * @param const size_t & size
   */
  virtual void load(CReadConfig & configbuffer, size_t size)
  {
    size_t i;

    CCopasiVector< CType >::cleanup();
    CCopasiVector< CType >::resize(size);

    typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin();

    for (i = 0; i < size; i++, Target++)*Target = NULL;

    for (i = 0, Target = std::vector< CType * >::begin(); i < size; i++, Target++)
      {
        try
          {
            *Target = new CType("NoName", this);
          }
        catch (...)
          {
            *Target = NULL;
          }

        if (*Target == NULL)
          CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, size * sizeof(CType));

        (*Target)->load(configbuffer);
      }
  }
};

template < class CType > class CCopasiVectorN: public CCopasiVector < CType >
{
public:
  // typedef typename std::vector< CType * >::value_type value_type;
  // typedef typename std::vector< CType * >::iterator iterator;
  // typedef typename std::vector< CType * >::const_iterator const_iterator;

  // Operations
public:
  /**
   * Default constructor
   * @param const std::string & name (Default: "NoName")
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorN(const std::string & name = "NoName",
                 const CCopasiContainer * pParent = NO_PARENT):
    CCopasiVector< CType >(name, pParent,
                           CCopasiObject::Container
                           + CCopasiObject::Vector
                           + CCopasiObject::NameVector)
  {}

  /**
   * Copy constructor
   * @param const CCopasiVectorN < CType > & src
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorN(const CCopasiVectorN < CType > & src,
                 const CCopasiContainer * pParent) :
    CCopasiVector < CType > (src, pParent)
  {}

  /**
   *  Destructor
   */
  virtual ~CCopasiVectorN() {}

  /**
   * Assignment operator.
   * @param const std::vector< CType * > & rhs
   * @return CCopasiVectorN< CType > & lhs
   */
  CCopasiVectorN< CType > & operator = (const std::vector< CType * > & rhs)
  {
    CCopasiVector< CType >::operator=(rhs);
    return *this;
  }

  /**
   * Add a copy of the object to the end of the vector.
   * @param const CType & src
   * @return bool success.
   */
  virtual bool add(const CType & src)
  {
    if (!isInsertAllowed(&src))
      {
        CCopasiMessage ex(CCopasiMessage::ERROR,
                          MCCopasiVector + 2, src.getObjectName().c_str());
        return false;
      }

    CType * Element;

    try
      {
        Element = new CType(src, this);
      }
    catch (...)
      {
        Element = NULL;
      }

    if (Element == NULL)
      CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, sizeof(CType));

    std::vector< CType * >::push_back(Element);
    return CCopasiContainer::add(Element, true);
  }

  /**
   * Add an object to the end of the vector, if adobt is true the vector.
   * becomes the parent of the object.
   * @param CType * src
   * @param const bool & adopt (Default: false)
   * @return bool success
   */
  virtual bool add(CCopasiObject * pObject, const bool & adopt = true)
  {
    // This is not very efficient !!!
    // It results in a lot of resizing of the vector !!!

    CType * pNew = dynamic_cast< CType * >(pObject);

    if (pNew != NULL)
      {
        if (!isInsertAllowed(pNew))
          {
            CCopasiMessage ex(CCopasiMessage::ERROR,
                              MCCopasiVector + 2, pNew->getObjectName().c_str());
            return false;
          }

        std::vector< CType * >::push_back(pNew);
      }

    return CCopasiContainer::add(pObject, adopt);
  }

  /**
   * Remove the object with the given name from the vector.
   * @param const std::string & name
   */
  virtual void remove(const std::string & name)
  {
    size_t Index = getIndex(name);

    if (Index == C_INVALID_INDEX)
      {
        CCopasiMessage ex(CCopasiMessage::ERROR,
                          MCCopasiVector + 1, name.c_str());
        return;
      }

    CCopasiVector< CType >::remove(Index);
    return;
  }

  /**
   * Retrieve the indexed object.
   * @param const size_t & index
   * @return value_type & object
   */
  CType & operator[](const size_t & index)
  {return CCopasiVector< CType >::operator[](index);}

  /**
   * Retrieve the indexed object.
   * @param const size_t & index
   * @return const value_type & object
   */
  const CType & operator[](const size_t & index) const
  {return CCopasiVector< CType >::operator[](index);}

  /**
   * Retrieve the named object.
   * @param const std::string & name
   * @return value_type & object
   */
  CType & operator[](const std::string & name)
  {
    CCopasiContainer::objectMap::range Range = CCopasiContainer::getObjects().equal_range(name);

    CType * pType = NULL;

    for (; Range.first != Range.second && pType == NULL; ++Range.first)
      {
        pType = dynamic_cast< CType * >(*Range.first);
      }

    if (pType == NULL)
      {
        CCopasiMessage ex(CCopasiMessage::EXCEPTION,
                          MCCopasiVector + 1, name.c_str());
      }

    return *pType;
  }

  /**
   * Retrieve the named object.
   * @param const std::string & name
   * @return const value_type & object
   */
  const CType & operator[](const std::string &name) const
  {
    CCopasiContainer::objectMap::range Range = CCopasiContainer::getObjects().equal_range(name);

    CType * pType = NULL;

    for (; Range.first != Range.second && pType == NULL; ++Range.first)
      {
        pType = dynamic_cast< CType * >(*Range.first);
      }

    if (pType == NULL)
      {
        CCopasiMessage ex(CCopasiMessage::EXCEPTION,
                          MCCopasiVector + 1, name.c_str());
      }

    return *pType;
  }

  /**
   * Retrieve a pointer to the named object.
   * @param const std::string & name
   * @return const CObjectInterface * pObject
   */
  virtual const CObjectInterface * getObject(const CCopasiObjectName &name) const
  {
    CCopasiContainer::objectMap::range Range = CCopasiContainer::getObjects().equal_range(name.getElementName(0));

    for (; Range.first != Range.second; ++Range.first)
      {
        if (dynamic_cast< CType * >(*Range.first) != NULL)
          return *Range.first;
      }

    return NULL;
  }

  /**
   * Retrieve the index of the named object in the vector. If an object with the
   * given name is not found C_INVALID_INDEX is returned.
   * @param const std::string & name
   * @return size_t index
   */
  virtual size_t getIndex(const std::string &name) const
  {
    size_t i, imax = CCopasiVector< CType >::size();
    typename std::vector< CType * >::const_iterator Target = std::vector< CType * >::begin();

    std::string Name = unQuote(name);

    for (i = 0; i < imax; i++, Target++)
      if (*Target &&
          ((*Target)->getObjectName() == name ||
           (*Target)->getObjectName() == Name)) return i;

    return C_INVALID_INDEX;
  }

private:
  /**
   * Check whether an insert of the given src into the vector is allowed
   * to avoid naming conflicts.
   * @param const CType * src
   * @return bool insertAllowed
   */
  virtual bool isInsertAllowed(const CType * src)
  {
    bool isInserAllowed = true;
    CCopasiContainer::objectMap::range Range = CCopasiContainer::getObjects().equal_range(src->getObjectName());

    for (; Range.first != Range.second && isInserAllowed; ++Range.first)
      {
        if (dynamic_cast< CType * >(*Range.first) != NULL)
          isInserAllowed = (CCopasiVector< CType >::getIndex(src) == C_INVALID_INDEX);
      }

    return isInserAllowed;
  }
};

template < class CType > class CCopasiVectorNS: public CCopasiVectorN < CType >
{
  // Operations
public:
  /**
   * Default constructor
   * @param const std::string & name (Default: "NoName")
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorNS(const std::string & name = "NoName",
                  const CCopasiContainer * pParent = NO_PARENT):
    CCopasiVectorN< CType >(name, pParent)
  {}

  /**
   * Copy constructor
   * @param const CCopasiVectorNS < CType > & src
   * @param const CCopasiContainer * pParent (Default: NULL)
   */
  CCopasiVectorNS(const CCopasiVectorNS < CType > & src,
                  const CCopasiContainer * pParent) :
    CCopasiVectorN< CType >(src, pParent) {}

  /**
   *  Destructor
   */
  virtual ~CCopasiVectorNS() {}

  /**
   *  Loads an object with data coming from a CReadConfig object.
   *  (CReadConfig object reads an input stream)
   *  @param CReadConfig & configbuffer
   *  @param const size_t & size
   */
  virtual void load(CReadConfig & configbuffer, size_t size)
  {
    size_t i;

    CCopasiVector< CType >::cleanup();
    CCopasiVector< CType >::resize(size);

    typename std::vector< CType * >::iterator Target = std::vector< CType * >::begin();

    for (i = 0; i < size; i++, Target++) *Target = NULL;

    for (i = 0, Target = std::vector< CType * >::begin(); i < size; i++, Target++)
      {
        try
          {
            *Target = new CType("NoName", this);
          }
        catch (...)
          {
            *Target = NULL;
          }

        if (*Target == NULL)
          CCopasiMessage ex(CCopasiMessage::EXCEPTION, MCopasiBase + 1, size * sizeof(CType));

        (*Target)->load(configbuffer);
      }
  }
};

template <class CType>
std::ostream &operator<<(std::ostream &os, const CCopasiVector<CType> & d)
{
  os << "   +++Vektor;  size: " << d.size() << std::endl;

  unsigned int i;

  for (i = 0; i < d.size(); i++)
    os << "   " << d[i];

  if (d.size() == 0)
    os << "   empty" << std::endl;

  os << "   ---Vektor" << std::endl;

  return os;
}

#endif // COPASI_CCopasiVector
