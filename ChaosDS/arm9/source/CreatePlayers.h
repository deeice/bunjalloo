#ifndef CreatePlayers_h_seen
#define CreatePlayers_h_seen

#include "TouchScreen.h"
class CreatePlayers: public TouchScreen
{
  public:
    /** @param start flag to say if the screen is the first, or if it should
     * only redraw already existing wizard details. If true, then it will
     * create a new set of wizards.
     */
    CreatePlayers(bool start=true);

    virtual void show();
    virtual void animate();
    virtual void handleKeys();
    
  private:
    int m_hilightItem;
    int m_hilightWizard;
    bool m_start;

    static void incrPlayerCb(void * arg);
    static void decrPlayerCb(void * arg);
    static void changeIconCb(void * arg);
    static void changeIconColCb(void * arg);
    static void changeNameCb(void * arg);
    static void changeLevelCb(void * arg);
    static void resetPlayersCb(void * arg);
    static void startCb(void * arg);

    void selectWizFromY();

    void selectItem(int);
    void deselectItem();
    void updatePlayers(void);

    void up();
    void down();
    void left();
    void right();
    void a();
    void l();
    void r();
    void start();
    
};
#endif
