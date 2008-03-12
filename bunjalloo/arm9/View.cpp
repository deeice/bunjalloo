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
#include <libgen.h>
#include "ndspp.h"
#include "libnds.h"
#include "BrowseToolbar.h"
#include "BookmarkToolbar.h"
#include "Canvas.h"
#include "Config.h"
#include "Controller.h"
#include "Document.h"
#include "HtmlDocument.h"
#include "File.h"
#include "FormControl.h"
#include "Language.h"
#include "InternalVisitor.h"
#include "HtmlElement.h"
#include "Keyboard.h"
#include "Link.h"
#include "LinkHandler.h"
#include "PreferencesToolbar.h"
#include "ProgressBar.h"
#include "ScrollPane.h"
#include "SearchEntry.h"
#include "Stylus.h"
#include "TextField.h"
#include "RichTextArea.h"
#include "URI.h"
#include "View.h"
#include "ViewRender.h"
#include "WidgetColors.h"

using namespace std;
const static char * ENTER_URL_TITLE("enter_url");
const static char * SAVE_AS_TITLE("save_as");
const static char * ENTER_TEXT_TITLE("enter_text");
const static int STEP(1);
const static char * BOOKMARK_FILE  = "/"DATADIR"/user/bookmarks.html";
const static char * SEARCH_TEMPLATE = "/"DATADIR"/docs/search-example.txt";

struct KeyState
{
  KeyState() { }

  void initialise( u16 repeat, u16 down, u16 held, u16 up)
  {
    m_repeat = repeat;
    m_down = down;
    m_held = held;
    m_up = up;
  }

  inline int isRepeat(int mask) const
  {
    return (m_repeat & mask);
  }
  inline int isHeld(int mask) const
  {
    return m_held & mask;
  }
  inline int isDown(int mask) const
  {
    return m_down & mask;
  }
  inline int isUp(int mask) const
  {
    return m_up & mask;
  }
  private:
  u16 m_repeat;
  u16 m_down;
  u16 m_held;
  u16 m_up;
};

View::View(Document & doc, Controller & c):
  m_document(doc),
  m_controller(c),
  m_keyboard(new Keyboard),
  m_renderer(new ViewRender(this)),
  m_addressBar(new TextField(UnicodeString())),
  m_browseToolbar(new BrowseToolbar(*this)),
  m_bookmarkToolbar(new BookmarkToolbar(*this)),
  m_prefsToolbar( new PreferencesToolbar(*this)),
  m_toolbar(m_browseToolbar),
  m_progress(new ProgressBar(0, 100)),
  m_scrollPane(new ScrollPane),
  m_state(BROWSE),
  m_form(0),
  m_linkHandler(new LinkHandler(this)),
  m_search(0),
  m_keyState(new KeyState),
  m_dirty(true),
  m_refreshing(0)
{
  m_scrollPane->setTopLevel();
  m_scrollPane->setLocation(0, 0);
  m_scrollPane->setSize(nds::Canvas::instance().width(), nds::Canvas::instance().height());
  m_scrollPane->setScrollIncrement(20);
  m_keyboard->setTopLevel(m_scrollPane);
  m_keyboard->setTitle(T(ENTER_TEXT_TITLE));
  m_document.registerView(this);
  keysSetRepeat( 10, 5 );
  m_toolbar->setVisible(true);
  string searchFile;
  if (m_controller.config().resource(Config::SEARCHFILE_STR,searchFile))
  {
    if (nds::File::exists(searchFile.c_str()) == nds::File::F_NONE)
    {
      // copy the template file there instead
      Config::copyTemplate(SEARCH_TEMPLATE, searchFile.c_str());
    }
    m_search = new SearchEntry(searchFile);
  }
  m_progress->setShowString();
  m_progress->setSize(250, 40);
  m_progress->setLocation(2, 2);
}

View::~View()
{
  delete m_keyboard;
  delete m_renderer;
  delete m_addressBar;
  delete m_browseToolbar;
  delete m_bookmarkToolbar;
  delete m_linkHandler;
  delete m_search;
  Stylus::deleteInstance();
}

void View::extractTitle()
{
  // use title text..
  // extract the title from the document
  // only allow bookmarking of html? no, allow any old crap - firefox does.
  // so, extract either the title, or the basename (remove cgi stuff?)
  const HtmlElement * title = m_document.titleNode();
  if (title)
  {
    const UnicodeString & titleText = title->firstChild()->text();
    m_bookmarkTitleUtf8 = unicode2string(titleText, true);
  }
  else
  {
    URI tmpUri(m_document.uri());
    m_bookmarkTitleUtf8 = tmpUri.fileName();
    m_bookmarkTitleUtf8 = nds::File::base(m_bookmarkTitleUtf8.c_str());
  }
}

void View::notify()
{
  Document::Status status(m_document.status());

  switch (status) {
    case Document::REDIRECTED:
        m_filenameForProgress.clear();
        break;
    case Document::LOADED:
      {
        m_filenameForProgress.clear();
        /** Broken by BWT.
         m_textArea->setStartLine( (-SCREEN_HEIGHT / m_textArea->font().height()) - 1);
        swiWaitForVBlank();
         */
        m_renderer->render();
        int pos = m_document.position();
        if (pos == -1)
        {
          // is it relative?
          URI uri(m_document.uri());
          const string & internal(uri.internalLink());
          if (not internal.empty())
          {
            // do some stuff with internal links
            InternalVisitor visitor(internal);
            HtmlElement * root((HtmlElement*)m_document.rootNode());
            root->accept(visitor);
            if (visitor.found())
            {
              RichTextArea * text((RichTextArea*)m_scrollPane->childAt(m_scrollPane->childCount()-1));
              unsigned int linkPos = text->linkPosition(visitor.index()) + 192;
              pos = (linkPos * 256) / (text->bounds().h);
            }

          }
        }
        m_scrollPane->scrollToPercent(pos);
        m_dirty = true;
        string refresh;
        int refreshTime;
        m_document.refresh(refresh, refreshTime);
        if (not refresh.empty() and refreshTime >= 0)
        {
          m_refreshing = (refreshTime+1) * 30;
        }
        else
        {
          m_refreshing = 0;
        }
      }
      break;
    case Document::INPROGRESS:
      {
        // add a progress bar or something here...
        unsigned int pc = m_document.percentLoaded();
        m_progress->setValue(pc);
        if (m_filenameForProgress.empty())
        {
          URI u(m_document.uri());
          m_filenameForProgress = nds::File::base(u.fileName().c_str());
          if (m_filenameForProgress.empty())
          {
            m_filenameForProgress = "index";
          }
        }
        char buffer[10];
        string s(m_filenameForProgress);
        sprintf_platform(buffer, " %d%%", pc);
        s += buffer;
        m_progress->setText(string2unicode(s));
      }
      break;
    case Document::HAS_HEADERS:
      {
        switch (m_document.htmlDocument()->mimeType())
        {
          case HtmlParser::OTHER:
            saveAs();
            m_state = SAVE_DOWNLOADING;
            break;

          case HtmlParser::UNINITIALISED:
          default:
            // can see it
            break;
        }
      }
    default:
      break;
  }
}

void View::enterUrl()
{
  m_addressBar->setText(string2unicode(m_document.uri()));
  m_keyboard->setTitle(T(ENTER_URL_TITLE));
  m_keyboard->editText(m_addressBar);
  m_toolbar->setVisible(false);
  m_state = ENTER_URL;
  m_dirty = true;
}

void View::setToolbar(Toolbar * toolbar)
{
  m_toolbar->setVisible(false);
  m_toolbar = toolbar;
  m_toolbar->setVisible(true);
}

void View::endBookmark()
{
  m_state = BROWSE;
  m_document.clearConfigHistory();
  m_document.setHistoryEnabled(false);
  m_controller.clearReferer();
  if (m_linkHref.empty())
    m_linkHref = m_document.uri();
  m_document.setHistoryEnabled(true);

  setToolbar(m_browseToolbar);
}

void View::bookmarkUrl()
{
  // Add a line to the file DATADIR/userdata/bookmarks.html
  setToolbar(m_bookmarkToolbar);
  m_state = BOOKMARK;
  // extract the *current* title
  extractTitle();
  showBookmarkPage();
}

void View::showBookmarkPage()
{
  m_document.setHistoryEnabled(false);
  if (nds::File::exists(BOOKMARK_FILE) == nds::File::F_NONE)
  {
    // create it
    nds::File bookmarks;
    // doesn't exist, so write out the header...
    bookmarks.open(BOOKMARK_FILE, "w");
    if (not bookmarks.is_open()) {
      // that's unpossible!
      return;
    }
    const static string header("<META HTTP-EQUIV='Content-Type' CONTENT='text/html; charset=UTF-8'><TITLE>Bookmarks</TITLE>\n");
    bookmarks.write(header.c_str(), header.length());
  }
  string bookmarkUrl("file://");
  bookmarkUrl += BOOKMARK_FILE;
  m_controller.doUri(bookmarkUrl);
  m_document.setHistoryEnabled(true);
}

void View::editConfig()
{
  // show a config page with values for the configuration :-/
  URI configFile("config://index.html");
  m_controller.doUri(configFile);
}

void View::bookmarkCurrentPage()
{
  {
    nds::File bookmarks;
    // OK - add to the bm file
    bookmarks.open(BOOKMARK_FILE, "a");
    if (bookmarks.is_open())
    {
      // write out "<a href=%1>%2</a>" 1=href 2=title
      string href("<a href='");
      bookmarks.write(href.c_str(), href.length());
      const string & uri(m_document.uri());
      bookmarks.write(uri.c_str(), uri.length());
      href = "'>";
      bookmarks.write(href.c_str(), href.length());
      // output titleText to bookmarks
      bookmarks.write(m_bookmarkTitleUtf8.c_str(), m_bookmarkTitleUtf8.length());
      href = "</a><br>\n";
      bookmarks.write(href.c_str(), href.length());
    }
  }
  showBookmarkPage();
}

void View::preferences()
{
  // clicked preferences button.
  // This should allow:
  // 1) Homepage (?) issues 30, 38
  // 2) Cookie file edit, issue 36
  // 3) Add current server to cookie file (+edit?), issue 36
  // 4) shortcut file edition (Add field?)

  // Solution: new toolbar! PreferencesToolbar.cpp
  // back
  // edit cookie file
  // add cookie + edit
  // add shortcut + edit
  setToolbar(m_prefsToolbar);
}

void View::saveAs()
{
  // save file as ???
  URI uri(m_document.uri());
  string fileName(nds::File::base(uri.fileName().c_str()));
  if (fileName.empty())
  {
    fileName = "index.html";
  }
  m_addressBar->setText(string2unicode(fileName));
  m_keyboard->setTitle(T(SAVE_AS_TITLE));
  m_keyboard->editText(m_addressBar);
  m_toolbar->setVisible(false);
  m_state = SAVE_CURRENT_FILE;
  m_dirty = true;
}

void View::updateInput()
{
  m_keyState->initialise(
      keysDownRepeat(),
      keysDown(),
      keysHeld(),
      keysUp());
  touchPosition tp = touchReadXY();
  Stylus::TouchType touchType = Stylus::keysToTouchType( m_keyState->isHeld(KEY_TOUCH), m_keyState->isUp(KEY_TOUCH));
  Stylus::instance()->update(touchType, m_keyState->isRepeat(KEY_TOUCH),
      tp.px, tp.py+SCREEN_HEIGHT);
}

void View::browse()
{
  updateInput();

  if (m_keyState->isRepeat(KEY_START)) {
    enterUrl();
  }
  if (not m_keyboard->visible())
  {
    if (m_keyState->isRepeat(KEY_SELECT)) {
      m_toolbar->cyclePosition();
    }
    if (m_keyState->isRepeat(KEY_DOWN)) {
      // scroll down ...
      m_scrollPane->down();
      m_dirty = true;
    }
    if (m_keyState->isRepeat(KEY_UP)) {
      // scroll up ...
      m_scrollPane->up();
      m_dirty = true;
    }
    if (m_keyState->isRepeat(KEY_RIGHT)) {
      // scroll down ...
      m_scrollPane->pageDown();
      m_dirty = true;
    }
    if (m_keyState->isRepeat(KEY_LEFT)) {
      // scroll up ...
      m_scrollPane->pageUp();
      m_dirty = true;
    }
    if (m_keyState->isRepeat(KEY_L)) {
      m_controller.previous();
    }
    if (m_keyState->isRepeat(KEY_R)) {
      m_controller.next();
    }
  }

  if (m_keyState->isRepeat(KEY_A)) {
    // render the node tree
    m_document.dumpDOM();
  }

  // change to keys actually down, not repeating
  Stylus * stylus(Stylus::instance());
  if (stylus->touchType() != Stylus::NOTHING)
  {
    m_dirty = true;
    m_dirty = m_keyboard->dirty();
    if (not m_dirty) {
      m_dirty = m_scrollPane->dirty();
      if (m_dirty)
      {
        m_document.setPosition( m_scrollPane->currentPosition());
      }
    }
    m_toolbar->setVisible(!m_keyboard->visible());
  }
  // else --- add drag gestures, etc..
  if (m_scrollPane->visible() and m_dirty)
  {
    m_document.setPosition( m_scrollPane->currentPosition());
  }

  if (m_refreshing > 0)
  {
    m_refreshing--;
    if (m_refreshing == 0)
    {
      int tmp;
      m_document.refresh(m_linkHref, tmp);
    }
  }
}

void View::pressed(ButtonI * button)
{
  if (m_form)
    return;

  FormControl * formControl = (FormControl*)button;
  m_form = formControl;
}

void View::linkClicked(Link * link)
{
  // there are types of link
  // 1) Anchor only
  // 2) Image only
  // 3) Image and anchor

  bool isAnchor = link->eventType() == Link::STATE_LINK;
  bool isImage  = link->color() == WidgetColors::LINK_IMAGE;

  m_controller.setReferer(m_document.uri());
  if (isAnchor and not isImage)
  {
    m_linkHref = link->href();
  }
  else if (isImage and not isAnchor)
  {
    // image link?
    m_linkHref = link->src();
  }
  else // isAnchor and isImage
  {
    Stylus * stylus(Stylus::instance());
    m_linkHandler->setLink(link);
    m_linkHandler->setLocation(stylus->lastX(), stylus->lastY());
    m_linkHandler->setVisible();
  }
}

void View::keyboard()
{
  updateInput();
  m_dirty = m_keyboard->dirty();
  if (not m_dirty)
    m_dirty = m_scrollPane->dirty();
}

void View::tick()
{
  scanKeys();
  switch (m_state)
  {
    case BROWSE:
      browse();
      break;
    case ENTER_URL:
    case SAVE_CURRENT_FILE:
    case SAVE_DOWNLOADING:
      keyboard();
      break;
    case BOOKMARK:
      // have a page of bookmarks or what?
      // need to be able to
      // 1) jump to bookmark
      // 2) edit bookmark
      // 3) add current page easily
      // 4) add any page
      // for now cheap solution: have an Add button and change the Toolbar state.
      browse();
      break;
  }
  m_dirty |= m_keyboard->tick();
  m_dirty |= (m_document.status() == Document::INPROGRESS) and m_progress->dirty();
  m_toolbar->tick();


  if (m_dirty) {
    const static nds::Rectangle clip = {0, 0, nds::Canvas::instance().width(), nds::Canvas::instance().height()};
    m_scrollPane->paint(clip);
    m_keyboard->paint(clip);
    m_linkHandler->paint(clip);
    if (m_progress->dirty() and m_document.status() == Document::INPROGRESS)
    {
      m_progress->paint(m_progress->bounds());
    }
    nds::Canvas::instance().endPaint();
    m_dirty = false;
    m_toolbar->updateIcons();
  }

  if (m_state != BROWSE and not m_keyboard->visible())
  {
    m_keyboard->setTitle(T(ENTER_TEXT_TITLE));
  }
  if (m_state == ENTER_URL and not m_keyboard->visible()) {
    m_toolbar->setVisible(true);
    doEnterUrl();
  }

  if (( m_state == SAVE_CURRENT_FILE or m_state == SAVE_DOWNLOADING )
    and not m_keyboard->visible()) {
    m_toolbar->setVisible(true);
    doSaveAs();
  }

  // clicked a link:
  if (not m_linkHref.empty()) {
    URI uri(m_document.uri());
    if (m_state == BOOKMARK)
    {
      endBookmark();
      uri = m_linkHref;
    }
    string tmp(m_linkHref);
    m_linkHref = "";
    // cout << "Navigated to " << m_linkHref << endl;
    // TODO - "navigate or download"..
    m_controller.doUri( uri.navigateTo(tmp).asString() );
  }

  if (m_form)
  {
    URI uri(m_document.uri());
    FormControl * tmp = m_form;
    m_form = 0;

    tmp->input(m_controller, uri);
    m_state = BROWSE;
  }
}

void View::doEnterUrl()
{
  m_state = BROWSE;
  string newAddress = unicode2string(m_keyboard->result());
  if (not newAddress.empty() and m_keyboard->selected() == Keyboard::OK)
  {
    // check for search
    string result;
    if (m_search and m_search->checkSearch(newAddress, result))
    {
      newAddress = result;
    }
    m_toolbar->setVisible(true);
    m_controller.clearReferer();
    m_controller.doUri(newAddress);
  }
}

void View::doSaveAs()
{
  string fileName = unicode2string(m_keyboard->result());
  if (not fileName.empty() and m_keyboard->selected() == Keyboard::OK)
  {
    m_toolbar->setVisible(true);
    m_controller.saveAs(fileName.c_str(),
        m_state==SAVE_DOWNLOADING?Controller::SAVE_DOWNLOADING:Controller::SAVE_CURRENT_FILE);
  }
  else
  {
    // this does nothing on already loaded pages.
    // cancels save on download in progress stuff
    m_controller.stop();
  }
  m_state = BROWSE;
}
