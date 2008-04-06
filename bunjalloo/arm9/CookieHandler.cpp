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
#include "ButtonGroup.h"
#include "Button.h"
#include "CookieHandler.h"
#include "CookieJar.h"
#include "Document.h"
#include "Language.h"
#include "RadioButton.h"
#include "RichTextArea.h"
#include "URI.h"
#include "View.h"
#include "ViewRender.h"

CookieHandler::CookieHandler(View * view): m_view(*view),
  m_group(0),
  m_siteButton(0),
  m_allButton(0),
  m_ok(0),
  m_cancel(0)
{
  init();
}

void CookieHandler::init()
{
  // only the group button doesn't get eaten by textArea
  delete m_group;

  m_group = new ButtonGroup;
  m_siteButton = new RadioButton;
  m_allButton = new RadioButton;
  m_ok = new Button(T("ok"));
  m_cancel = new Button(T("cancel"));

  m_group->add(m_siteButton);
  m_group->add(m_allButton);
  m_ok->setListener(this);
  m_cancel->setListener(this);
}

void CookieHandler::show()
{
  init();
  m_view.renderer()->doTitle(T("add_ck_title"));
  m_siteButton->setSelected();
  URI uri(m_view.document().uri());
  RichTextArea & textArea(*m_view.renderer()->textArea());
  switch (uri.protocol())
  {
    case URI::HTTPS_PROTOCOL:
    case URI::HTTP_PROTOCOL:
      textArea.appendText(T("add_cookie"));
      textArea.insertNewline();
      textArea.add(m_siteButton);
      textArea.appendText(string2unicode(uri.server()));
      textArea.insertNewline();
      textArea.add(m_allButton);
      textArea.appendText(string2unicode(CookieJar::topLevel(uri.server())));
      textArea.insertNewline();
      textArea.add(static_cast<Button*>(m_ok));
      textArea.add(static_cast<Button*>(m_cancel));
      break;
    default:
      break;
  }
  m_view.resetScroller();
}

void CookieHandler::pressed(ButtonI * button)
{
  if (button == m_ok)
  {
    URI uri(m_view.document().uri());
    std::string domain(uri.server());
    if (m_allButton->selected())
    {
      domain = CookieJar::topLevel(domain);
    }
    m_view.document().cookieJar()->setAcceptCookies(domain);
    m_view.endBookmark();
  }
  else if (button == m_cancel)
  {
    m_view.endBookmark();
  }
}

