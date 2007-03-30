#include <algorithm>
#include "HardwareHandler.h"

using namespace nds;
using namespace std;

//! Add a renderable object.
template <class T> void HardwareHandler<T>::add(T * p)
{
  HardwareHandler::instance().m_vector.push_back(p);
};

//! remove the object.
template <class T> void HardwareHandler<T>::remove(T * p)
{
  VectorT & vectorT = HardwareHandler<T>::instance().m_vector;
  VectorT_iterator iter = find(vectorT.begin(), vectorT.end() , p);
  if (iter != vectorT.end())
  {
    HardwareHandler<T>::instance().m_vector.erase(iter);
  }
}

//! Render the background objects.
template <class T> void HardwareHandler<T>::render()
{
  VectorT & vectorT = HardwareHandler::instance().m_vector;
  sort(vectorT.begin(), vectorT.end(), HardwareHandler::compare);
  for_each(vectorT.begin(), vectorT.end(), mem_fun(&T::render));
}

template <class T> HardwareHandler<T>& HardwareHandler<T>::instance()
{
  static HardwareHandler<T> s_handler;
  return s_handler;
}

template <class T> bool HardwareHandler<T>::compare(const T * x, const T * y)
{
  if ( *x < *y )
    return 1;
  if ( *x > *y )
    return -1;
  return 0;
}
