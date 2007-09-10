#ifndef Toolbar_h_seen
#define Toolbar_h_seen

#include <vector>
#include "Sprite.h"

class Document;
class ControllerI;
class View;

class Toolbar: public ViewI
{
  public:
    Toolbar(Document & doc, ControllerI & cont, View & view);
    ~Toolbar();

    bool visible() const;
    void setVisible(bool visible=true);

    void tick();

    bool touch(int x, int y);

    void updateIcons();

    virtual void notify();

  private:
    bool m_visible;
    typedef std::vector<nds::Sprite * > SpriteVector;
    SpriteVector m_sprites;
    Document & m_document;
    ControllerI & m_controller;
    View & m_view;

    int m_timerReset;
    int m_timer;

    void handlePress(int i);

};
#endif
