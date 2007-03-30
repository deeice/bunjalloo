#ifndef HardwareHandler_h_seen
#define HardwareHandler_h_seen
#include <vector>

namespace nds
{
  template <class T>
  class HardwareHandler
  {
    public:
      //! Create a background implementation.
      static void add(T * p );
      //! Destroy a background implementation.
      static void remove(T * );

      //! Render the background objects.
      static void render();

      static bool compare(const T * x, const T * y);
      
    private:
      //! Keep track of the objects created.
      typedef typename std::vector<T*> VectorT;
      typedef typename std::vector<T*>::iterator VectorT_iterator;
      VectorT m_vector;

      static HardwareHandler& instance();
      
      HardwareHandler() {};
      ~HardwareHandler() {};
      
  };
};

#endif
