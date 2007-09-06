/*
  Copyright 2007 Richard Quirk
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "FormControl.h"
#include "HtmlElement.h"
#include "HtmlConstants.h"
#include "HtmlInputElement.h"
#include "ControllerI.h"
#include "URI.h"

const int FormControl::MAX_SIZE(120);
const int FormControl::MIN_SIZE(8);

FormControl::FormControl(const HtmlElement * element):
  Button(element->attribute("value")),
  m_element(element)
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

void FormControl::input(ControllerI & controller, URI & uri)
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

  ElementList inputs(currentNode->elementsByTagName(HtmlConstants::INPUT_TAG));
  ElementList selects(currentNode->elementsByTagName(HtmlConstants::SELECT_TAG));
  ElementList::iterator inputEnd(inputs.end());
  inputs.splice(inputEnd, selects);
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
  UnicodeString myName = m_element->attribute("name");
  const UnicodeString one(string2unicode("1"));
  for (; inputIt != inputs.end(); ++inputIt)
  {
    const HtmlElement * element(*inputIt);
    UnicodeString name = element->attribute("name");
    UnicodeString value = element->attribute("value");
    bool includeValue(false);
    if (element->isa(HtmlConstants::INPUT_TAG))
    {
      HtmlInputElement::InputType inputType = ((const HtmlInputElement*)element)->inputType();
      bool checked = ((const HtmlInputElement*)element)->checked();
      switch (inputType)
      {
        case HtmlInputElement::TEXT:
        case HtmlInputElement::PASSWORD:
          includeValue = not name.empty();
          break;

        case HtmlInputElement::SUBMIT:
          includeValue = not name.empty() and not value.empty();
          break;

        case HtmlInputElement::CHECKBOX:
          includeValue = checked;
          if (value.empty())
          {
            value = string2unicode("on");
          }
          break;

        case HtmlInputElement::RADIO:
          // only include the selected radio button
          includeValue = checked;
          break;

        case HtmlInputElement::HIDDEN:
          includeValue = true;
          break;

      }
    }
    else if (element->isa(HtmlConstants::SELECT_TAG))
    {
      includeValue = not value.empty();
    }
    if (m_element == element and not myName.empty())
    {
      includeValue = true;
    }
    if (includeValue)
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
  if (isGet) {
    m_processedData = unicode2string(currentNode->attribute("action"));
    m_processedData += '?';
    m_processedData += processedData;
    uri = uri.navigateTo(m_processedData);
  }
  else {
    std::string contentType = "Content-Type: application/x-www-form-urlencoded\r\n";
    char buffer[256];
#ifdef ARM9
    siprintf(buffer, "%d", processedData.length());
#else
    sprintf(buffer, "%d", processedData.length());
#endif
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
  printf("Go to %s\n", uri.asString().c_str());
  controller.doUri(uri);
}
