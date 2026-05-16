

#ifndef WTFILEUTILS_COREEVENTS_H
#define WTFILEUTILS_COREEVENTS_H

#include "event.h"

namespace ecs {
  ECS_UNICAST_EVENT_TYPE(EventEntityCreated)
  // does more permanent stuff like actual UnitRef.unit destruction
  ECS_UNICAST_EVENT_TYPE(EventEntityDestroyed)
  // basic destruction, only does minimal, like remove uid lookup entry, also only called
  ECS_UNICAST_EVENT_TYPE(EventEntityDestroyedBasic)
  // dummy event used for a query
  ECS_UNICAST_EVENT_TYPE(EventQuery)
}


#endif //WTFILEUTILS_COREEVENTS_H
