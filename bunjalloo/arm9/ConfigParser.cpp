#include "Language.h"
#include "ConfigParser.h"
#include "Config.h"

using std::string;

void ConfigParser::parseMiddle(std::string & middle)
{
  // can be of the form:
  //   <empty>
  //   Word
  //   <something>=<something>?<value>[:<value>]
  if (middle.empty())
  {
    // trivial
    return;
  }
  // look for a ?
  size_t pos = middle.find('?');
  if (pos == string::npos)
  {
    // check for cfg: value
    if (middle.find("cfg:")==0)
    {
      string resource = middle.substr(4, middle.length()-4);
      string value;
      middle.clear();
      if ( m_config.resource(resource, value) )
      {
        middle = value;
      }
    }
    return;
  }
  else
  {
    // found - now split if part from then/else part
    string condition(middle.substr(0,pos));
    ParameterSet set(condition, '|');
    const KeyValueMap & kv(set.keyValueMap());
    if (kv.empty())
    {
      return;
    }
    bool isTrue(false);
    // else see if true - can use | to separate conditions
    for (KeyValueMap::const_iterator it(kv.begin()); it != kv.end(); ++it)
    {
      string tmp;
      if (m_config.resource(it->first, tmp) and tmp == it->second)
      {
        // condition fulfilled
        isTrue = true;
        break;
      }
    }
    // now get the values - if no else exists, then set middle to ""
    string values(middle.substr(pos+1, middle.length()-pos));
    size_t sep(values.find(':'));
    string trueVal, falseVal;
    if (sep != string::npos)
    {
      trueVal = values.substr(0,sep);
      falseVal = values.substr(sep+1, values.length()-sep);
    }
    else
    {
      trueVal = values;
      falseVal.clear();
    }

    if (isTrue)
    {
      middle = trueVal;
    }
    else
    {
      middle = falseVal;
    }
  }
}

void ConfigParser::replaceMarkers(std::string & line, const char marker)
{
  // find %Foo% and replace it with its localised value, or Foo if none found.
  bool found(true);
  while (found)
  {
    found = false;
    size_t pos = line.find(marker);
    if (pos != string::npos)
    {
      // look for the end of it
      size_t endpos = line.find(marker, pos+1);
      if (endpos != string::npos)
      {
        // subs pos->endpos with the bit in the middle
        string a = line.substr(0, pos);
        string middle = line.substr(pos+1, endpos-pos-1);
        // see if the middle is simple, or if it needs computing
        parseMiddle(middle);
        a += unicode2string(T(middle), true).c_str();
        a += line.substr(endpos+1, line.length()-endpos-1);
        line = a;
        found = true;
      }
    }
  }
}

ConfigParser::ConfigParser(Config & config)
  : m_config(config)
{}
