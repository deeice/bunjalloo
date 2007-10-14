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
#include <libgen.h>
#include "ndspp.h"
#include "libnds.h"
#include "Canvas.h"
#include "Config.h"
#include "Controller.h"
#include "Document.h"
#include "FormControl.h"
#include "Keyboard.h"
#include "Link.h"
#include "LinkHandler.h"
#include "TextField.h"
#include "Toolbar.h"
#include "ScrollPane.h"
#include "URI.h"
#include "View.h"
#include "ViewRender.h"
#include "WidgetColors.h"

using namespace std;
const static int STEP(1);

View::View(Document & doc, Controller & c):
  m_document(doc), 
  m_controller(c),
  m_scrollPane(new ScrollPane),
  m_keyboard(new Keyboard),
  m_renderer(new ViewRender(this)),
  m_addressBar(new TextField(UnicodeString())),
  m_toolbar(new Toolbar(doc, c, *this)),
  m_state(BROWSE),
  m_form(0),
  m_linkHandler(new LinkHandler(this)),
  m_dirty(true),
  m_lastX(0),
  m_lastY(0)
{
  m_scrollPane->setTopLevel();
  m_scrollPane->setLocation(0, 0);
  m_scrollPane->setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
  m_scrollPane->setScrollIncrement(20);
  m_keyboard->setTopLevel(m_scrollPane);
  m_document.registerView(this);
  keysSetRepeat( 10, 5 );
  m_toolbar->setVisible();
}

View::~View()
{
  delete m_keyboard;
  delete m_renderer;
  delete m_addressBar;
  delete m_toolbar;
  delete m_linkHandler;
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
        string refresh;
        int refreshTime;
        m_document.refresh(refresh, refreshTime);
        if (not refresh.empty() and refreshTime >= 0)
        {
          m_refreshing = refreshTime * 30;
        }
        else
        {
          m_refreshing = 0;
        }
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

void View::enterUrl()
{
  m_addressBar->setText(string2unicode(m_document.uri()));
  m_keyboard->editText(m_addressBar);
  m_toolbar->setVisible(false);
  m_state = KEYBOARD;
  m_dirty = true;
}

void View::browse()
{
  u16 keys = keysDownRepeat();
  if (keys & KEY_START) {
    // nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
    enterUrl();
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
  if ( (keys & KEY_LEFT) or (keys & KEY_L) ) {
    m_controller.previous();
  }
  if ( (keys & KEY_RIGHT) or (keys & KEY_R) ) {
    m_controller.next();
  }
  if (keys & KEY_A) {
    // render the node tree
    m_document.dumpDOM();
  }
  if (keys & KEY_TOUCH)
  {
    touchPosition tp = touchReadXY();
    m_lastX = tp.px;
    m_lastY = tp.py+SCREEN_HEIGHT;

    if (not m_keyboard->visible())
    {
      if ( m_toolbar->touch(tp.px, tp.py) )
      {
        return;
      }
      if ( m_linkHandler->visible())
      {
        m_dirty = m_linkHandler->touch(m_lastX, m_lastY);
        if (m_dirty)
          return;
      }
    }

    m_dirty = m_keyboard->touch(m_lastX, m_lastY);
    if (not m_dirty)
      m_dirty = m_scrollPane->touch(m_lastX, m_lastY);
    if (m_keyboard->visible())
      m_toolbar->setVisible(false);
    else if (not m_scrollPane-> scrollBarHit(m_lastX, m_lastY))
      m_toolbar->setVisible(true);
  }
  if (m_refreshing > 0)
  {
    m_refreshing--;
    if (m_refreshing == 0)
    {
      int tmp;
      m_document.refresh(m_linkHref, tmp);
    }
  }
}

void View::pressed(ButtonI * button)
{
  if (m_form)
    return;
  // FIXME - BWT - callback for button presses
  // Hmm, how to go from the button pressed to the form it lives in?
  // Originally, each control had a HtmlElement associated with it.
  FormControl * formControl = (FormControl*)button;
  // check for form click
  // more complex than the link..
  m_form = formControl;
}

void View::linkClicked(Link * link)
{
  // there are types of link
  // 1) Anchor only
  // 2) Image only
  // 3) Image and anchor

  bool isAnchor = link->eventType() == Link::STATE_LINK;
  bool isImage  = link->color() == WidgetColors::LINK_IMAGE;

  if (isAnchor and not isImage)
  {
    m_linkHref = link->href();
  }
  else if (isImage and not isAnchor)
  {
    // image link?
    m_linkHref = link->src();
  }
  else // isAnchor and isImage
  {
    m_linkHandler->setLink(link);
    m_linkHandler->setLocation(m_lastX, m_lastY);
    m_linkHandler->setVisible();
  }
}

void View::keyboard()
{
  u16 keys = keysDownRepeat();
  if (keys & KEY_TOUCH)
  {
    touchPosition tp = touchReadXY();
    m_dirty = m_keyboard->touch(tp.px, tp.py+SCREEN_HEIGHT);
    if (not m_dirty)
      m_dirty = m_scrollPane->touch(tp.px, tp.py+SCREEN_HEIGHT);
  }
}

void View::tick()
{
  scanKeys();
  switch (m_state)
  {
    case BROWSE:
      browse();
      break;
    case KEYBOARD:
      keyboard();
      break;
  }
  m_dirty |= m_keyboard->tick();
  m_toolbar->tick();

  if (m_dirty) {
    const static nds::Rectangle clip = {0, 0, nds::Canvas::instance().width(), nds::Canvas::instance().height()};
    m_scrollPane->paint(clip);
    m_keyboard->paint(clip);
    m_linkHandler->paint(clip);
    nds::Canvas::instance().endPaint();
    m_dirty = false;
    m_toolbar->updateIcons();
  }

  if (m_state == KEYBOARD and not m_keyboard->visible()) {
    m_state = BROWSE;
    string newAddress = unicode2string(m_keyboard->result());
    if (not newAddress.empty() and m_keyboard->selected() == Keyboard::OK)
    {
      m_toolbar->setVisible(true);
      m_controller.doUri(newAddress);
    }
  }

  // clicked a link:
  if (not m_linkHref.empty()) {
    URI uri(m_document.uri());
    string tmp(m_linkHref);
    m_linkHref = "";
    // cout << "Navigated to " << m_linkHref << endl;
    // TODO - "navigate or download"..
    m_controller.doUri( uri.navigateTo(tmp).asString() );
  }

  if (m_form)
  {
    URI uri(m_document.uri());
    FormControl * tmp = m_form;
    m_form = 0;

    tmp->input(m_controller, uri);
    m_state = BROWSE;
  }
}
