#include "libnds.h"
#include <string>
#include "Palette.h"
#include "Keyboard.h"
#include "Rectangle.h"
#include "TextArea.h"
#include "Canvas.h"
#include "Sprite.h"
#include "pointer.h"

using namespace std;

static const int KEYBOARD_POSITION_Y(SCREEN_HEIGHT+72);
static const int KEYBOARD_POSITION_X(10);
static const int KEYBOARD_TILE_SIZE(24);
static const int KEYBOARD_ROW0(9);
static const int KEYBOARD_ROW1(19);
static const int KEYBOARD_ROW2(29);

static const int KEYBOARD_RESULT_POSITION_X(10);
static const int KEYBOARD_RESULT_POSITION_Y(10+SCREEN_HEIGHT);

static const char * s_characterMap[2]={
  "1234567890qwertyuiopasdfghjkl/zxcvbnm.",
  "!-#$%&*_?=QWERTYUIOPASDFGHJKL\\ZXCVBNM:",
};

class Key {
  public:
    Key(const char *value, int x, int y, int size):
      m_value(value),
      m_x(x),
      m_y(y),
      m_size(size),
      m_selected(false)
    { }

    void dimensions(Rectangle & rect, const Font & font) {
      int rectx = m_x - m_size/4;
      int recty = m_y - m_size/4;
      int width, height;
      font.textSize(m_value.c_str(), m_value.length(), width, height);
      width *= 3;
      width /= 2;
      width = width<m_size?m_size:width;
      height = height<m_size?m_size:height;
      rect.x = rectx;
      rect.y = recty;
      rect.w = width;
      rect.h = height;
    }

    void draw(TextArea * textArea) {
      Rectangle rect;
      dimensions(rect, textArea->font());
      unsigned short col(m_selected?nds::Color(0,31,31):nds::Color(30,29,31));
      nds::Canvas::instance().fillRectangle(rect.x, rect.y, 
          rect.w, rect.h, col);
      nds::Canvas::instance().drawRectangle(rect.x, rect.y, 
          rect.w, rect.h, nds::Color(0,0,6));
      textArea->setBackgroundColor(col);
      textArea->setStartLine(0,false);
      textArea->setCursor(m_x, m_y);
      textArea->print(m_value.c_str(),m_value.length());//,m_x,m_y);
      textArea->setDefaultColor();
    }

    bool hit(int testx, int testy, const Font & font) {
      Rectangle rect;
      dimensions(rect, font);
      nds::Canvas::instance().drawPixel(testx,testy, nds::Color(31,0,0));
      return rect.hit(testx, testy);
    }

    void setValue(const string & val) {
      m_value = val;
    }
    void setSelected(bool sel) {
      m_selected = sel;
    }

    string value() const
    {
      return m_value;
    }

    bool selected() const
    {
      return m_selected;
    }

    static void remove(Key * k)
    {
      delete k;
    }

  private:
    string m_value;
    int m_x;
    int m_y;
    int m_size;
    bool m_selected;

};

Keyboard::Keyboard(TextArea & textArea):
  m_shift(0),
  m_textArea(textArea),
  m_cursor(new nds::Sprite(1,16,16)),
  m_visible(false),
  m_redraw(false),
  m_ticks(0)
{
}

Keyboard::~Keyboard()
{
  setVisible(false);
}

void Keyboard::initKeys()
{
  int chars = strlen(s_characterMap[m_shift]);
  int x(KEYBOARD_POSITION_X);
  int y(KEYBOARD_POSITION_Y);
  int size(KEYBOARD_TILE_SIZE);
  for (int i = 0; i < chars ; i++) {
    char s[2];
    s[0] = s_characterMap[m_shift][i];
    s[1] = 0;
    m_keys.push_back(new Key(s, x, y, size));
    if (i == KEYBOARD_ROW0 or i == KEYBOARD_ROW1 or i == KEYBOARD_ROW2) {
      x = KEYBOARD_POSITION_X; 
      y += size;
    } else {
      x += size;
    }
  }
  m_keys.push_back(new Key("Enter", x, y, size));
  m_keys.push_back(new Key("Del", x, y-size*4, size));
  m_keys.push_back(new Key("Shift", KEYBOARD_POSITION_X, y+size, size));
  m_keys.push_back(new Key("               ", KEYBOARD_POSITION_X+size*3, y+size, size));
  m_keys.push_back(new Key("Clear", KEYBOARD_POSITION_X+size*8, y+size, size));
  for_each(m_keys.begin(), m_keys.end(), 
      bind2nd(mem_fun(&Key::draw),&m_textArea));
}

void Keyboard::initCursor()
{
  nds::ObjectPalette p(1);
  p.load(_binary_pointer_pal_bin_start, (int)&_binary_pointer_pal_bin_size);

  m_cursor->loadTileData(_binary_pointer_img_bin_start, (int)&_binary_pointer_img_bin_size);

  m_cursor->x(KEYBOARD_POSITION_X+KEYBOARD_TILE_SIZE/2);
  m_cursor->y(KEYBOARD_POSITION_Y-SCREEN_HEIGHT+(KEYBOARD_TILE_SIZE/2));
  m_cursor->enable(false);
}

void Keyboard::setShift(int shift)
{
  m_shift = shift;
  int chars = strlen(s_characterMap[m_shift]);
  vector<Key*>::iterator it(m_keys.begin());
  for (int i = 0; i < chars and it != m_keys.end(); ++i, ++it) {
    char s[2];
    s[0] = s_characterMap[m_shift][i];
    s[1] = 0;
    (*it)->setValue(s);
  }

}

void Keyboard::undrawResult()
{
  int width, height;
  m_textArea.font().textSize(m_result.c_str(), m_result.length(),width, height);
  nds::Canvas::instance().fillRectangle(KEYBOARD_RESULT_POSITION_X,KEYBOARD_RESULT_POSITION_Y,width,height,
      nds::Color(31,31,31));
}
void Keyboard::drawResult()
{
  m_textArea.setStartLine(0, false);
  m_textArea.setCursor(KEYBOARD_RESULT_POSITION_X, KEYBOARD_RESULT_POSITION_Y);
  m_textArea.print(m_result.c_str(),m_result.length());
//      KEYBOARD_RESULT_POSITION_X, KEYBOARD_RESULT_POSITION_Y
}

void Keyboard::clearResult()
{
  undrawResult();
  m_result.clear();
  drawResult();
}

void Keyboard::deleteLetter()
{
  if (m_result.size()) {
    undrawResult();
    m_result.erase(m_result.end()-1);
    drawResult();
  }
}

void Keyboard::doPress(Key * key) 
{
  if ((key->value() == "Del")) {
    deleteLetter();
  }
  if (key->value().length() == 1) {
    m_result += key->value();
    undrawResult();
    drawResult();
  }

  if ((key->value() == "Shift") or (m_shift and key->value().length() == 1)) {
    setShift(!m_shift);
  } 
  else if (key->value() == "Enter")
  {
    setVisible(false);
  } 
  else if (key->value() == "Clear")
  {
    clearResult();
  } 
}

void Keyboard::checkPress(int x, int y)
{
  vector<Key*>::iterator it(m_keys.begin());
  for (; it != m_keys.end() ; ++it) {
    Key * key = *it;
    if (key->hit(x, y, m_textArea.font())) {
      key->setSelected(true);
      doPress(key);
      m_redraw = true;
      m_ticks = 30;
      if (not visible()) {
        return;
      }
    } else {
      key->setSelected(false);
    }
  }
}

std::string Keyboard::result() const
{
  return m_result;
}

string Keyboard::selected()
{
  vector<Key*>::iterator it(find_if(m_keys.begin(), m_keys.end(), mem_fun(&Key::selected)));
  if (it != m_keys.end())
  {
    (*it)->setSelected(false);
    return (*it)->value();
  }
  return "";
}

void Keyboard::handleButtons(unsigned short keys)
{
  if (keys & KEY_A) {
    checkPress(m_cursor->x(), m_cursor->y()+m_cursor->height()+SCREEN_HEIGHT);
  }
  if (keys & KEY_B) {
    deleteLetter();
  }
  if (keys & KEY_UP) {
    m_cursor->y(m_cursor->y()-KEYBOARD_TILE_SIZE);
  }
  if (keys & KEY_DOWN) {
    m_cursor->y(m_cursor->y()+KEYBOARD_TILE_SIZE);
  }
  if (keys & KEY_LEFT) {
    m_cursor->x(m_cursor->x()-KEYBOARD_TILE_SIZE);
  }
  if (keys & KEY_RIGHT) {
    m_cursor->x(m_cursor->x()+KEYBOARD_TILE_SIZE);
  }
}

void Keyboard::handleInput() 
{
  u16 keys = keysDownRepeat();
  static const int interestingKeys((KEY_A | KEY_B | KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT ));
  if (not m_cursor->enabled()
      and (keys & interestingKeys))
  {
    m_cursor->enable();
    m_cursor->update();
    return;
  }
  if (m_cursor->enabled() and (keys & KEY_TOUCH))
  {
    m_cursor->enable(false);
    m_cursor->update();
    return;
  }

  if (keys & interestingKeys)
  {
    handleButtons(keys);
  } else if (keys & KEY_TOUCH) {
    touchPosition tp = touchReadXY();
    checkPress(tp.px, tp.py+SCREEN_HEIGHT);
  }

  if (keys & KEY_START) {
    setVisible(false);
  }

  checkRedraw();
}

void Keyboard::checkRedraw() 
{
  if (m_ticks>0)
    m_ticks--;
  if (m_ticks == 0) {
    m_redraw = true;
    m_ticks = -1;
    checkPress(0,0);
  }
  if (m_redraw) {
    m_redraw = false;
    for_each(m_keys.begin(), m_keys.end(), 
        bind2nd(mem_fun(&Key::draw),&m_textArea));
  }
  if (m_cursor->enabled()) {
    m_cursor->update();
  }
}

void Keyboard::setVisible(bool visible)
{
  if (visible)
  {
    clearResult();
    initKeys();
    initCursor();
  } 
  else
  {
    for_each(m_keys.begin(), m_keys.end(), Key::remove);
    m_keys.clear();
    m_cursor->enable(false);
    m_cursor->update();
  }
  m_visible = visible;
}

bool Keyboard::visible() const
{
  return m_visible;
}
