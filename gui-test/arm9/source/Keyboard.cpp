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
#include "Keyboard.h"
#include "TextAreaFactory.h"
#include "TextArea.h"
#include "ScrollPane.h"
#include "Button.h"
#include "Canvas.h"
#include "TextEntryI.h"

// TODO: use config for this
// Based on http://en.wikipedia.org/wiki/Image:KB_US-International.svg
static const char * NUMBERS          = "1234567890-=";
static const char * NUMBERS_SHIFT    = "!@#$%^&*()_+";
static const char * LETTERS          = "qwertyuiopasdfghjklzxcvbnm,./[]:\"";
static const char * LETTERS_SHIFT    = "QWERTYUIOPASDFGHJKLZXCVBNM<>?{};'";

// QWERTYUIOPASDFGHJKLZXCVBNM{};'<>?
// ààáâãäåèéêëìíîïñðòóôõöùúûü¿£¥ýþç¡
// ÀÀÁÂÃÄÅÈÉÊÝÞËÌÍÎÏÑÐÇßÒÓÔÕÖÙÚÛÜ¢|ÿ
static const char * EXTRA =
"\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf9\xfa\xfb\xfc\xbf\xa3\xa5\xfd\xfe\xe7\xa1";
static const char * EXTRA_SHIFT =
"\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd9\xda\xdb\xdc\xa2\x7c\xff\xdd\xde\xc7\xdf";

static const UnicodeString BACKSPACE_STR(string2unicode("BkSp."));
static const UnicodeString CAPS_STR(string2unicode("Cap"));
static const UnicodeString ENTER_STR(string2unicode(" Enter"));
static const UnicodeString SHIFT_STR(string2unicode("Shift"));
static const UnicodeString SPACE_STR(string2unicode(" "));
static const UnicodeString EXTRA_STR(string2unicode(" Alt"));

const static int KEY_HEIGHT = 18;
const static int KEY_WIDTH = 18;
const static int INITIAL_Y = 192/2 + 192;
const static int INITIAL_X = 22;

const static int ROW1_LENGTH = 10;
const static int ROW2_LENGTH = 9;
const static int ROW3_LENGTH = 10;

Keyboard::Keyboard():
  Component(),
  m_extra(false),
  m_shift(false),
  m_capsLock(false),
  m_scrollPane(new ScrollPane),
  m_textArea(TextAreaFactory::create(true)),
  m_shiftKey(new Button),
  m_capsLockKey(new Button),
  // m_tabKey(new Button),
  m_enterKey(new Button),
  m_backspaceKey(new Button),
  //m_deleteKey(new Button),
  m_spaceKey(new Button),
  m_extraKey(new Button)
{
  setVisible(false);
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
  x += (KEY_WIDTH+1)*(ROW3_LENGTH-3);
  text += 2;
  createRow(x, y, text, 2);

  // shift, capsLock, tab, enter, backspace, delete, altkeys, space
  // backspace - at the end of the q-p row, 2 keys wide
  createSpecialKey(INITIAL_X+(ROW1_LENGTH)*(KEY_WIDTH+1), INITIAL_Y+KEY_HEIGHT,
      KEY_WIDTH*2+1, KEY_HEIGHT, 
      BACKSPACE_STR, 
      m_backspaceKey);

  // caps - at the start of the a-l row. 1.5 keys wide
  createSpecialKey(INITIAL_X-KEY_WIDTH, INITIAL_Y+(KEY_HEIGHT*2),
      KEY_WIDTH*3/2-1, KEY_HEIGHT, 
      CAPS_STR, 
      m_capsLockKey);

  // enter - at the end of the a-l row, 2.5 keys wide.
  createSpecialKey(INITIAL_X+(ROW2_LENGTH)*(KEY_WIDTH+1)+KEY_WIDTH/2, INITIAL_Y+(KEY_HEIGHT*2),
      KEY_WIDTH*5/2 + 2, KEY_HEIGHT, 
      ENTER_STR, 
      m_enterKey);

  // shift - at the start of the z-m row, 2 keys wide.
  createSpecialKey(INITIAL_X-KEY_WIDTH, INITIAL_Y+(KEY_HEIGHT*3),
      KEY_WIDTH*2-1, KEY_HEIGHT, 
      SHIFT_STR, 
      m_shiftKey);

  // spaceKey - in between the final 4 chars on the final row
  createSpecialKey(INITIAL_X+(KEY_WIDTH*3/2)+(KEY_WIDTH+1)*2, INITIAL_Y+(KEY_HEIGHT*4),
      (KEY_WIDTH)*5+4, KEY_HEIGHT, 
      SPACE_STR, 
      m_spaceKey);
  // m_extraKey
  createSpecialKey(INITIAL_X-(KEY_WIDTH/2), INITIAL_Y+(KEY_HEIGHT*4),
      (KEY_WIDTH*2)-1, KEY_HEIGHT, 
      EXTRA_STR, 
      m_extraKey);
  
}
void Keyboard::initUI()
{
  m_scrollPane->add(m_textArea);
  m_scrollPane->setTopLevel(false);
  m_scrollPane->setSize(nds::Canvas::instance().width(),192/2-10);
  m_scrollPane->setLocation(0, 194);
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
    unsigned int uchar[] = { (unsigned char)newText[i]&0xff, 0};
    key.setText(UnicodeString(uchar));
  }
}

void Keyboard::createRow(int x, int y, const char * text, int keys)
{
  for (int i = 0; i < keys; ++i)
  {
    Button * key = new Button();
    unsigned int uchar[] = {text[i], 0};
    key->setSize(KEY_WIDTH, KEY_HEIGHT);
    key->setText(UnicodeString(uchar));
    key->setLocation(x+i*(KEY_WIDTH+1), y);
    add(key);
    key->setListener(this);
  }
}

UnicodeString Keyboard::result() const
{
  return m_result;
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
    m_textArea->setSize(m_scrollPane->width(), m_textArea->preferredSize().h);
    //m_scrollPane->setSize(m_scrollPane->width(), m_scrollPane->height());
    m_scrollPane->paint(m_scrollPane->bounds());
  }
}

void Keyboard::editText(TextEntryI * entry)
{
  printf("Edit the text: %s\n", unicode2string(entry->text()).c_str());
  m_topLevel->setVisible(false);
  this->setVisible();
  m_textArea->appendText(entry->text());
}

Keyboard::SpecialKey Keyboard::buttonToSpecialKey(const ButtonI * button)
{
  if (button == m_shiftKey) return SPKY_SHIFT;
  if (button == m_capsLockKey) return SPKY_CAPS;
  if (button == m_enterKey) return SPKY_ENTER;
  if (button == m_backspaceKey) return SPKY_BACKSPACE;
  if (button == m_spaceKey) return SPKY_SPACE;
  if (button == m_extraKey) return SPKY_EXTRA;
  return SPKY_UNKNOWN;
}

void Keyboard::pressed(ButtonI * button)
{
  // a button press...
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
      updateTicksForUI(button);
      m_textArea->appendText(string2unicode("\n"));
      break;
    case SPKY_BACKSPACE:
      updateTicksForUI(button);
      break;
    case SPKY_SPACE:
      updateTicksForUI(button);
      break;
    case SPKY_EXTRA:
      m_extra = not m_extra;
      updateModifierKeys();
      break;
    case SPKY_UNKNOWN:
      /* anything else */
      updateTicksForUI(button);
      m_textArea->appendText(((Button*)button)->text());
      printf("%s\n", m_textArea->asString().c_str());
      m_shift = false;
      m_extra = false;
      break;
  }
}

void Keyboard::updateTicksForUI(ButtonI * button)
{
  m_ticks = 1;
  tick();
  button->setSelected(true);
  if (m_shift or m_extra)
  {
    m_ticks = 20;
  }
  else {
    m_ticks = 40;
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
  updateRow(text, 4, start);

}

bool Keyboard::touch(int x, int y)
{
  bool repaint(false);
  if (visible())
  {
    std::vector<Component*>::iterator it(m_children.begin());
    for (; it != m_children.end(); ++it)
    {
      Component * c(*it);
      if ( c->touch(x, y) ) {
        repaint = true;
      }
    }
  }
  return repaint;
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

  if (m_ticks>0)
  {
    m_ticks--;
    if (m_ticks == 0)
    {
      std::vector<Component*>::iterator it(m_children.begin());
      for (; *it != m_backspaceKey; ++it)
      {
        Button * c((Button*)*it);
        c->setSelected(false);
      }
      m_spaceKey->setSelected(false);
      m_backspaceKey->setSelected(false);
      m_enterKey->setSelected(false);
      updateModifierKeys();
      return true;
    }
  }
  return false;
}
