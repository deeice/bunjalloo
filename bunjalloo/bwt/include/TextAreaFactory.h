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
#ifndef TextAreaFactory_h_seen
#define TextAreaFactory_h_seen

class TextArea;
/** A factory that generates TextArea instances. */
class TextAreaFactory
{

  public:
    enum TextType
    {
      TXT_NORMAL,
      TXT_EDIT,
      TXT_RICH
    };
    /** The user is responsible for calling delete on each TextArea created.
     * @return a new TextArea.
     */
    static TextArea * create(TextType type=TXT_NORMAL);

  private:
    TextAreaFactory();
    ~TextAreaFactory();
    TextAreaFactory(const TextAreaFactory &);
    const TextAreaFactory operator=(const TextAreaFactory &);

};
#endif
