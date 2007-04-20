#ifndef Entity_h_seen
#define Entity_h_seen

#include <string>

/** An HTML entity, for use with "&NNN;" type values.*/
struct Entity {
  //! The name of the entity.
  const char * const name;
  //! The equivalent Unicode character value.
  const unsigned int value;
};

//! Total number of entities.
#define ENTITY_COUNT 259
//! A static array of Entity values.
extern const Entity s_entity[ENTITY_COUNT];

#endif
