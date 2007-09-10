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
#ifndef Controller_h_seen
#define Controller_h_seen

class Document;
class View;
class Config;
class URI;

#include "ControllerI.h"

class HttpClient;
/** Controller maps user actions to model updates. For example, responds to setting the URI.
 */
class Controller : public ControllerI
{
  friend class HttpClient;
  public:
    //!Constructor.
    Controller();
    //! Destructor
    ~Controller();

    /** Handle the user request for setting a URI.
     * @param uri the URI to use.
     */
    virtual void doUri(const URI & uri);
    
    virtual void previous();
    virtual void next();
    virtual void stop();

    virtual const Config & config() const;

    /** Show the software licence.*/
    void showLicence();

    /** Loops forever.*/
    void mainLoop();

    virtual bool wifiInitialised() const;

  private:
    Document * m_document;
    View * m_view;
    Config * m_config;
    bool m_wifiInit;
    bool m_stop;


    void localFile(const std::string &);
    void fetchHttp(const URI &);
    void configureUrl(const std::string & fileName);
    void loadError();
    // helper to avoid code dupe
    void handleUri(const URI & uri);

};
#endif
