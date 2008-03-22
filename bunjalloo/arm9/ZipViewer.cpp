#include "Button.h"
#include "Language.h"
#include "RichTextArea.h"
#include "ZipFile.h"
#include "ZipViewer.h"
#include "PatchDLDI.h"

using std::vector;
using std::string;

static const string ndsExt(".nds");

ZipViewer::ZipViewer( const std::string & filename):
  m_filename(filename),
  m_unzip(0),
  m_unzipAndPatch(0)
{
}

void ZipViewer::show(RichTextArea & textArea)
{
  ZipFile file;
  file.open(m_filename.c_str());

  vector<string> contents;
  file.list(contents);
  if (not contents.empty())
  {
    // add an "unzip" button!
    Button * unzipButton = new Button(T("unzip"));
    unzipButton->setListener(this);
    textArea.add(unzipButton);
    m_unzip = unzipButton;

    Button * patch = new Button(T("unzip_patch"));
    patch->setListener(this);
    textArea.add(patch);
    m_unzipAndPatch = patch;
    textArea.insertNewline();
  }

  for (vector<string>::const_iterator it(contents.begin()); it != contents.end(); ++it)
  {
    const UnicodeString & u(string2unicode(*it));
    textArea.appendText(u);
    textArea.insertNewline();
  }
}

void ZipViewer::unzip()
{
  ZipFile file;
  // unzip the file
  file.open(m_filename.c_str());
  if (file.is_open())
  {
    file.extract();
  }
}

void ZipViewer::unzipAndPatch()
{
  unzip();
  ZipFile file;
  file.open(m_filename.c_str());
  if (file.is_open())
  {
    vector<string> contents;
    file.list(contents);
    for (vector<string>::const_iterator it(contents.begin()); it!= contents.end(); ++it)
    {
      string lowName(*it);
      transform(lowName.begin(), lowName.end(), lowName.begin(), ::tolower);
      if (lowName.rfind(ndsExt) == (lowName.length() - ndsExt.length()))
      {
        const string & name(*it);
        nds::PatchDLDI dldi(name.c_str());
        dldi.patch();
      }
    }
  }
}

void ZipViewer::pressed(ButtonI * button)
{
  if (button == m_unzip)
  {
    // unzip the file
    unzip();
  }
  else if (button == m_unzipAndPatch)
  {
    unzipAndPatch();
  }
}
