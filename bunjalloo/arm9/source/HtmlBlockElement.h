#ifndef HtmlBlockElement_h_seen
#define HtmlBlockElement_h_seen

class HtmlBlockElement: public HtmlElement
{

  public:
    HtmlBlockElement(const std::string & tagName) 
      : HtmlElement(tagName) 
    {
      m_block = true;
    }
  
};
#endif
