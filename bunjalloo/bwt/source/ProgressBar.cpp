/*
  Copyright (C) 2008 Richard Quirk

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
#include "Palette.h"
#include "Canvas.h"
#include "TextArea.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(int min, int max): TextContainer(),
  m_min(min),
  m_max(max),
  m_value(min),
  m_showString(false)
{
  setBackgroundColor(nds::Color(0,29,10));
}

void ProgressBar::setShowString(bool show)
{
  m_showString = show;
  m_dirty = true;
}

bool ProgressBar::showString() const
{
  return m_showString;
}

void ProgressBar::setValue(int val)
{
  m_value = val;
  m_dirty = true;
}

unsigned int ProgressBar::value() const
{
  return m_value;
}

void ProgressBar::setMin(int min)
{
  m_min = min;
  m_dirty = true;
}

unsigned int ProgressBar::min() const
{
  return m_min;
}

void ProgressBar::setMax(int max)
{
  m_max = max;
  m_dirty = true;
}

unsigned int ProgressBar::max() const
{
  return m_max;
}

void ProgressBar::paint(const nds::Rectangle & clip)
{
  // fill the bg rectangle in the text container to the desired colour
  nds::Canvas::instance().setClip(clip);
  // fill to %
  // min = x
  // max = x+w
  // val = x + ( (w*val) / (max-min))
  nds::Canvas::instance().drawRectangle(clip.x, clip.y, clip.w-1, clip.h, 0);
  unsigned int range = (clip.w * m_value) / (m_max-m_min);
  nds::Rectangle progress = {clip.x+1,clip.y+1, range, clip.h};
  if (m_showString)
  {
    TextContainer::paint(progress);
  }
  else
  {
    nds::Canvas::instance().fillRectangle(progress.x, progress.y,
        progress.w, progress.h, textArea()->backgroundColor());
  }
  m_dirty = false;
}

