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
#ifndef ControllerI_h_seen
#define ControllerI_h_seen

#include <string>
class Config;
class URI;
class ControllerI
{
  public:
    virtual void doUri(const URI & uri) = 0;
    virtual void previous() = 0;
    virtual void next() = 0;
    virtual void stop() = 0;
    virtual const Config & config() const = 0;
    virtual bool wifiInitialised() const = 0;
    virtual ~ControllerI() {}


};
#endif
