#ifndef ZipViewer_h_seen
#define ZipViewer_h_seen

#include <string>
#include "ButtonListener.h"

class RichTextArea;

class ZipViewer: public ButtonListener
{
  public:
    ZipViewer(const std::string & filename);

    void unzip();
    void unzipAndPatch();
    void show(RichTextArea & textArea);
    virtual void pressed(ButtonI * button);
  private:
    std::string m_filename;
    ButtonI * m_unzip;
    ButtonI * m_unzipAndPatch;
};
#endif
