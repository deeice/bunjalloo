/*
  Copyright (C) 2007,2008 Richard Quirk

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
#include "HtmlAnchorElement.h"
#include "HtmlBlockElement.h"
#include "HtmlBodyElement.h"
#include "HtmlConstants.h"
#include "HtmlDocument.h"
#include "HtmlElement.h"
#include "HtmlFormElement.h"
#include "HtmlImageElement.h"
#include "HtmlInputElement.h"
#include "HtmlMetaElement.h"
#include "HtmlOptionElement.h"
#include "HtmlPreElement.h"
#include "HtmlTextAreaElement.h"
#include "InputText.h"
#include "Image.h"
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
    bool parseNewline(m_self->m_document.htmlDocument()->mimeType() == HtmlDocument::TEXT_PLAIN);
    m_textArea->setParseNewline(parseNewline);
    m_self->m_scrollPane->add(m_textArea);
  }
  return m_textArea;
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

void ViewRender::doImage(const UnicodeString & imgStr,
    const UnicodeString & src)
{
  textArea()->addImage(unicode2string(src));
  textArea()->appendText(imgStr);
  textArea()->endImage();
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

void ViewRender::render()
{
  m_self->m_scrollPane->removeChildren();

  clearRadioGroups();

  m_textArea = 0;
  const HtmlElement * root = m_self->m_document.rootNode();
  HtmlDocument::MimeType mimeType = m_self->m_document.htmlDocument()->mimeType();
  bool useScrollPane(false);
  if (mimeType == HtmlDocument::IMAGE_PNG
      or mimeType == HtmlDocument::IMAGE_GIF
      or mimeType == HtmlDocument::IMAGE_JPEG)
  {
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
    Image * image(0);
    if (not filename.empty())
    {
      image = new Image(filename.c_str(), (Image::ImageType)mimeType);
    }
    ImageComponent * imageComponent = new ImageComponent(image);
    textArea()->add(imageComponent);
    useScrollPane = true;

  }
  else if (mimeType == HtmlParser::OTHER)
  {
    string notViewable("Content not viewable");
    textArea()->appendText(string2unicode(notViewable));
    useScrollPane = true;
  }
  else
  {
    assert(root->isa(HtmlConstants::HTML_TAG));
    assert(root->hasChildren());
    doTitle(m_self->m_document.titleNode());

    HtmlElement * body = (HtmlElement*)root->lastChild();
    if (body->hasChildren())
    {
      body->accept(*this);
      useScrollPane = true;
    }
  }

  if (useScrollPane)
  {
    ScrollPane & scrollPane(*m_self->m_scrollPane);
    scrollPane.setLocation(0,0);
    scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
    scrollPane.setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
    scrollPane.scrollToPercent(0);
  }
}

void ViewRender::doTitle(const HtmlElement * title)
{
  if (title)
  {
    m_textArea = (RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH);
    m_textArea->setCentred();
    m_textArea->setOutlined();
    m_textArea->setSize(nds::Canvas::instance().width()-7, m_textArea->font().height());
    m_self->m_scrollPane->add(m_textArea);
    HtmlElement * titleText = title->firstChild();
    visit(*titleText);
    m_textArea = 0;
  }
}

void ViewRender::renderSelect(const HtmlElement * selectElement)
{
  Select * select = new Select(const_cast<HtmlElement*>(selectElement));
  textArea()->add(select);
}


// FIXME - where should this go?
static const int MAX_SIZE(SCREEN_WIDTH-7);
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

// Visitor implementation
void ViewRender::begin(HtmlAnchorElement & element)
{
  const UnicodeString & href(element.attribute("href"));
  bool viewed = false;
  if (not href.empty())
  {
    URI newUri = URI(m_self->m_document.uri()).navigateTo(unicode2string(href));
    viewed = m_self->m_controller.cache()->contains(newUri, false);
  }
  textArea()->addLink( unicode2string(href), viewed);
}

bool ViewRender::visit(HtmlAnchorElement & element)
{
  return true;
}
void ViewRender::end(HtmlAnchorElement & element)
{
  textArea()->endLink();
}

void ViewRender::begin(HtmlBlockElement & element)
{
  if (element.isBlock())
  {
    textArea()->insertNewline();
  }
}
bool ViewRender::visit(HtmlBlockElement & element)
{
  return true;
}
void ViewRender::end(HtmlBlockElement & element)
{
  if (element.isBlock())
  {
    textArea()->insertNewline();
  }
}

void ViewRender::begin(HtmlBodyElement & element)
{
}
bool ViewRender::visit(HtmlBodyElement & element)
{
  return true;
}
void ViewRender::end(HtmlBodyElement & element)
{
}

void ViewRender::begin(HtmlElement & element)
{
  if (element.isa(HtmlConstants::UL_TAG) or element.isa(HtmlConstants::OL_TAG))
  {
    // FIXME!!
    /** m_self->m_textArea->increaseIndent(); */
    if (not element.isBlock() and element.parent()->isa(HtmlConstants::LI_TAG))
      textArea()->insertNewline();
  }
  else if (element.isa(HtmlConstants::LI_TAG)) {
    const HtmlElement * prev(element.parent()->previousSibling(&element));
    if (prev and prev->isa("#TEXT")) // TODO - remove #TEXT and so forth
    {
      textArea()->insertNewline();
    }
  }
}

bool ViewRender::visit(HtmlElement & element)
{
  if (not element.text().empty())
  {
    textArea()->appendText(element.text());
  }
  else if (element.isa(HtmlConstants::BR_TAG))
  {
    textArea()->insertNewline();
  }
  else if (element.isa(HtmlConstants::TEXTAREA_TAG))
  {
    renderTextArea(&element);
  }
  /*else if (element.isa(HtmlConstants::TITLE_TAG))
  {
    if (element.hasChildren())
    {
      visit(*element.firstChild());
    }
  }*/
  return true;
}
void ViewRender::end(HtmlElement & element)
{
  if (element.isa(HtmlConstants::LI_TAG))
  {
    ElementList::const_iterator it(element.children().begin());
    ElementList::const_iterator end(element.children().end());
    bool hasBlock(false);
    for (; it != end; ++it)
    {
      if ( (*it)->isa(HtmlConstants::UL_TAG) or (*it)->isBlock()) {
        hasBlock = true;
        break;
      }
    }
    if (not hasBlock)
      textArea()->insertNewline();
  }
  else if (element.isa(HtmlConstants::P_TAG) or (element.tagName()[0] == 'h' and (element.tagName()[1] >= '1' and element.tagName()[1] <= '6')))
  {
    textArea()->insertNewline();
  }
}

void ViewRender::begin(HtmlFormElement & element)
{
  textArea()->insertNewline();
}
bool ViewRender::visit(HtmlFormElement & element)
{
  return true;
}
void ViewRender::end(HtmlFormElement & element)
{
}

void ViewRender::begin(HtmlImageElement & element)
{
}
bool ViewRender::visit(HtmlImageElement & element)
{
  // hurrah for alt text. some people set it to "", which screws up any
  // easy way to display it (see w3m google.com - Google [hp1] [hp2] [hp3]... huh?)
  bool hasAltText = element.hasAltText();
  UnicodeString imgText = extractImageText(&element, hasAltText);
  if (not imgText.empty())
  {
    doImage(imgText, element.attribute("src"));
  }
  return true;
}
void ViewRender::end(HtmlImageElement & element)
{
}

void ViewRender::begin(HtmlInputElement & element)
{
}
bool ViewRender::visit(HtmlInputElement & element)
{
  if (element.isa(HtmlConstants::SELECT_TAG))
  {
    renderSelect(&element);
    return false;
  }
  else if (element.isa(HtmlConstants::INPUT_TAG))
  {
    renderInput(&element);
  }
  return true;
}
void ViewRender::end(HtmlInputElement & element)
{
}

void ViewRender::begin(HtmlMetaElement & element)
{
}
bool ViewRender::visit(HtmlMetaElement & element)
{
  return true;
}
void ViewRender::end(HtmlMetaElement & element)
{
}

void ViewRender::begin(HtmlOptionElement & element)
{
}
bool ViewRender::visit(HtmlOptionElement & element)
{
  return true;
}
void ViewRender::end(HtmlOptionElement & element)
{
}

void ViewRender::begin(HtmlPreElement & element)
{
  textArea()->setParseNewline(true);
}
bool ViewRender::visit(HtmlPreElement & element)
{
  return true;
}
void ViewRender::end(HtmlPreElement & element)
{
  textArea()->setParseNewline(false);
}

void ViewRender::begin(HtmlTextAreaElement & element)
{
}
bool ViewRender::visit(HtmlTextAreaElement & element)
{
  renderTextArea(&element);
  return true;
}
void ViewRender::end(HtmlTextAreaElement & element)
{
}
