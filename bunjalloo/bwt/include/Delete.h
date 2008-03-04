#ifndef Delete_h_seen
#define Delete_h_seen
/** Functor that helps delete STL containers.*/
struct delete_ptr {
  template<class T> T* operator()(T* p) { delete p; return 0; }
};
#endif
