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
#include "Config.h"
#include "Cache.h"
#include "Controller.h"
#include "Document.h"
#include "File.h"
#include "HtmlDocument.h"
#include "HtmlElement.h"
#include "ParameterSet.h"
#include "RichTextArea.h"
#include "Updater.h"
#include "URI.h"
#include "View.h"
#include "ZipViewer.h"
#include "Language.h"

using std::string;
using std::vector;

Updater::Updater(Controller & c,
    Document & d, View & view): m_controller(c), m_document(d), m_view(view)
{
}

void Updater::init()
{
  string update;
  m_controller.config().resource(Config::UPDATE, update);
  m_document.setHistoryEnabled(false);
  m_controller.doUri(update);
  m_document.setHistoryEnabled(true);
}

void Updater::show(RichTextArea & textArea)
{
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
          string version, download, size;
          tokenize(data, lines, "\n");
          for (vector<string>::const_iterator it(lines.begin()); it != lines.end(); ++it)
          {
            ParameterSet set(*it);
            if (set.hasParameter("version"))
            {
              set.parameter("version", version);
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
          string update;
          m_controller.config().resource(Config::UPDATE, update);
          const URI & downloadUrl(URI(update).navigateTo(download));
          m_document.setHistoryEnabled(false);
          m_view.setSaveAsEnabled(false);
          m_controller.doUri(downloadUrl);
          m_view.setSaveAsEnabled(true);
          // now find out where that was saved...
          string cachedFile = m_controller.cache()->fileName(downloadUrl);
          if (not m_controller.stopped() and nds::File::exists(cachedFile.c_str()) == nds::File::F_REG)
          {
            ZipViewer viewer(cachedFile);
            viewer.unzipAndPatch();
          }
          m_document.setHistoryEnabled(true);
          textArea.appendText(T("updated"));
          textArea.appendText(string2unicode(version));
        }
      }
    }
  }
}

void Updater::pressed(ButtonI * button)
{
}
