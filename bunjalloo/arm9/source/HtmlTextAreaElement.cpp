#include "HtmlTextAreaElement.h"

static const int MINIMUM_ROWS(4);
static const int MINIMUM_COLS(10);
void HtmlTextAreaElement::setAttribute(const std::string & name, 
    const UnicodeString & value)
{
  HtmlElement::setAttribute(name, value);
  if (name == "rows")
  {
    m_rows = strtol( unicode2string(value).c_str(), 0, 10);
  }
  if (name == "cols")
  {
    m_cols = strtol( unicode2string(value).c_str(), 0, 10);
  }
}
int HtmlTextAreaElement::rows() const
{
  if (m_rows < MINIMUM_ROWS)
  {
    return MINIMUM_ROWS;
  }
  return m_rows;
}

int HtmlTextAreaElement::cols() const
{
  if (m_cols < MINIMUM_COLS)
  {
    return MINIMUM_COLS;
  }
  return m_cols;
}
