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
#ifndef View_h_seen
#define View_h_seen

#include "ViewI.h"
#include "ButtonListener.h"
#include "LinkListener.h"

class Document;
class ControllerI;

class FormControl;
class Keyboard;
class ScrollPane;
class TextField;
class Toolbar;
class ViewRender;

/** Handle the displaying of HTML data.*/
class View : public ViewI, public ButtonListener, public LinkListener
{
  friend class ViewRender;
  public:
    /** Create a View for the given document and controller.
     * @param doc the model.
     * @param controller the controller.
     */
    View(Document & doc, ControllerI & controller);

    ~View();

    void notify();

    /** Show the view, handle key events for one frame.
     */
    void tick();

    /** Implement the ButtonListener interface.
     * @param button the button that has been pressed.
     */
    virtual void pressed(ButtonI * button);

    /** Implement the LinkListener interface.
     * @param link the link that has been clicked.
     */
    virtual void linkClicked(Link * link);

    void enterUrl();

  private:

    enum InputState
    {
      BROWSE,
      KEYBOARD
    };

    Document & m_document;
    ControllerI & m_controller;
    ScrollPane * m_scrollPane;
    Keyboard * m_keyboard;
    ViewRender * m_renderer;
    TextField * m_addressBar;
    Toolbar * m_toolbar;
    InputState m_state;
    FormControl * m_form;
    int m_preInputStartLine;
    bool m_dirty;
    std::string m_linkHref;

    void browse();
    void keyboard();

};
#endif
