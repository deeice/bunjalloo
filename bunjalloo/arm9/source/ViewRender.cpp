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
#include "HtmlConstants.h"

using namespace std;

ViewRender::ViewRender(View * self):
  m_self(self),
  m_lastElement(0)
{
}

void ViewRender::preFormat(const HtmlElement * element)
{
  // FIXME - BWT changes - this needs rewriting!
#if 0
  if (element->isa(HtmlConstants::A_TAG))
  {
    m_self->m_textArea->setTextColor(nds::Color(0,0,31));
    m_self->m_textArea->addLink( unicode2string(element->attribute("href")) );
  }
  else if (element->isa(HtmlConstants::PRE_TAG))
  {
    m_self->m_textArea->setParseNewline(true);
  }
  else if (element->isa(HtmlConstants::UL_TAG) or element->isa(HtmlConstants::OL_TAG))
  {
    m_self->m_textArea->increaseIndent();
    if (not element->isBlock() and element->parent()->isa(HtmlConstants::LI_TAG))
      m_self->m_textArea->insertNewline();
  }
  else if (element->isa(HtmlConstants::LI_TAG)) {
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
#endif
}

void ViewRender::postFormat(const HtmlElement * element)
{
  // FIXME - BWT changes - this needs rewriting!
#if 0
  if (element->isa(HtmlConstants::PRE_TAG))
  {
    m_self->m_textArea->setParseNewline(false);
  }
  else if (element->isa(HtmlConstants::UL_TAG) or element->isa(HtmlConstants::OL_TAG))
  {
    m_self->m_textArea->decreaseIndent();
    // only add an extra \n if the ul is a top level one
    const HtmlElement * next = element->parent()->nextSibling(element);
    if ( next 
        and not next->isa(HtmlConstants::LI_TAG) 
        and not element->isBlock() )
        //and not (element->parent()->isa(HtmlConstants::LI_TAG) or element->parent()->isa(HtmlConstants::UL_TAG) or element->parent()->isa(HtmlConstants::OL_TAG)))
    {
      m_self->m_textArea->insertNewline();
    }
  }
  else if (element->isa(HtmlConstants::LI_TAG))
  {
    ElementList::const_iterator it(element->children().begin());
    bool hasBlock(false);
    for (; it != element->children().end(); ++it)
    {
      if ( (*it)->isa(HtmlConstants::UL_TAG) or (*it)->isBlock()) {
        hasBlock = true;
        break;
      }
    }
    if (not hasBlock)
      m_self->m_textArea->insertNewline();
  }
  else if (element->isa(HtmlConstants::P_TAG) or (element->tagName()[0] == 'h' and (element->tagName()[1] >= '1' and element->tagName()[1] <= '6')))
  {
    m_self->m_textArea->insertNewline();
  } 
  else if (element->isa(HtmlConstants::A_TAG))
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
#endif
}

bool ViewRender::applyFormat(const HtmlElement * element)
{
  // FIXME - BWT changes - this needs rewriting!
#if 0
  if (not element->text().empty())
  {
    m_self->m_textArea->printu(element->text());
  }
  else if (element->isa(HtmlConstants::SCRIPT_TAG) or element->isa(HtmlConstants::STYLE_TAG))
  {
    return false;
  }
  else if (element->isa(HtmlConstants::BR_TAG))
  {
    m_self->m_textArea->insertNewline();
  } 
  else if (element->isa(HtmlConstants::IMG_TAG))
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
  else if (element->isa(HtmlConstants::SELECT_TAG))
  {
    renderSelect(element);
    return false; // do not walk the children
  }
  else if (element->isa(HtmlConstants::INPUT_TAG))
  {
    renderInput(element);
    return false; 
  }
#endif
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
    /** FIXME - BWT changes
    m_self->m_textArea->setBackgroundColor(col);
    */
  }
}

void ViewRender::doImage(const UnicodeString & imgStr)
{
  /** FIXME - BWT changes
  m_self->m_textArea->setTextColor(nds::Color(0,31,0));
  m_self->m_textArea->printu(imgStr);
  m_self->m_textArea->setTextColor(nds::Color(0,0,0));
  */
}

void ViewRender::render()
{
  /** FIXME - BWT changes
  m_self->m_textArea->setCursor(0, 0);
  m_self->m_textArea->setParseNewline(false);
  */
  const HtmlElement * root = m_self->m_document.rootNode();
  assert(root->isa(HtmlConstants::HTML_TAG));
  assert(root->hasChildren());
  const HtmlElement * body = root->lastChild();
  setBgColor(body);
  /** FIXME - BWT changes
  m_self->m_textArea->clear();
  */
  if (body->hasChildren())
  {
    walkTree(body);
  }
}

void ViewRender::renderSelect(const HtmlElement * selectElement)
{
  /** FIXME - BWT changes
  // render the select
  Select * formSelect = new Select( const_cast<HtmlElement*>(selectElement));
  if (selectElement->hasChildren())
  {
    const ElementList & theChildren = selectElement->children();
    ElementList::const_iterator it(theChildren.begin());
    for (; it != theChildren.end(); ++it)
    {
      if ( (*it)->isa(HtmlConstants::OPTION_TAG) ) {
        formSelect->addOption((*it), m_self->m_textArea);
      }
    }
  }
  m_self->m_textArea->addFormControl(formSelect);
  */
}

void ViewRender::renderInput(const HtmlElement * inputElement)
{
  /** FIXME - BWT changes */
#if 0
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
#endif
}
