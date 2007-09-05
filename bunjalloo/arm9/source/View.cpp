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
#include <libgen.h>
//#include <iostream>
#include "ndspp.h"
#include "libnds.h"
#include "Canvas.h"
#include "Config.h"
#include "ControllerI.h"
#include "Document.h"
#include "FormControl.h"
#include "Keyboard.h"
#include "Link.h"
#include "ScrollPane.h"
#include "URI.h"
#include "View.h"
#include "ViewRender.h"

using namespace std;
const static int STEP(1);

View::View(Document & doc, ControllerI & c):
  m_document(doc), 
  m_controller(c),
  m_scrollPane(new ScrollPane),
  m_keyboard(new Keyboard),
  m_renderer(new ViewRender(this)),
  m_state(BROWSE),
  m_form(0),
  m_dirty(true)
{
  m_scrollPane->setTopLevel();
  m_scrollPane->setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
  m_scrollPane->setLocation(0, 0);
  m_scrollPane->setScrollIncrement(20);
  m_keyboard->setTopLevel(m_scrollPane);
  m_document.registerView(this);
  keysSetRepeat( 10, 5 );
}

void View::notify()
{
  Document::Status status(m_document.status());

  switch (status) {
    case Document::LOADED:
      {
        /** Broken by BWT.
         m_textArea->setStartLine( (-SCREEN_HEIGHT / m_textArea->font().height()) - 1);
        swiWaitForVBlank();
         */
        m_renderer->render();
        m_dirty = true;
      }
      break;
    case Document::INPROGRESS:
      {
        /** FIXME - broken by BWT.
        const char * l = "Loading..";
        m_textArea->setCursor(0, 0);
        m_textArea->print(l, strlen(l));
        */
        /** FIXME - double buffering means this no longer works
        unsigned int pc = m_document.percentLoaded();
        nds::Canvas::instance().fillRectangle(0,40, SCREEN_WIDTH, 20, nds::Color(31,31,31));
        nds::Canvas::instance().fillRectangle(0,40, pc*SCREEN_WIDTH / 100, 20, nds::Color(30,20,0));
        swiWaitForVBlank();
        */
      }
      break;
    default:
      break;
  }
}

void View::browse()
{
  u16 keys = keysDownRepeat();
  if (keys & KEY_START) {
    // nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
    /** FIXME - BWT
    m_preInputStartLine = m_textArea->startLine();
    m_keyboard->setVisible();
    m_state = KEYBOARD;
    */
  }
  if (keys & KEY_DOWN) {
    // scroll down ...
    /** FIXME - BWT
    m_textArea->setStartLine(m_textArea->startLine()+STEP);
    m_renderer->render();
    */
    m_scrollPane->down();
    m_dirty = true;
  }
  if (keys & KEY_UP) {
    // scroll up ...
    /** FIXME - BWT
    if (m_textArea->startLine() > ((-SCREEN_HEIGHT / m_textArea->font().height()) - 1)) {
      m_textArea->setStartLine(m_textArea->startLine()-STEP);
      m_renderer->render();
    }
    */
    m_scrollPane->up();
    m_dirty = true;
  }
  if (keys & KEY_LEFT) {
    m_controller.previous();
  }
  if (keys & KEY_RIGHT) {
    m_controller.next();
  }
  if (keys & KEY_A) {
    // render the node tree
    m_document.dumpDOM();
  }
  if (keys & KEY_TOUCH)
  {
    touchPosition tp = touchReadXY();
    m_dirty = m_keyboard->touch(tp.px, tp.py+SCREEN_HEIGHT);
    if (not m_dirty)
      m_dirty = m_scrollPane->touch(tp.px, tp.py+SCREEN_HEIGHT);
  }
}

void View::pressed(ButtonI * button)
{
  // FIXME - BWT - callback for button presses
  // Hmm, how to go from the button pressed to the form it lives in?
#if 0
  Link * clicked = m_textArea->clickLink(tp.px, tp.py+SCREEN_HEIGHT);
  if (clicked != 0)
  {
    clicked->highlight();
    string s = clicked->href();

    m_textArea->setStartLine(0);
    URI uri(m_document.uri());
    //cout << "Navigated to " << tmpURI.asString() << endl;
    // TODO - "navigate or download"..
    m_controller.doUri( uri.navigateTo(s).asString() );
  }
  else
  {
    // check for form click
    FormControl * formClick = m_textArea->clickForm(tp.px, tp.py+SCREEN_HEIGHT);
    if (formClick) 
    {
      // more complex than the link..
      m_form = formClick;
      switch (m_form->inputType())
      {
        case FormControl::ONE_CLICK:
          {
            // m_form->input
            URI uri(m_document.uri());
            m_form->input(tp.px, tp.py, m_controller, uri);
            m_state = BROWSE;
          }
          break;
        case FormControl::KEYBOARD:
          {
            m_state = FORM_KEYBOARD;
            m_preInputStartLine = m_textArea->startLine();
            m_keyboard->setVisible();
          }
          break;
        case FormControl::MENU:
          m_state = FORM;
          break;
      }
    }
  }
#endif
}

void View::linkClicked(Link * link)
{
  m_linkHref = link->href();
  /*
  URI uri(m_document.uri());
  cout << "Navigated to " << s << endl;
  // TODO - "navigate or download"..
  m_controller.doUri( uri.navigateTo(s).asString() );
  */
}

void View::keyboard()
{
  if (m_keyboard->visible()) {
    /** FIXME - BWT changes
    m_keyboard->handleInput();
    */
    if (not m_keyboard->visible())
    {
      nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
      if (not m_keyboard->result().empty())
      {
        /** FIXME  - conver UnicodeString to std::string
         * BWT Keyboard returns Unicode.
        m_controller.doUri(m_keyboard->result());
        */
      }
      else
      {
        /** FIXME BWT
        m_textArea->setStartLine(m_preInputStartLine);
        */
        m_renderer->render();
      }
    }
  }
  else
  {
    m_state = BROWSE;
  }
}

void View::formKeyboard()
{
  // FIXME - BWT changes break most of this.
#if 0
  if (m_keyboard->visible()) {
    m_keyboard->handleInput();
    if (not m_keyboard->visible())
    {
      nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
      if (not m_keyboard->result().empty())
      {
        UnicodeString ustr(string2unicode(m_keyboard->result()));
        m_form->input(ustr);
      }
      m_textArea->setStartLine(m_preInputStartLine);
      m_renderer->render();
    }
  }
  else
  {
    m_state = BROWSE;
  }
#endif
}

void View::tick()
{
  scanKeys();
  switch (m_state)
  {
    case BROWSE:
      browse();
      break;
    case FORM:
      break;
    case FORM_KEYBOARD:
      formKeyboard();
      break;
    case KEYBOARD:
      keyboard();
      break;
  }
  m_dirty |= m_keyboard->tick();

  if (m_dirty) {
    m_scrollPane->paint(m_scrollPane->preferredSize());
    m_keyboard->paint(m_scrollPane->preferredSize());
    nds::Canvas::instance().endPaint();
    m_dirty = false;
  }

  if (not m_linkHref.empty()) {
    URI uri(m_document.uri());
    string tmp(m_linkHref);
    m_linkHref = "";
    // cout << "Navigated to " << m_linkHref << endl;
    // TODO - "navigate or download"..
    m_controller.doUri( uri.navigateTo(tmp).asString() );
  }
}
