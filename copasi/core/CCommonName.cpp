// Copyright (C) 2017 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and University of
// of Connecticut School of Medicine.
// All rights reserved.

/**
 * Class CCommonName
 *
 * This class is the class for handling Copasi object names.
 *
 * Copyright Stefan Hoops 2002
 */

//TODO what does getName() vs. getObjectName do?

#include <sstream>

#include "copasi.h"
#include "CCommonName.h"
#include "copasi/utilities/utility.h"

using std::string;

CCommonName::CCommonName():
  string()
{}

CCommonName::CCommonName(const std::string & name):
  string(name)
{}

CCommonName::CCommonName(const CCommonName & src):
  string(src)
{}

CCommonName::~CCommonName()
{}

CCommonName CCommonName::getPrimary() const
{return substr(0, findEx(","));}

CCommonName CCommonName::getRemainder() const
{
  if (empty()) return CCommonName();

  std::string Separator = ",";

  if (at(0) != '[')
    {
      Separator += "[";
    }

  std::string::size_type pos = findEx(Separator);

  if (pos == std::string::npos) return CCommonName();

  if (at(pos) == ',') pos++;

  return substr(pos);
}

std::string CCommonName::getObjectType() const
{
  CCommonName Primary(getPrimary());

  return CCommonName::unescape(Primary.substr(0, Primary.findEx("=")));
}

std::string CCommonName::getObjectName() const
{
  CCommonName Primary = getPrimary();
  std::string::size_type pos = Primary.findEx("=");

  if (pos == std::string::npos) return "";

  CCommonName tmp = Primary.substr(pos + 1);

  if (getObjectType() != "String")
    {
      tmp = tmp.substr(0, tmp.findEx("["));
    }

  return CCommonName::unescape(tmp);
}

size_t CCommonName::getElementIndex(const size_t & pos) const
{
  size_t Index = C_INVALID_INDEX;

  if (strToIndex(getElementName(pos), Index))
    {
      return Index;
    }

  return C_INVALID_INDEX;
}

std::string CCommonName::getElementName(const size_t & pos,
                                        const bool & unescape) const
{
  CCommonName Primary = getPrimary();

  std::string::size_type open = Primary.findEx("[");
  size_t i;

  for (i = 0; i < pos && open != std::string::npos; i++)
    open = Primary.findEx("[", open + 1);

  std::string::size_type close = Primary.findEx("]", open + 1);

  if (open == std::string::npos || close == std::string::npos) return "";

  if (unescape)
    return CCommonName::unescape(Primary.substr(open + 1,
                                 close - open - 1));

  return Primary.substr(open + 1, close - open - 1);
}

std::string CCommonName::escape(const std::string & name)
{
#define toBeEscaped "\\[]=,>"
  std::string Escaped(name);
  std::string::size_type pos = Escaped.find_first_of(toBeEscaped);

  while (pos != std::string::npos)
    {
      Escaped.insert(pos, "\\");
      pos += 2;
      pos = Escaped.find_first_of(toBeEscaped, pos);
    }

  return Escaped;
#undef toBeEscaped
}

std::string CCommonName::unescape(const std::string & name)
{
  std::string Unescaped(name);
  std::string::size_type pos = Unescaped.find("\\");

  while (pos != std::string::npos)
    {
      Unescaped.erase(pos, 1);
      pos++;
      pos = Unescaped.find("\\", pos);
    }

  return Unescaped;
}

std::string::size_type
CCommonName::findEx(const std::string & toFind,
                    const std::string::size_type & pos) const
{
  std::string::size_type where = find_first_of(toFind, pos);

  std::string::size_type tmp;

  while (where && where != std::string::npos)
    {
      tmp = find_last_not_of("\\", where - 1);

      if ((where - tmp) % 2)
        return where;

      where = find_first_of(toFind, where + 1);
    }

  return where;
}
