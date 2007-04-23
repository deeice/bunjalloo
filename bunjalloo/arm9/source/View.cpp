#include <libgen.h>
//#include <iostream>
#include "ndspp.h"
#include "libnds.h"
#include "View.h"
#include "Document.h"
#include "URI.h"
#include "TextArea.h"
#include "Canvas.h"
#include "ControllerI.h"
#include "Keyboard.h"
#include "Link.h"
#include "ViewRender.h"
#include "FormControl.h"

using namespace std;
const static int STEP(10);

View::View(Document & doc, ControllerI & c):
  m_document(doc), 
  m_controller(c),
  m_textArea(new TextArea),
  m_keyboard(new Keyboard(*m_textArea)),
  m_renderer(new ViewRender(this)),
  m_state(BROWSE),
  m_form(0)
{
  m_document.registerView(this);
  keysSetRepeat( 10, 5 );
}

void View::notify()
{
  Document::Status status(m_document.status());

  switch (status) {
    case Document::LOADED:
      {
        m_textArea->setStartLine( (-SCREEN_HEIGHT / m_textArea->font().height()) - 1);
        m_renderer->render();
        swiWaitForVBlank();
      }
      break;
    case Document::INPROGRESS:
      {
        const char * l = "Loading..";
        m_textArea->setCursor(0, 0);
        m_textArea->print(l, strlen(l));
        unsigned int pc = m_document.percentLoaded();
        nds::Canvas::instance().fillRectangle(0,40, SCREEN_WIDTH, 20, nds::Color(31,31,31));
        nds::Canvas::instance().fillRectangle(0,40, pc*SCREEN_WIDTH / 100, 20, nds::Color(30,20,0));
        swiWaitForVBlank();
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
    nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
    m_keyboard->setVisible();
    m_state = KEYBOARD;
  }
  if (keys & KEY_DOWN) {
    // scroll down ...
    m_textArea->setStartLine(m_textArea->startLine()+STEP);
    m_renderer->render();
  }
  if (keys & KEY_UP) {
    // scroll up ...
    m_textArea->setStartLine(m_textArea->startLine()-STEP);
    m_renderer->render();
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
  if (keys & KEY_TOUCH) {
    touchPosition tp = touchReadXY();
    Link * clicked = m_textArea->clickLink(tp.px, tp.py+SCREEN_HEIGHT);
    if (clicked != 0)
    {
      string s = clicked->href();
      string original = m_document.uri();
      URI tmpURI(original);
      tmpURI.navigateTo(s);
      m_textArea->setStartLine(0);
      //cout << "Navigated to " << tmpURI.asString() << endl;
      // TODO - "navigate or download"..
      m_controller.doUri( tmpURI.asString() );
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
              m_keyboard->setVisible();
            }
            break;
          case FormControl::MENU:
            m_state = FORM;
            break;
        }
      }
    }
  }
}

void View::keyboard()
{
  if (m_keyboard->visible()) {
    m_keyboard->handleInput();
    if (not m_keyboard->visible())
    {
      nds::Canvas::instance().fillRectangle(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, nds::Color(31,31,31));
      if (not m_keyboard->result().empty())
      {
        m_controller.doUri(m_keyboard->result());
      }
      else
      {
        m_textArea->setStartLine( m_textArea->startLine());
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
      m_textArea->setStartLine( m_textArea->startLine());
      m_renderer->render();
    }
  }
  else
  {
    m_state = BROWSE;
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
    case FORM:
      break;
    case FORM_KEYBOARD:
      formKeyboard();
      break;
    case KEYBOARD:
      keyboard();
      break;
  }
}
