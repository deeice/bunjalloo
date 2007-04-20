#ifndef ViewI_h_seen
#define ViewI_h_seen

class ViewI
{

  public:
    /** Notify the view for updates.*/
    virtual void notify() = 0;

    virtual ~ViewI() {}

};
#endif
