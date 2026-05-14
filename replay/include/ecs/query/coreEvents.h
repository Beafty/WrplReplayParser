

#ifndef WTFILEUTILS_COREEVENTS_H
#define WTFILEUTILS_COREEVENTS_H

namespace ecs {
  ECS_UNICAST_EVENT_TYPE(EventEntityCreated)
  // does more permanent stuff like actual UnitRef.unit destruction
  ECS_UNICAST_EVENT_TYPE(EventEntityDestroyed)
  // basic destruction, only does minimal, like remove uid lookup entry, also only called
  ECS_UNICAST_EVENT_TYPE(EventEntityDestroyedBasic)
}


#endif //WTFILEUTILS_COREEVENTS_H
