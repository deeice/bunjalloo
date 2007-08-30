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
