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
    virtual const Config & config() const = 0;
    virtual ~ControllerI() {}


};
#endif
