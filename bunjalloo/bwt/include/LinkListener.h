#ifndef LinkListener_h_seen
#define LinkListener_h_seen

class Link;
class LinkListener
{
  public:
    virtual ~LinkListener() {}

    virtual void linkClicked(Link * link) = 0;

};
#endif
