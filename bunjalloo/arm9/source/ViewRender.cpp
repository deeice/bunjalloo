//#include <iostream>
#include <assert.h>
#include "ndspp.h"
#include "libnds.h"
#include "Canvas.h"
#include "Document.h"
#include "ViewRender.h"
#include "View.h"
#include "TextArea.h"
#include "HtmlElement.h"
#include "HtmlImageElement.h"
#include "HtmlBodyElement.h"
#include "File.h"
#include "Select.h"
#include "Button.h"
#include "InputText.h"
#include "PasswordField.h"

using namespace std;

ViewRender::ViewRender(View * self):
  m_self(self),
  m_lastElement(0)
{
}

void ViewRender::preFormat(const HtmlElement * element)
{
  if (element->isa("a"))
  {
    m_self->m_textArea->setTextColor(nds::Color(0,0,31));
    m_self->m_textArea->addLink(element);
  }
  else if (element->isa("pre"))
  {
    m_self->m_textArea->setParseNewline(true);
  }
  else if (element->isa("ul") or element->isa("ol"))
  {
    m_self->m_textArea->increaseIndent();
    if (not element->isBlock() and element->parent()->isa("li"))
      m_self->m_textArea->insertNewline();
  }
  else if (element->isa("li")) {
    const HtmlElement * prev(element->parent()->previousSibling(element));
    if (prev and prev->isa("#TEXT"))
    {
      m_self->m_textArea->insertNewline();
    }
  }

  if (element->isBlock())
  {
    m_self->m_textArea->insertNewline();
  }
}

void ViewRender::postFormat(const HtmlElement * element)
{
  if (element->isa("pre"))
  {
    m_self->m_textArea->setParseNewline(false);
  }
  else if (element->isa("ul") or element->isa("ol"))
  {
    m_self->m_textArea->decreaseIndent();
    // only add an extra \n if the ul is a top level one
    const HtmlElement * next = element->parent()->nextSibling(element);
    if ( next 
        and not next->isa("li") 
        and not element->isBlock() )
        //and not (element->parent()->isa("li") or element->parent()->isa("ul") or element->parent()->isa("ol")))
    {
      m_self->m_textArea->insertNewline();
    }
  }
  else if (element->isa("li"))
  {
    ElementList::const_iterator it(element->children().begin());
    bool hasBlock(false);
    for (; it != element->children().end(); ++it)
    {
      if ( (*it)->isa("ul") or (*it)->isBlock()) {
        hasBlock = true;
        break;
      }
    }
    if (not hasBlock)
      m_self->m_textArea->insertNewline();
  }
  else if (element->isa("p") or (element->tagName()[0] == 'h' and (element->tagName()[1] >= '1' and element->tagName()[1] <= '6')))
  {
    m_self->m_textArea->insertNewline();
  } 
  else if (element->isa("a"))
  {
    m_self->m_textArea->setTextColor(nds::Color(0,0,0));
    m_self->m_textArea->setLink(false);
  }


  /*
  if (element->isBlock())
  {
    m_self->m_textArea->insertNewline();
  }
  */
}

bool ViewRender::applyFormat(const HtmlElement * element)
{
  if (not element->text().empty())
  {
    m_self->m_textArea->printu(element->text());
  }
  else if (element->isa("script") or element->isa("style"))
  {
    return false;
  }
  else if (element->isa("br"))
  {
    m_self->m_textArea->insertNewline();
  } 
  else if (element->isa("img"))
  {
    // hurrah for alt text. some people set it to "", which screws up any
    // easy way to display it (see w3m google.com - Google [hp1] [hp2] [hp3]... huh?)
    const UnicodeString & altText = element->attribute("alt");
    bool hasAltText = ((HtmlImageElement*)element)->hasAltText();
    if (hasAltText) {
      if (not altText.empty()) {
        doImage(altText);
      }
      return false;
    }
    const UnicodeString & srcText = element->attribute("src");
    if (not srcText.empty())
    {
      string tmp = unicode2string(srcText);
      const char * bname = nds::File::base(tmp.c_str());
      string bnamestr(bname);
      bnamestr = "["+ bnamestr+"]";
      UnicodeString ustr (string2unicode(bnamestr));
      doImage(ustr);
      return false;
    }
    UnicodeString ustr( string2unicode("[IMG]"));
    doImage(ustr);
    return false;
  }
  else if (element->isa("select"))
  {
    renderSelect(element);
    return false; // do not walk the children
  }
  else if (element->isa("input"))
  {
    renderInput(element);
    return false; 
  }
  return true;
}

void ViewRender::walkTree(const HtmlElement * element)
{
  preFormat(element);
  if (element->hasChildren())
  {
    const ElementList & theChildren = element->children();
    ElementList::const_iterator it(theChildren.begin());
    bool hasBlock(false);
    for (; it != theChildren.end(); ++it)
    {
      const HtmlElement * child(*it);
      if (child->isBlock()) {
        hasBlock = true;
        break;
      }
    }

    it = theChildren.begin();
    for (; it != theChildren.end(); ++it)
    {
      const HtmlElement * child(*it);
      if (hasBlock) {
        ((HtmlElement*)child)->setBlock();
      }
      if (applyFormat(child)) {
        walkTree(child);
      }
    }
  }
  postFormat(element);
  m_lastElement = element;
}

void ViewRender::setBgColor(const HtmlElement * body)
{
  UnicodeString bgcolor = body->attribute("bgcolor");
  if (not bgcolor.empty())
  {
    unsigned int rgb8 = ((HtmlBodyElement*)body)->bgColor();
    nds::Color col( ((rgb8 >> 16)&0xff)/8, ((rgb8 >> 8)&0xff)/8, (rgb8&0xff)/8);
    m_self->m_textArea->setBackgroundColor(col);
  }
}

void ViewRender::doImage(const UnicodeString & imgStr)
{
  m_self->m_textArea->setTextColor(nds::Color(0,31,0));
  m_self->m_textArea->printu(imgStr);
  m_self->m_textArea->setTextColor(nds::Color(0,0,0));
}

void ViewRender::render()
{
  m_self->m_textArea->setCursor(0, 0);
  m_self->m_textArea->setParseNewline(false);
  const HtmlElement * root = m_self->m_document.rootNode();
  assert(root->isa("html"));
  assert(root->hasChildren());
  const HtmlElement * body = root->lastChild();
  setBgColor(body);
  m_self->m_textArea->clear();
  if (body->hasChildren())
  {
    walkTree(body);
  }
}

void ViewRender::renderSelect(const HtmlElement * selectElement)
{
  // render the select
  Select * formSelect = new Select( const_cast<HtmlElement*>(selectElement));
  if (selectElement->hasChildren())
  {
    const ElementList & theChildren = selectElement->children();
    ElementList::const_iterator it(theChildren.begin());
    for (; it != theChildren.end(); ++it)
    {
      if ( (*it)->isa("option") ) {
        formSelect->addOption((*it), m_self->m_textArea);
      }
    }
  }
  m_self->m_textArea->addFormControl(formSelect);
}

void ViewRender::renderInput(const HtmlElement * inputElement)
{
  //Input * formInput = new Input;
  string type = unicode2string(inputElement->attribute("type"));
  if (type == "submit")
  {
    Button * submitButton = new Button(const_cast<HtmlElement*>(inputElement), m_self->m_textArea);
    m_self->m_textArea->addFormControl(submitButton);
  }
  else if (type.empty() or type == "text")
  {
    InputText * text = new InputText(const_cast<HtmlElement*>(inputElement), m_self->m_textArea);
    m_self->m_textArea->addFormControl(text);
  } 
  else if (type == "password")
  {
    PasswordField * text = new PasswordField(const_cast<HtmlElement*>(inputElement), m_self->m_textArea);
    m_self->m_textArea->addFormControl(text);
  } 
}
