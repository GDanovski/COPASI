/*
 * This file was automaticly generated by clo++ version 0.6.1 (REL-0_6_1)
 * http://pmade.org/pjones/software/clo++/
 *
 * Template enhancements by Stefan Hoops mailto:shoops@vt.edu
 */

/** @file
 * This file contains the implementation of the COptionParser class.
 * You can control the name of this source file using the
 * cxx_source_file variable.
 **/

#ifdef WIN32
#define strcasecmp _stricmp
#endif

#include "COptionParser.h"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <sstream>

namespace
  {
  const char const_usage[] =
    "  --home string              Your home directory.\n"
    "  --rc string                The configuration file for copasi.\n"
    "  --systemFunctionDB string  A write protected database of kinetic\n"
    "                             functions.\n"
    "  --userFunctionDB string    The user extensible database of kinetic\n"
    "                             functions.\n"
    "  -b, --bool bool            A bool test.\n"
    "  -c, --copasidir string     The location of suplementary files for copasi.\n"
    "  -d, --default string       The SBML file to export.\n"
    "  -e, --exportSBML string    The SBML file to export.\n"
    "  -f, --flag                 A flag test.\n"
    "  -i, --importSBML string    A SBML file to import.\n"
    "  -s, --save string          The file the model is saved in after work.\n";

  const char const_help_comment[] =
    "use the -h option for help";

  const char* expand_long_name (const std::string &name);
}

//#########################################################################
copasi::COptionParser::COptionParser (void)
    : state_(state_option)
{
  memset(&locations_, 0, sizeof(locations_));
}
//#########################################################################
void copasi::COptionParser::parse(int argc, char *argv[], bool call_finalize)
{
  for (int i = 1; i < argc; ++i) parse_element(argv[i], i, source_cl);
  if (call_finalize) finalize();
}
//#########################################################################
void copasi::COptionParser::parse(const char * fileName)
{
  int LineCounter = 0;
  char LineBuffer[1024];
  std::string Line;
  std::string Option;
  std::string Value;
  std::string::size_type pos;
  std::ifstream File(fileName);

  if (File.fail())
    {
      std::ostringstream error;
      error << "error opening file: '" << fileName << "'";

      throw option_error(error.str());
    }

  while (true)
    {
      try
        {
          File.getline(LineBuffer, 1024);
          LineCounter++;

          if (File.eof()) break;
          if (File.fail())
            {
              std::ostringstream error;
              error << "unknown problem";

              throw option_error(error.str());
            }

          Line = LineBuffer;

          // eat leading spaces
          pos = Line.find_first_not_of(' ');
          if (pos == std::string::npos)
            Line.erase();
          else
            Line = Line.substr(pos);

          // skip comments and empty lines
          if (Line.length() == 0 || Line[0] == '#') continue;

          // find first position of '=' or ':'
          pos = Line.find_first_of(":=");

          Option = "--" + Line.substr(0, Line.find_last_not_of(' ', pos - 1) + 1);
          parse_element(Option.c_str(), 0, source_cf);

          if (pos != std::string::npos) // We have a '='
            {
              pos = Line.find_first_not_of(' ', pos + 1);
              if (pos == std::string::npos)
                Value.erase();
              else
                Value = Line.substr(pos, Line.find_last_not_of(' ') - pos + 1);

              if (Value.length()) parse_element(Value.c_str(), 0, source_cf);
            }

          finalize();
        }

      catch (copasi::option_error &e)
        {
          std::ostringstream error;
          error << "error reading file: '" << fileName << "(" << LineCounter
          << ")': " << e.what();

          throw option_error(error.str());
        }
    }

  File.clear();
  File.close();
}
//#########################################################################
void copasi::COptionParser::finalize (void)
{
  if (state_ == state_value)
    {
      switch (openum_)
        {
        case option_Bool:
          throw option_error("missing value for 'bool' option");
        case option_ConfigFile:
          throw option_error("missing value for 'rc' option");
        case option_CopasiDir:
          throw option_error("missing value for 'copasidir' option");
        case option_Default:
          throw option_error("missing value for 'default' option");
        case option_ExportSBML:
          throw option_error("missing value for 'exportSBML' option");
        case option_Flag:
          throw option_error("missing value for 'flag' option");
        case option_Home:
          throw option_error("missing value for 'home' option");
        case option_ImportSBML:
          throw option_error("missing value for 'importSBML' option");
        case option_SystemFunctionDB:
          throw option_error("missing value for 'systemFunctionDB' option");
        case option_UserFunctionDB:
          throw option_error("missing value for 'userFunctionDB' option");
        case option_save:
          throw option_error("missing value for 'save' option");
        }
    }
}
//#########################################################################
void copasi::COptionParser::parse_element (const char *element, int position, opsource source)
{
  size_t length = strlen(element);

  switch (state_)
    {
    case state_consume:
      non_options_.push_back(element);
      break;
    case state_option:
      if (length >= 2 && element[0] == '-' && element[1] == '-')
        {
          if (length == 2) {state_ = state_consume; return;}
          element += 2;
          const char *value = element;
          while (*value != 0 && *value != '=') ++value;
          if (*value == '=')
            {
              std::string selement(element, value - element), svalue(++value);

              parse_long_option(selement.c_str(), position, source);
              if (state_ != state_value)
                {
                  std::string error("the '"); error += element; error += "' option does not take a value";
                  throw option_error(error);
                }

              parse_value(svalue.c_str());
              state_ = state_option;
            }
          else
            {
              parse_long_option(element, position, source);
            }
        }
      else if (length >= 2 && element[0] == '-')
        {
          ++element;

          if (length > 2)
            {
              while (*element != 0)
                {
                  parse_short_option(*element, position, source);
                  ++element;

                  if (state_ == state_value && *element == '=')
                    {
                      parse_value(++element);
                      state_ = state_option;
                      break;
                    }
                  else if (state_ == state_value) finalize();
                }
            }
          else
            {
              parse_short_option(*element, position, source);
            }
        }
      else
        {
          non_options_.push_back(element);
        }
      break;
    case state_value:
      parse_value(element);
      state_ = state_option;
      break;
    }
}
//#########################################################################
void copasi::COptionParser::parse_short_option (char option, int position, opsource source)
{
  switch (option)
    {
    case 'b':
      if (locations_.Bool)
        {
          throw option_error("the 'bool' option is only allowed once");
        }
      if (locations_.Flag)
        {
          throw option_error("the flag and bool options are mutually exclusive");
        }

      openum_ = option_Bool;
      state_ = state_value;
      locations_.Bool = position;
      return;
    case 'c':
      if (source != source_cl) break;
      if (locations_.CopasiDir)
        {
          throw option_error("the 'copasidir' option is only allowed once");
        }
      openum_ = option_CopasiDir;
      state_ = state_value;
      locations_.CopasiDir = position;
      return;
    case 'd':
      openum_ = option_Default;
      state_ = state_value;
      locations_.Default = position;
      return;
    case 'e':
      if (source != source_cl) break;
      if (locations_.ExportSBML)
        {
          throw option_error("the 'exportSBML' option is only allowed once");
        }
      openum_ = option_ExportSBML;
      state_ = state_value;
      locations_.ExportSBML = position;
      return;
    case 'f':
      if (source != source_cl) break;
      if (locations_.Bool)
        {
          throw option_error("the bool and flag options are mutually exclusive");
        }

      openum_ = option_Flag;
      options_.Flag = !options_.Flag;
      locations_.Flag = position;
      return;
    case 'i':
      if (source != source_cl) break;
      if (locations_.ImportSBML)
        {
          throw option_error("the 'importSBML' option is only allowed once");
        }
      openum_ = option_ImportSBML;
      state_ = state_value;
      locations_.ImportSBML = position;
      return;
    case 's':
      if (locations_.save)
        {
          throw option_error("the 'save' option is only allowed once");
        }
      openum_ = option_save;
      state_ = state_value;
      locations_.save = position;
      return;
    case 'h':
      if (source != source_cl) break;
      throw autoexcept(autothrow_help, const_usage);
    case '?':
      if (source != source_cl) break;
      throw autoexcept(autothrow_help, const_usage);
    }

  std::string error("unknown option: '"); error += option; error += "'";
  throw option_error(error);
}
//#########################################################################
void copasi::COptionParser::parse_long_option (const char *option, int position, opsource source)
{
  option = expand_long_name(option);

  if (strcmp(option, "bool") == 0)
    {
      if (locations_.Bool)
        {
          throw option_error("the 'bool' option is only allowed once");
        }
      if (locations_.Flag)
        {
          throw option_error("the flag and bool options are mutually exclusive");
        }

      openum_ = option_Bool;
      locations_.Bool = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "copasidir") == 0)
    {
      if (locations_.CopasiDir)
        {
          throw option_error("the 'copasidir' option is only allowed once");
        }
      openum_ = option_CopasiDir;
      locations_.CopasiDir = position;
      state_ = state_value;
      return;
    }
  else if (strcmp(option, "default") == 0)
    {
      openum_ = option_Default;
      locations_.Default = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "exportSBML") == 0)
    {
      if (locations_.ExportSBML)
        {
          throw option_error("the 'exportSBML' option is only allowed once");
        }
      openum_ = option_ExportSBML;
      locations_.ExportSBML = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "flag") == 0)
    {
      if (locations_.Bool)
        {
          throw option_error("the bool and flag options are mutually exclusive");
        }

      openum_ = option_Flag;
      locations_.Flag = position;
      options_.Flag = !options_.Flag;
      return;
    }
  else if (source == source_cl && strcmp(option, "home") == 0)
    {
      if (locations_.Home)
        {
          throw option_error("the 'home' option is only allowed once");
        }
      openum_ = option_Home;
      locations_.Home = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "importSBML") == 0)
    {
      if (locations_.ImportSBML)
        {
          throw option_error("the 'importSBML' option is only allowed once");
        }
      openum_ = option_ImportSBML;
      locations_.ImportSBML = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "rc") == 0)
    {
      if (locations_.ConfigFile)
        {
          throw option_error("the 'rc' option is only allowed once");
        }
      openum_ = option_ConfigFile;
      locations_.ConfigFile = position;
      state_ = state_value;
      return;
    }
  else if (strcmp(option, "save") == 0)
    {
      if (locations_.save)
        {
          throw option_error("the 'save' option is only allowed once");
        }
      openum_ = option_save;
      locations_.save = position;
      state_ = state_value;
      return;
    }
  else if (strcmp(option, "systemFunctionDB") == 0)
    {
      if (locations_.SystemFunctionDB)
        {
          throw option_error("the 'systemFunctionDB' option is only allowed once");
        }
      openum_ = option_SystemFunctionDB;
      locations_.SystemFunctionDB = position;
      state_ = state_value;
      return;
    }
  else if (strcmp(option, "userFunctionDB") == 0)
    {
      if (locations_.UserFunctionDB)
        {
          throw option_error("the 'userFunctionDB' option is only allowed once");
        }
      openum_ = option_UserFunctionDB;
      locations_.UserFunctionDB = position;
      state_ = state_value;
      return;
    }
  else if (source == source_cl && strcmp(option, "help") == 0)
    {
      throw autoexcept(autothrow_help, const_usage);
    }

  std::string error("unknown option '"); error += option; error += "'";
  throw option_error(error);
}
//#########################################################################
void copasi::COptionParser::parse_value (const char *value)
{
  switch (openum_)
    {
    case option_Bool:
      {
        bool bvalue;

        if (strcasecmp(value, "true") == 0)
          {
            bvalue = true;
          }
        else if (strcasecmp(value, "yes") == 0)
          {
            bvalue = true;
          }
        else if (strcasecmp(value, "false") == 0)
          {
            bvalue = false;
          }
        else if (strcasecmp(value, "no") == 0)
          {
            bvalue = false;
          }
        else
          {
            std::string error("invalid boolean value '"); error += value; error += "'";
            throw option_error(error);
          }

        options_.Bool = bvalue;
      }
      break;
    case option_ConfigFile:
      {
        options_.ConfigFile = value;
      }
      break;
    case option_CopasiDir:
      {
        options_.CopasiDir = value;
      }
      break;
    case option_Default:
      {
        std::string svalue(value);
        std::string::size_type eqpos = svalue.find('=');
        if (eqpos == std::string::npos)
          {
            throw option_error("invalid key=value pair for the 'default' option");
          }

        std::string k(svalue.substr(0, svalue.find_last_not_of(' ', eqpos - 1) + 1));
        std::string v(svalue.substr(svalue.find_first_not_of(' ', eqpos + 1)));
        v = v.substr(0, v.find_last_not_of(' ') + 1);
        value = v.c_str();

        options_.Default[k] = v;
      }
      break;
    case option_ExportSBML:
      {
        options_.ExportSBML = value;
      }
      break;
    case option_Flag:
      break;
    case option_Home:
      {
        options_.Home = value;
      }
      break;
    case option_ImportSBML:
      {
        options_.ImportSBML = value;
      }
      break;
    case option_SystemFunctionDB:
      {
        options_.SystemFunctionDB = value;
      }
      break;
    case option_UserFunctionDB:
      {
        options_.UserFunctionDB = value;
      }
      break;
    case option_save:
      {
        options_.save = value;
      }
      break;
    }
}
//#########################################################################
const char* copasi::option_error::get_help_comment (void) const {
  return const_help_comment;
}
//#########################################################################
namespace
  {
  const char* expand_long_name (const std::string &name)
  {
    std::string::size_type name_size = name.size();
    std::vector<const char*> matches;

    if (name_size <= 4 && name.compare("bool") == 0)
      matches.push_back("bool");

    if (name_size <= 9 && name.compare("copasidir") == 0)
      matches.push_back("copasidir");

    if (name_size <= 7 && name.compare("default") == 0)
      matches.push_back("default");

    if (name_size <= 10 && name.compare("exportSBML") == 0)
      matches.push_back("exportSBML");

    if (name_size <= 4 && name.compare("flag") == 0)
      matches.push_back("flag");

    if (name_size <= 4 && name.compare("home") == 0)
      matches.push_back("home");

    if (name_size <= 10 && name.compare("importSBML") == 0)
      matches.push_back("importSBML");

    if (name_size <= 2 && name.compare("rc") == 0)
      matches.push_back("rc");

    if (name_size <= 4 && name.compare("save") == 0)
      matches.push_back("save");

    if (name_size <= 16 && name.compare("systemFunctionDB") == 0)
      matches.push_back("systemFunctionDB");

    if (name_size <= 14 && name.compare("userFunctionDB") == 0)
      matches.push_back("userFunctionDB");

    if (name_size <= 4 && name.compare("help") == 0)
      matches.push_back("help");

    if (matches.empty())
      {
        std::string error("unknown option '"); error += name; error += "'";
        throw copasi::option_error(error);
      }

    if (matches.size() == 1)
      {
        return matches[0];
      }

    std::string error("the option name '"); error += name; error += "' is ambiguous";
    throw copasi::option_error(error);
  }
} // end anonymous namespace
