#include "UnicodeString.h"
#include "ParameterSet.h"

using namespace std;

class PSImpl
{
  friend class ParameterSet;
  public:
    PSImpl(const string & scskvs):
      m_state(BEFORE_NAME),
      m_position(scskvs.begin()),
      m_end(scskvs.end())
    {
      if (m_position != m_end) {
        next();
        doParse();
      }
    }

  private:
    enum ParseState {
      BEFORE_NAME,
      IN_NAME,
      AFTER_NAME,
      BEFORE_VALUE,
      VALUE_DOUBLE_QUOTE,
      VALUE_SINGLE_QUOTE,
      VALUE_UNQUOTED
    };
    ParseState m_state;
    string::const_iterator m_position;
    string::const_iterator m_end;
    char m_value;
    KeyValueMap m_keyValueMap;
    string m_paramName;
    string m_paramValue;

    inline void next()
    {
      m_value = *m_position;
      ++m_position;
    }
    void doParse();
    void cleanup();
    void addCurrentParam();

    void beforeName();
    void inName();
    void afterName();
    void beforeValue();
    void inValue();

};

void PSImpl::beforeName()
{
  if (m_value != ';' and not isWhitespace(m_value))
  {
    m_paramName += m_value;
    m_state = IN_NAME;
  }
}
void PSImpl::inName()
{
  if (not isWhitespace(m_value) and m_value != '=' and m_value != ';')
  {
    m_paramName += m_value;
  }
  else if (m_value == '=')
  {
    m_state = BEFORE_VALUE;
  }
  else if (m_value == ';')
  {
    addCurrentParam();
  }
  else
  {
    m_state = AFTER_NAME;
  }
}

void PSImpl::afterName()
{
  if (m_value == '=')
  {
    m_state = BEFORE_VALUE;
  }
}

void PSImpl::beforeValue()
{
  if (not isWhitespace(m_value))
  {
    if (m_value == '"')
    {
      m_state = VALUE_DOUBLE_QUOTE;
    }
    else if (m_value == '\'')
    {
      m_state = VALUE_SINGLE_QUOTE;
    }
    else
    {
      m_paramValue += m_value;
      m_state = VALUE_UNQUOTED;
    }
  }
}

void PSImpl::addCurrentParam()
{
  m_keyValueMap[m_paramName] = m_paramValue;
  m_paramName.clear();
  m_paramValue.clear();
  m_state = BEFORE_NAME;
}

void PSImpl::inValue()
{
  if (m_value == ';')
  {
    addCurrentParam();
  }
  else if (not isWhitespace(m_value))
  {
    // TODO - check escaped quotes
    char quote = m_state==VALUE_DOUBLE_QUOTE?'"':(m_state==VALUE_SINGLE_QUOTE?'\'':0);
    if (m_value == quote)
    {
      addCurrentParam();
    }
    else
    {
      m_paramValue += m_value;
    }
  }
  else 
  {
    // whitespace
    /*if (m_state == VALUE_UNQUOTED)
    {
      addCurrentParam();
    }
    else
    {
    */
      m_paramValue += m_value;
    //}
  }
}

void PSImpl::doParse()
{
  for (;m_position != m_end; next())
  {
    if (m_value == '\r' or m_value == '\n')
    {
      break;
    }
    switch (m_state)
    {
      case BEFORE_NAME:
        beforeName();
        break;
      case IN_NAME:
        inName();
        break;
      case AFTER_NAME:
        afterName();
        break;
      case BEFORE_VALUE:
        beforeValue();
        break;
      case VALUE_DOUBLE_QUOTE:
      case VALUE_SINGLE_QUOTE:
      case VALUE_UNQUOTED:
        inValue();
        break;
    }
  }
  cleanup();
}

void PSImpl::cleanup()
{
  if ( not m_paramName.empty() )
    addCurrentParam();
}

ParameterSet::ParameterSet(const std::string & semiColonSeperatedKeyValueString):
  m_psImpl(new PSImpl(semiColonSeperatedKeyValueString))
{
}

bool ParameterSet::hasParameter(const std::string & key) const
{
  KeyValueMap::const_iterator it(m_psImpl->m_keyValueMap.find(key));
  KeyValueMap::const_iterator end(m_psImpl->m_keyValueMap.end());
  return (it != end);
}

bool ParameterSet::parameter(const std::string & key, std::string & value) const
{
  KeyValueMap::const_iterator it(m_psImpl->m_keyValueMap.find(key));
  KeyValueMap::const_iterator end(m_psImpl->m_keyValueMap.end());
  if (it != end)
  {
    value = it->second;
  }
  return (it != end);
}

const KeyValueMap & ParameterSet::keyValueMap() const
{
  return m_psImpl->m_keyValueMap;
}
