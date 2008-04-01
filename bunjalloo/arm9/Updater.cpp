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
#include <string>
#include <vector>
#include "Button.h"
#include "Cache.h"
#include "Config.h"
#include "Controller.h"
#include "Document.h"
#include "File.h"
#include "HeaderParser.h"
#include "HtmlDocument.h"
#include "HtmlElement.h"
#include "Language.h"
#include "ParameterSet.h"
#include "RichTextArea.h"
#include "Updater.h"
#include "Version.h"
#include "View.h"
#include "ViewRender.h"
#include "ZipViewer.h"

using std::string;
using std::vector;
extern const char * VERSION;

Updater::Updater(Controller & c,
    Document & d, View & view): m_controller(c), m_document(d), m_view(view), m_state(START)
{
}

void Updater::init()
{
  string update;
  // set state before calling get uri
  m_state = GOT_INI;
  m_controller.config().resource(Config::UPDATE, update);
  m_document.setHistoryEnabled(false);
  m_controller.doUri(update);
  m_document.setHistoryEnabled(true);
}

void Updater::show()
{
  switch (m_state)
  {
    case START:
      init();
      break;
    case GOT_INI:
      getZip();
      break;
    case GOT_ZIP:
      askUpdate();
      break;
    case DO_UPDATE:
      doUpdate();
      break;
    case CANCELLED:
      m_view.endBookmark();
      break;
    default:
      break;
  }
}

void Updater::getZip()
{
  m_state = INI_FAIL;
  if (m_document.status() == Document::LOADED
      and not m_controller.stopped()
      and m_document.htmlDocument()->mimeType() == HtmlParser::TEXT_PLAIN)
  {
    // loaded. check it is what we expect
    const HtmlElement * rootNode = m_document.rootNode();
    if (rootNode->hasChildren())
    {
      const HtmlElement * body(rootNode->lastChild());
      if (body and body->hasChildren())
      {
        const HtmlElement * text(body->firstChild());
        if (text->isa("#TEXT"))
        {
          // yipee
          const string & data = unicode2string(text->text(), true);
          vector<string> lines;
          string download, size;
          tokenize(data, lines, "\n");
          for (vector<string>::const_iterator it(lines.begin()); it != lines.end(); ++it)
          {
            ParameterSet set(*it);
            if (set.hasParameter("version"))
            {
              set.parameter("version", m_newVersion);
            }
            if (set.hasParameter("URL"))
            {
              set.parameter("URL", download);
            }
            if (set.hasParameter("size"))
            {
              set.parameter("size", size);
            }
          }
          Version current(VERSION);
          Version online(m_newVersion.c_str());
          m_state = GOT_ZIP;
          if (online > current)
          {
            string update;
            m_controller.config().resource(Config::UPDATE, update);
            m_downloadUrl = URI(update).navigateTo(download);
            m_document.setHistoryEnabled(false);
            m_view.setSaveAsEnabled(false);
            m_controller.doUri(m_downloadUrl);
            m_view.setSaveAsEnabled(true);
            m_document.setHistoryEnabled(true);
          }
          else
          {
            alreadyGotLatest();
          }
        }
      }
    }
  }
  if (m_state == INI_FAIL)
  {
    iniFail();
  }
}

void Updater::iniFail()
{
  doTitle();
  RichTextArea & textArea(*(m_view.renderer()->textArea()));
  textArea.appendText(T("fail_ini"));
  textArea.insertNewline();
  addCancel(textArea);
}

void Updater::doUpdate()
{
  RichTextArea & textArea(*(m_view.renderer()->textArea()));
  string cachedFile = m_controller.cache()->fileName(m_downloadUrl);
  ZipViewer viewer(m_view);
  viewer.setFilename(cachedFile);
  viewer.unzipAndPatch();
  textArea.appendText(T("done"));
  m_view.resetScroller();
}

extern const char * VERSION;
void Updater::askUpdate()
{
  doTitle();
  RichTextArea & textArea(*(m_view.renderer()->textArea()));
  // now find out where that was saved...
  string cachedFile = m_controller.cache()->fileName(m_downloadUrl);
  if (not m_controller.stopped()
      and nds::File::exists(cachedFile.c_str()) == nds::File::F_REG
      and m_document.headerParser().httpStatusCode() == 200)
  {
    textArea.appendText(T("update_ask"));
    textArea.insertNewline();
    textArea.appendText(T("new_ver"));
    textArea.appendText(string2unicode(m_newVersion));
    textArea.insertNewline();
    textArea.appendText(T("cur_ver"));
    textArea.appendText(string2unicode(VERSION));
    textArea.insertNewline();
    textArea.insertNewline();
    addOk(textArea);
    addCancel(textArea);
    textArea.insertNewline();
    m_view.resetScroller();
  }
  else
  {
    textArea.appendText(T("fail_zip"));
    textArea.appendText(string2unicode(m_newVersion));
    textArea.insertNewline();
    addCancel(textArea);
    m_state = ZIP_FAIL;
  }
}

void Updater::addOk(RichTextArea & textArea)
{
  m_ok = new Button(T("ok"));
  m_ok->setListener(this);
  textArea.add(m_ok);
}

void Updater::addCancel(RichTextArea & textArea)
{
  m_cancel = new Button(T("cancel"));
  m_cancel->setListener(this);
  textArea.add(m_cancel);
}

void Updater::pressed(ButtonI * button)
{
  if (m_state == GOT_ZIP)
  {
    if (button == (ButtonI*)m_ok)
    {
      m_state = DO_UPDATE;
    }
    else
    {
      m_state = CANCELLED;
    }
    show();
  }
  else
  {
    m_view.endBookmark();
  }
}

void Updater::doTitle()
{
  m_view.renderer()->doTitle(T("update_title"));
}

void Updater::alreadyGotLatest()
{
  doTitle();
  RichTextArea & textArea(*(m_view.renderer()->textArea()));
  textArea.appendText(T("alrdy_upd"));
  textArea.insertNewline();
  textArea.appendText(string2unicode(m_newVersion));
  textArea.insertNewline();
  textArea.insertNewline();
  addCancel(textArea);
}
