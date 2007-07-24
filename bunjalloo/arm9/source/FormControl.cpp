#include "Canvas.h"
#include "URI.h"
#include "HtmlElement.h"
#include "ControllerI.h"
#include "Palette.h"
#include "FormControl.h"
#include "TextArea.h"
//#include "md5.h"

using nds::Canvas;
using nds::Color;
const int FormControl::MAX_SIZE(120);
const int FormControl::MIN_SIZE(8);

FormControl::FormControl(HtmlElement * element)
  : m_size(new nds::Rectangle), m_element(element)
{
  m_size->x = 0;
  m_size->y = 0;
  m_size->w = 1;
  m_size->h = 1;
}

FormControl::~FormControl()
{
  delete m_size;
}

void FormControl::setPosition(int x, int y)
{
  m_size->x = x;
  m_size->y = y;
}

void FormControl::setHeight(int h)
{
  m_size->h = h;
}
int FormControl::width() const
{
  return m_size->w+MIN_SIZE;
}

bool FormControl::hitTest(int x, int y) const
{
  return m_size->hit(x,y);
}

void FormControl::draw(TextArea * gfx)
{
  Canvas::instance().drawRectangle(m_size->x, m_size->y, m_size->w, m_size->h, Color(15,25,20));
}

FormControl::InputType FormControl::inputType() const
{
  return ONE_CLICK;
}

void FormControl::input(const UnicodeString & str)
{
}

#if 0
void FormControl::walkForm(const HtmlElement * formElement)
{
  UnicodeString name = formElement->attribute("name");
  if (not name.empty() )
  {
    UnicodeString value = formElement->attribute("value");
    m_processedData += 
  }
  if (formElement->hasChildren())
  {
    ElementList::const_iterator it(formElement->children());
    for (; it != formElement->children().end(); ++it)
    {
      const HtmlElement * element(*it);
      if (element)
        walkForm(element);
    }
  }
}

static void addBoundary(std::string & processedData, unsigned char boundary[16])
{
  for (int i = 0; i < 16; ++i) {
    char buffer[3];
    sprintf(buffer,"%02x",boundary[i]);
    processedData += buffer;
  }
  processedData += "\r\n";
}
#endif


void FormControl::input(int x, int y, ControllerI & controller, URI & uri)
{
  // need to walk up m_element until we find the form father or the html element.
  HtmlElement * currentNode = m_element->parent();
  while (not currentNode->isa("html"))
  {
    if (currentNode->isa("form"))
      break;
    currentNode = currentNode->parent();
  }
  if (currentNode->isa("html"))
  {
    return;
  }
  // select the "successful controls" and post them
  //m_processedData = unicode2string(currentNode->attribute("action"));
  //m_processedData += '?';
  uri.setMethod(unicode2string(currentNode->attribute("method")));

  ElementList inputs(currentNode->elementsByTagName("input"));
  ElementList::const_iterator inputIt(inputs.begin());
  bool needAmp(false);
  bool isGet = uri.method() == "GET";
  /*
  // use md5 when forms support larger fields...
  std::vector<std::string> postData;
  md5_context ctx;
  if (not isGet)
  {
    md5_starts(&ctx);
  }
  */
  std::string processedData;
  for (; inputIt != inputs.end(); ++inputIt)
  {
    const HtmlElement * element(*inputIt);
    UnicodeString name = element->attribute("name");
    std::string type = unicode2string(element->attribute("type"));
    if (not name.empty() and (type.empty() or type == "text" or type == "password")
        or (m_element == element) )
    {
      UnicodeString value = element->attribute("value");
      if (not value.empty() or (m_element == element))
      {
        //if (isGet) {
          if (needAmp) {
            processedData += '&';
          }
          processedData += unicode2string(URI::escape(name));
          processedData += "=";
          processedData += unicode2string(URI::escape(value));
          needAmp = true;
        /*}
        else
        {
          // post adds the data to the content type..
          postData.push_back(unicode2string(URI::escape(name)));
          md5_update(&ctx, (unsigned char*)postData.back().c_str(), postData.back().length());
          postData.push_back(unicode2string(URI::escape(value)));
          md5_update(&ctx, (unsigned char*)postData.back().c_str(), postData.back().length());
        }
        */
      }
    }
  }
  if (isGet) {
    m_processedData = unicode2string(currentNode->attribute("action"));
    m_processedData += '?';
    m_processedData += processedData;
    uri = uri.navigateTo(m_processedData);
  }
  else {
    std::string contentType = "Content-Type: application/x-www-form-urlencoded\r\n";
    char buffer[256];
    sprintf(buffer, "%d", processedData.length());
    contentType += "Content-Length: ";
    contentType += buffer;
    contentType += "\r\n";
    contentType += "\r\n";
    // add url

    contentType += processedData;
    uri.setRequestHeader(contentType);

    /*
    unsigned char output[16];
    md5_finish(&ctx, output);
    std::vector<std::string>::const_iterator it(postData.begin());
    std::string processedData;
    processedData += "Content-Type: multipart/form-data; boundary=";
    addBoundary(processedData, output);
    processedData += "\r\n";
    processedData += "--";
    addBoundary(processedData, output);
    for ( ; it != postData.end(); ++it)
    {
      processedData += "Content-Disposition: form-data; name=\"";
      processedData += *it;
      processedData += "\"\r\n";
      //processedData += "Content-Type: text/plain\r\n";
      processedData += "\r\n";
      // add the value
      ++it;
      processedData += *it;
      processedData += "\r\n";
      processedData += "--";
      addBoundary(processedData, output);
    }
    uri.setRequestHeader(processedData);
    */
  }
  controller.doUri(uri);
}
