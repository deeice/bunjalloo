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
#include <algorithm>
#include "Component.h"
#include "ButtonI.h"
#include "ButtonGroup.h"

void ButtonGroup::add(ButtonI * button)
{
  std::vector<ButtonI*>::const_iterator it = std::find(m_group.begin(), m_group.end(), button);
  if (it == m_group.end())
  {
    m_group.push_back(button);
    button->setListener(this);
  }
}

void ButtonGroup::remove(ButtonI * button)
{
  std::vector<ButtonI*>::iterator it = std::find(m_group.begin(), m_group.end(), button);
  if (it != m_group.end())
  {
    m_group.erase(it);
    button->removeListener(this);
  }
}


void ButtonGroup::pressed(ButtonI * pressed)
{
  std::vector<ButtonI*>::iterator it(m_group.begin());
  for (; it != m_group.end(); ++it)
  {
    ButtonI * b(*it);
    if (b != pressed)
    {
      b->setSelected(false);
    }
    else
    {
      b->setSelected(true);
    }
  }
}
