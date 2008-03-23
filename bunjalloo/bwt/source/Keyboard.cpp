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
#include "libnds.h"
#include "Button.h"
#include "Canvas.h"
#include "EditableTextArea.h"
#include "Keyboard.h"
#include "Language.h"
#include "RichTextArea.h"
#include "ScrollPane.h"
#include "Stylus.h"
#include "TextAreaFactory.h"
#include "TextEntryI.h"

// TODO: use config for this?
// Based on http://en.wikipedia.org/wiki/Image:KB_US-International.svg
static const char * NUMBERS          = "1234567890-=";
static const char * NUMBERS_SHIFT    = "!@#$%^&*()_+";
static const char * LETTERS          = "qwertyuiopasdfghjklzxcvbnm,./[]:\"";
static const char * LETTERS_SHIFT    = "QWERTYUIOPASDFGHJKLZXCVBNM<>?{};'";

// QWERTYUIOPASDFGHJKLZXCVBNM{};'<>?
// ~àáâãäåèéêëìíîïñðòóôõöùúûü¿£¥ýþç¡
// ÀÀÁÂÃÄÅÈÉÊÝÞËÌÍÎÏÑÐÇßÒÓÔÕÖÙÚÛÜ¢|ÿ
static const char * EXTRA =
"~\xe1\xe2\xe3\xe4\xe5\xe6\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf9\xfa\xfb\xfc\xbf\xa3\xa5\xfd\xfe\xe7\xa1";
static const char * EXTRA_SHIFT =
"\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd9\xda\xdb\xdc\xa2\x7c\xff\xdd\xde\xc7\xdf";

static const UnicodeString BACKSPACE_STR(string2unicode("BkSp."));
static const UnicodeString CAPS_STR(string2unicode("Cap"));
static const UnicodeString ENTER_STR(string2unicode(" Enter"));
static const UnicodeString SHIFT_STR(string2unicode("Shift"));
static const UnicodeString SPACE_STR(string2unicode(" "));
static const UnicodeString EXTRA_STR(string2unicode(" Alt"));
static const UnicodeString CLEAR_STR(string2unicode(" Clr"));

const static int KEY_HEIGHT = 18;
const static int KEY_WIDTH = 19;
const static int GAP = 10;
const static int SCROLLPANE_POS_Y = 4;
const static int SCROLLPANE_SIZE = SCREEN_HEIGHT*2/5 - GAP;
const static int INITIAL_Y = SCROLLPANE_SIZE + GAP + SCREEN_HEIGHT;
const static int INITIAL_X = 22;

const static int ROW1_LENGTH = 10;
const static int ROW2_LENGTH = 9;
const static int ROW3_LENGTH = 10;
const static int ROW4_LENGTH = 4;

const static int TICK_COUNT = 20;
const static int SCROLLBAR_DECOR = 7;

Keyboard::Keyboard():
  Component(),
  m_extra(false),
  m_shift(false),
  m_capsLock(false),
  m_selectedStatus(OK),
  m_scrollPane(new ScrollPane),
  m_textArea((EditableTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_EDIT)),
  m_richTextArea((RichTextArea*)TextAreaFactory::create(TextAreaFactory::TXT_RICH)),
  m_shiftKey(new Button),
  m_capsLockKey(new Button),
  // m_tabKey(new Button),
  m_enterKey(new Button),
  m_backspaceKey(new Button),
  //m_deleteKey(new Button),
  m_spaceKey(new Button),
  m_extraKey(new Button),
  m_ok(new Button),
  m_cancel(new Button),
  m_clearKey(new Button)
{
  initUI();
  int x = INITIAL_X;
  int y = INITIAL_Y;
  const char * text(NUMBERS);
  createRow(x-KEY_WIDTH/2, y, text, strlen(NUMBERS));

  // QWERTY
  text = LETTERS;
  y += KEY_HEIGHT;
  createRow(x, y, text, ROW1_LENGTH);

  // ASDFG
  x += KEY_WIDTH/2;
  y += KEY_HEIGHT;
  text += ROW1_LENGTH;
  createRow(x, y, text, ROW2_LENGTH);

  // ZXCVB
  x += KEY_WIDTH/2;
  y += KEY_HEIGHT;
  text += ROW2_LENGTH;
  createRow(x, y, text, ROW3_LENGTH);


  // special, end of chars. [] and /"
  x += KEY_WIDTH/2;
  y += KEY_HEIGHT;
  text += ROW3_LENGTH;
  createRow(x, y, text, 2);
  x += (KEY_WIDTH)*(ROW3_LENGTH-3);
  text += 2;
  createRow(x, y, text, 2);

  // shift, capsLock, tab, enter, backspace, delete, altkeys, space
  // backspace - at the end of the Q-P row, 2 keys wide
  createSpecialKey(INITIAL_X+(ROW1_LENGTH)*(KEY_WIDTH), INITIAL_Y+KEY_HEIGHT,
      KEY_WIDTH*2, KEY_HEIGHT,
      BACKSPACE_STR,
      m_backspaceKey);

  // enter - at the end of the A-L row, 2.5 keys wide.
  createSpecialKey(INITIAL_X+(ROW2_LENGTH)*(KEY_WIDTH)+KEY_WIDTH/2, INITIAL_Y+(KEY_HEIGHT*2),
      KEY_WIDTH*5/2, KEY_HEIGHT,
      ENTER_STR,
      m_enterKey);

  // m_clearKey - at the end of the space and final row
  createSpecialKey(INITIAL_X+KEY_WIDTH/2+(KEY_WIDTH)*ROW3_LENGTH-1, INITIAL_Y+(KEY_HEIGHT*4),
      (KEY_WIDTH*3/2), KEY_HEIGHT,
      CLEAR_STR,
      m_clearKey);

  // spaceKey - in between the final 4 chars on the final row
  createSpecialKey(INITIAL_X+(KEY_WIDTH*3/2)+(KEY_WIDTH)*2-1, INITIAL_Y+(KEY_HEIGHT*4),
      (KEY_WIDTH)*5, KEY_HEIGHT,
      SPACE_STR,
      m_spaceKey);

  // ok key - floating after the keyboard
  createSpecialKey(INITIAL_X+KEY_WIDTH, INITIAL_Y+(KEY_HEIGHT*5)+KEY_HEIGHT/3,
      (KEY_WIDTH)*3, KEY_HEIGHT,
      T("ok"),
      m_ok);

  // cancel key - floating after the keyboard
  createSpecialKey(INITIAL_X+KEY_WIDTH*8, INITIAL_Y+(KEY_HEIGHT*5)+KEY_HEIGHT/3,
      (KEY_WIDTH)*3, KEY_HEIGHT,
      T("cancel"),
      m_cancel);

  // caps - at the start of the a-l row. 1.5 keys wide
  createSpecialKey(INITIAL_X-KEY_WIDTH, INITIAL_Y+(KEY_HEIGHT*2),
      (KEY_WIDTH*3/2), KEY_HEIGHT,
      CAPS_STR,
      m_capsLockKey);

  // shift - at the start of the z-m row, 2 keys wide.
  createSpecialKey(INITIAL_X-KEY_WIDTH, INITIAL_Y+(KEY_HEIGHT*3),
      KEY_WIDTH*2-1, KEY_HEIGHT,
      SHIFT_STR,
      m_shiftKey);

  // m_extraKey
  createSpecialKey(INITIAL_X-(KEY_WIDTH/2)-2, INITIAL_Y+(KEY_HEIGHT*4),
      (KEY_WIDTH*2), KEY_HEIGHT,
      EXTRA_STR,
      m_extraKey);

  // By adding the m_scrollPane to the Component::m_children it gets deleted
  // in the destructor.
  add(m_scrollPane);
  add(m_richTextArea);
  setVisible(false);
  Stylus::instance()->registerListener(this);
}

Keyboard::~Keyboard()
{
  Stylus::instance()->unregisterListener(this);
}

void Keyboard::initUI()
{
  m_richTextArea->setCentred();
  m_richTextArea->setOutlined();
  m_textArea->setParentScroller(m_scrollPane);
  m_scrollPane->add(m_textArea);
  m_scrollPane->setTopLevel(false);
  m_scrollPane->setSize(nds::Canvas::instance().width(),SCROLLPANE_SIZE);
  m_scrollPane->setLocation(0, SCREEN_HEIGHT+SCROLLPANE_POS_Y);
  m_scrollPane->setScrollIncrement(m_textArea->font().height());
  m_scrollPane->setStretchChildren(true);
  m_scrollPane->setVisible(true);
}

void Keyboard::createSpecialKey(int x, int y, int w, int h, const UnicodeString & text, Button * button)
{
  button->setSize(w, h);
  button->setLocation(x, y);
  button->setText(text);
  add(button);
  button->setListener(this);
}

void Keyboard::updateRow(const char * newText, int keys, int offset)
{
  for (int i = 0; i < keys; ++i)
  {
    Button & key(*(Button*)m_children[i+offset]);
    unicodeint uchar[] = { (unsigned char)newText[i]&0xff, 0};
    key.setText(UnicodeString(uchar));
  }
}

void Keyboard::createRow(int x, int y, const char * text, int keys)
{
  for (int i = 0; i < keys; ++i)
  {
    Button * key = new Button();
    unicodeint uchar[] = {text[i], 0};
    key->setSize(KEY_WIDTH, KEY_HEIGHT);
    key->setText(UnicodeString(uchar));
    //key->setLocation(x+i*(KEY_WIDTH+1), y);
    key->setLocation(x+i*(KEY_WIDTH), y);
    add(key);
    key->setListener(this);
  }
}

UnicodeString Keyboard::result() const
{
  UnicodeString tmp;
  m_textArea->text(tmp);
  return tmp;
}

void Keyboard::applyResult()
{
  UnicodeString tmp;
  m_textArea->text(tmp);
  if (m_entry) {
    m_entry->setText(tmp);
  }
}

void Keyboard::paint(const nds::Rectangle & clip)
{
  if (visible())
  {
    std::vector<Component*>::iterator it(m_children.begin());
    for (; it != m_children.end(); ++it)
    {
      Component * c(*it);
      c->paint(c->bounds());
    }
    m_scrollPane->paint(m_scrollPane->bounds());
  }
}

void Keyboard::editText(TextEntryI * entry)
{
  m_topLevel->setVisible(false);
  this->setVisible();
  m_initialText.clear();
  entry->text(m_initialText);
  //m_textArea->clearText();
  m_textArea->setEchoText(entry->echoText());
  m_textArea->setText(m_initialText);
  m_entry = entry;
  layoutViewer();
}

Keyboard::SpecialKey Keyboard::buttonToSpecialKey(const ButtonI * button)
{
  if (button == m_shiftKey) return SPKY_SHIFT;
  if (button == m_capsLockKey) return SPKY_CAPS;
  if (button == m_enterKey) {
    if (multiLine()) {
      return SPKY_ENTER;
    }
    return SPKY_OK;
  }
  if (button == m_backspaceKey) return SPKY_BACKSPACE;
  if (button == m_spaceKey) return SPKY_SPACE;
  if (button == m_extraKey) return SPKY_EXTRA;
  if (button == m_ok) return SPKY_OK;
  if (button == m_cancel) return SPKY_CANCEL;
  if (button == m_clearKey) return SPKY_CLEAR;
  return SPKY_UNKNOWN;
}

void Keyboard::pressed(ButtonI * button)
{
  SpecialKey key = buttonToSpecialKey(button);
  switch (key)
  {
    case SPKY_SHIFT:
      m_shift = not m_shift;
      updateModifierKeys();
      break;
    case SPKY_CAPS:
      m_capsLock = not m_capsLock;
      updateModifierKeys();
      break;
    case SPKY_ENTER:
      appendText(string2unicode("\n"));
      break;
    case SPKY_BACKSPACE:
      m_textArea->deleteChar();
      layoutViewer();
      break;
    case SPKY_SPACE:
      appendText(((Button*)button)->text());
      break;
    case SPKY_EXTRA:
      m_extra = not m_extra;
      updateModifierKeys();
      break;
    case SPKY_CANCEL:
      m_selectedStatus = CANCEL;
      m_textArea->clearText();
      m_textArea->appendText(m_initialText);
      tick();
      m_topLevel->setVisible();
      this->setVisible(false);
      applyResult();
      break;
    case SPKY_OK:
      m_selectedStatus = OK;
      tick();
      m_topLevel->setVisible();
      this->setVisible(false);
      applyResult();
      break;
    case SPKY_CLEAR:
      m_textArea->clearText();
      layoutViewer();
      break;
    case SPKY_UNKNOWN:
      // anything else
      appendText(((Button*)button)->text());
      m_shift = false;
      m_extra = false;
      break;
  }
}

void Keyboard::appendText(const UnicodeString & text)
{
  m_textArea->appendText(text);
  layoutViewer();
}

void Keyboard::layoutViewer()
{
  // should scroll to caret line - only if it is not already visible
  // otherwise the scrolling is not very stable.
  if (not m_textArea->caretVisible())
  {
    int scrollTo(m_textArea->caretLine() * m_textArea->font().height() * 256 / m_textArea->preferredSize().h);
    m_scrollPane->scrollToPercent(scrollTo);
  }
}

void Keyboard::updateModifierKeys()
{
  m_extraKey->setSelected(m_extra);
  m_shiftKey->setSelected(m_shift);
  m_capsLockKey->setSelected(m_capsLock);

  bool caps = (m_shift and not m_capsLock) or (m_capsLock and not m_shift);
  const char * text;
  const char * numbers;
  if (m_extra)
  {
    numbers = NUMBERS;
    if (caps)
    {
      text = EXTRA_SHIFT;
    }
    else
    {
      text = EXTRA;
    }
  }
  else
  {
    numbers = NUMBERS;
    if (caps)
    {
      text = LETTERS_SHIFT;
    }
    else
    {
      text = LETTERS;
    }
    if (m_shift)
    {
      numbers = NUMBERS_SHIFT;
    }
  }

  updateLayout(text, numbers);
}

void Keyboard::updateLayout(const char * text, const char * numbers)
{
  int start = strlen(NUMBERS);
  updateRow(numbers, start, 0);
  int size = ROW1_LENGTH + ROW2_LENGTH + ROW3_LENGTH;
  updateRow(text, size, start);
  // now the odd keys
  // special, end of chars. [] and /"
  text += size;
  start += size;
  updateRow(text, ROW4_LENGTH, start);

}

bool Keyboard::stylusUp(const Stylus * stylus)
{
  if (not visible())
    return false;
  FOR_EACH_CHILD(stylusUp);
  return false;
}

bool Keyboard::stylusDownFirst(const Stylus * stylus)
{
  if (not visible())
    return false;
  FOR_EACH_CHILD(stylusDownFirst);
  return false;
}
bool Keyboard::stylusDownRepeat(const Stylus * stylus)
{
  if (not visible())
    return false;
  FOR_EACH_CHILD(stylusDownRepeat);
  return false;
}
bool Keyboard::stylusDown(const Stylus * stylus)
{
  if (not visible())
    return false;
  FOR_EACH_CHILD(stylusDown);
  return false;
}

void Keyboard::setTopLevel(Component * topLevel)
{
  m_topLevel = topLevel;
}

bool Keyboard::tick()
{
  if (not visible())
  {
    return false;
  }

#ifndef ARM9
  static int pressed = 0;
  unicodeint sdlKeyPress[2] = { keysRealKeyboard(), 0};
  if ((not pressed) and sdlKeyPress[0])
  {
    pressed = 30;
    appendText(UnicodeString(sdlKeyPress));
    return true;
  }
  if (pressed)
    pressed--;
#endif
  return false;
}

bool Keyboard::multiLine() const
{
  return m_entry->isMultiLine();
}

void Keyboard::setTitle(const UnicodeString & title)
{
  m_richTextArea->clearText();
  m_richTextArea->appendText(title);
  m_richTextArea->setLocation(0, SCREEN_HEIGHT -  m_richTextArea->preferredSize().h-GAP);
  m_richTextArea->setSize(nds::Canvas::instance().width()-1,m_richTextArea->preferredSize().h);
}
