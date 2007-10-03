/*
  Copyright (C) 2007 Richard Quirk

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <assert.h>
#include "ndspp.h"
#include "libnds.h"
#include "Canvas.h"
#include "Document.h"
#include "File.h"
#include "FormCheckBox.h"
#include "FormControl.h"
#include "FormRadio.h"
#include "FormTextArea.h"
#include "HtmlBodyElement.h"
#include "HtmlConstants.h"
#include "HtmlDocument.h"
#include "HtmlElement.h"
#include "HtmlImageElement.h"
#include "HtmlInputElement.h"
#include "InputText.h"
#include "ImageComponent.h"
#include "Keyboard.h"
#include "PasswordField.h"
#include "RadioButton.h"
#include "RichTextArea.h"
#include "ScrollPane.h"
#include "Select.h"
#include "TextAreaFactory.h"
#include "URI.h"
#include "View.h"
#include "ViewRender.h"
#include "Controller.h"
#include "Cache.h"

using namespace std;

ViewRender::ViewRender(View * self):
  m_self(self),
  m_textArea(0),
  m_lastElement(0)
{
}

RichTextArea * ViewRender::textArea()
{
  if (m_textArea == 0)
  {
    m_textArea = (RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH);
    m_textArea->setSize(nds::Canvas::instance().width()-7, m_textArea->font().height());
    m_textArea->addLinkListener(m_self);
    m_textArea->setParseNewline(false);
    m_self->m_scrollPane->add(m_textArea);
  }
  return m_textArea;
}

void ViewRender::preFormat(const HtmlElement * element)
{
  // FIXME - BWT changes - this needs rewriting!
  if (element->isa(HtmlConstants::A_TAG))
  {
    textArea()->addLink( unicode2string(element->attribute("href")) );
  }
  if (element->isa(HtmlConstants::FORM_TAG))
  {
    textArea()->insertNewline();
  }
  else if (element->isa(HtmlConstants::PRE_TAG))
  {
    textArea()->setParseNewline(true);
  }
  else if (element->isa(HtmlConstants::UL_TAG) or element->isa(HtmlConstants::OL_TAG))
  {
    // FIXME!!
    /** m_self->m_textArea->increaseIndent(); */
    if (not element->isBlock() and element->parent()->isa(HtmlConstants::LI_TAG))
      textArea()->insertNewline();
  }
  else if (element->isa(HtmlConstants::LI_TAG)) {
    const HtmlElement * prev(element->parent()->previousSibling(element));
    if (prev and prev->isa("#TEXT")) // TODO - remove #TEXT and so forth
    {
      textArea()->insertNewline();
    }
  }

  if (element->isBlock())
  {
    textArea()->insertNewline();
  }
}

void ViewRender::postFormat(const HtmlElement * element)
{
  if (element->isa(HtmlConstants::PRE_TAG))
  {
    textArea()->setParseNewline(false);
  }
  else if (element->isa(HtmlConstants::UL_TAG) or element->isa(HtmlConstants::OL_TAG))
  {
    // FIXME - BWT changes - this needs rewriting!
#if 0
    textArea()->decreaseIndent();
    // only add an extra \n if the ul is a top level one
    const HtmlElement * next = element->parent()->nextSibling(element);
    if ( next 
        and not next->isa(HtmlConstants::LI_TAG) 
        and not element->isBlock() )
        //and not (element->parent()->isa(HtmlConstants::LI_TAG) or element->parent()->isa(HtmlConstants::UL_TAG) or element->parent()->isa(HtmlConstants::OL_TAG)))
    {
      m_self->m_textArea->insertNewline();
    }
#endif
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
      textArea()->insertNewline();
  }
  else if (element->isa(HtmlConstants::P_TAG) or (element->tagName()[0] == 'h' and (element->tagName()[1] >= '1' and element->tagName()[1] <= '6')))
  {
    textArea()->insertNewline();
  } 
  else if (element->isa(HtmlConstants::A_TAG))
  {
    textArea()->endLink();
  }

  if (element->isBlock())
  {
    textArea()->insertNewline();
  }
}

static UnicodeString extractImageText(const HtmlElement * element, bool hasAltText)
{
  const UnicodeString & altText = element->attribute("alt");
  // NO! It might not be a HtmlImageElement - could be a HtmlInputElement
  if (hasAltText) {
    return altText;
  }
  const UnicodeString & srcText = element->attribute("src");
  if (not srcText.empty())
  {
    string tmp = unicode2string(srcText);
    const char * bname = nds::File::base(tmp.c_str());
    string bnamestr(bname);
    bnamestr = "["+ bnamestr+"]";
    return string2unicode(bnamestr);
  }
  return string2unicode("[IMG]");
}

bool ViewRender::applyFormat(const HtmlElement * element)
{
  if (not element->text().empty())
  {
    textArea()->appendText(element->text());
    // and clear the text.
    element->clearText();
  }
  else if (element->isa(HtmlConstants::SCRIPT_TAG) or element->isa(HtmlConstants::STYLE_TAG))
  {
    return false;
  }
  else if (element->isa(HtmlConstants::BR_TAG))
  {
    textArea()->insertNewline();
  } 
  else if (element->isa(HtmlConstants::IMG_TAG))
  {
    // hurrah for alt text. some people set it to "", which screws up any
    // easy way to display it (see w3m google.com - Google [hp1] [hp2] [hp3]... huh?)
    bool hasAltText = ((HtmlImageElement*)element)->hasAltText();
    UnicodeString imgText = extractImageText(element, hasAltText);
    if (not imgText.empty())
    {
      doImage(imgText);
    }
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
  else if (element->isa(HtmlConstants::TEXTAREA_TAG))
  {
    renderTextArea(element);
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
    /*
    for (; it != theChildren.end(); ++it)
    {
      const HtmlElement * child(*it);
      if (child->isBlock()) {
        hasBlock = true;
        break;
      }
    }
    */

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
    - semi fixed, but not perfect
    */
    m_self->m_scrollPane->setBackgroundColor(col);
  }
}

void ViewRender::doImage(const UnicodeString & imgStr)
{
  textArea()->setColor(nds::Color(0,21,0));
  textArea()->appendText(imgStr);
  textArea()->endColor();
}

void ViewRender::clearRadioGroups()
{
  FormGroupMap::iterator it(m_radioGroup.begin());
  for (; it != m_radioGroup.end(); ++it)
  {
    delete it->second;
  }
  m_radioGroup.clear();
}

/*
ElementList ViewRender::selectedRadioButtons() const
{
  ElementList theList;
  FormGroupMap::const_iterator it(m_radioGroup.begin());
  for (; it != m_radioGroup.end(); ++it)
  {
     theList.push_back(it->second->selectedElement());
  }
  return theList;
}
*/

void ViewRender::render()
{
  m_self->m_scrollPane->removeChildren();

  clearRadioGroups();

  m_textArea = 0;
  const HtmlElement * root = m_self->m_document.rootNode();
  if (m_self->m_document.htmlDocument()->mimeType() == HtmlDocument::IMAGE_PNG
      or m_self->m_document.htmlDocument()->mimeType() == HtmlDocument::IMAGE_GIF)
  {
    //textArea()->add();
    
    URI uri(m_self->m_document.uri());
    string filename;
    if (uri.protocol() == URI::FILE_PROTOCOL)
    {
      filename = uri.fileName();
    }
    else
    {
      filename = m_self->m_controller.cache()->fileName(m_self->m_document.uri());
    }
    ImageComponent * imageComponent = new ImageComponent(filename);
    textArea()->add(imageComponent);
      ScrollPane & scrollPane(*m_self->m_scrollPane);
      scrollPane.setLocation(0,0);
      scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
      scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
      scrollPane.scrollToPercent(0);
  }
  else
  {
    assert(root->isa(HtmlConstants::HTML_TAG));
    assert(root->hasChildren());
    const HtmlElement * body = root->lastChild();
    if (body->hasChildren())
    {
      walkTree(body);
      ScrollPane & scrollPane(*m_self->m_scrollPane);
      scrollPane.setLocation(0,0);
      scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
      scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
      scrollPane.scrollToPercent(0);
    }
  }
}

void ViewRender::renderSelect(const HtmlElement * selectElement)
{
  Select * select = new Select(const_cast<HtmlElement*>(selectElement));
  /*
  m_textArea = 0;
  m_self->m_scrollPane->add(select);
  */
  textArea()->add(select);
}


// FIXME - where should this go?
static const int MAX_SIZE(nds::Canvas::instance().width()-7);
static const int MIN_SIZE(8);


void ViewRender::renderInput(const HtmlElement * inputElement)
{
  string sizeText = unicode2string(inputElement->attribute("size"));
  int size(0);
  if (not sizeText.empty())
  {
    size = strtol(sizeText.c_str(), 0, 0);
    size *= 8; // FIXME! should be: textArea->font().height(); ?
  }
  if (size > MAX_SIZE)
    size = MAX_SIZE;

  HtmlInputElement::InputType type = ((const HtmlInputElement*)inputElement)->inputType();
  switch (type)
  {
    case HtmlInputElement::IMAGE:
    case HtmlInputElement::SUBMIT:
      {
        bool hasAltText = ((HtmlInputElement*)inputElement)->hasAltText();
        FormControl * submitButton = new FormControl(inputElement, 
            type == HtmlInputElement::SUBMIT?(
            inputElement->attribute("value").empty()?
            string2unicode("Submit"):
            inputElement->attribute("value")):
            extractImageText(inputElement, hasAltText)
            );
        submitButton->setListener(m_self);
        // m_textArea = 0;
        if (size > MIN_SIZE)
        {
          submitButton->setSize(size, submitButton->preferredSize().h);
        }
        /* m_self->m_scrollPane->add(submitButton); */
        textArea()->add(submitButton);
      }
      break;
    case HtmlInputElement::TEXT:
      {
        InputText * text = new InputText(const_cast<HtmlElement*>(inputElement));
        // m_textArea = 0;
        if (size <= 0)
          size = MIN_SIZE*8; // FIXME! textArea->font().height();
        text->setListener(m_self->m_keyboard);
        text->setSize(size, text->preferredSize().h);
        /*m_self->m_scrollPane->add(text);*/
        textArea()->add(text);
      } 
      break;
    case HtmlInputElement::PASSWORD:
      {
        // TODO - password field should show *** instead of real text.
        PasswordField * text = new PasswordField(const_cast<HtmlElement*>(inputElement));
        // m_textArea = 0;
        if (size <= 0)
          size = MIN_SIZE*8; // FIXME! textArea->font().height();
        text->setListener(m_self->m_keyboard);
        text->setSize(size, text->preferredSize().h);
        // m_self->m_scrollPane->add(text);
        textArea()->add(text);
      }
      break;
    case HtmlInputElement::CHECKBOX:
      {
        // m_textArea = 0;
        FormCheckBox * checkbox = new FormCheckBox(const_cast<HtmlElement*>(inputElement));
        textArea()->add(checkbox);
      }
      break;
    case HtmlInputElement::RADIO:
      {
        // see if there is a RadioGroup with this name
        UnicodeString name = inputElement->attribute("name");
        // FIXME - get the group.
        // m_textArea = 0;
        RadioButton * radio = new RadioButton;
        textArea()->add(radio);
        if (not name.empty())
        {
          FormGroupMap::iterator it(m_radioGroup.find(name));
          if (it != m_radioGroup.end())
          {
            it->second->add(radio, const_cast<HtmlElement*>(inputElement));
          }
          else
          {
            FormRadio * fr = new FormRadio;
            fr->add(radio, const_cast<HtmlElement*>(inputElement));
            m_radioGroup[name] = fr;
          }
        }
      }
      break;
    case HtmlInputElement::HIDDEN:
      break;
  }
}

void ViewRender::renderTextArea(const HtmlElement * textAreaElement)
{
  FormTextArea * text = new FormTextArea(const_cast<HtmlElement*>(textAreaElement));
  text->setListener(m_self->m_keyboard);
  m_textArea = 0;
  m_self->m_scrollPane->add(text);
}

