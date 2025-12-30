#include "network/Connection.h"

#ifndef MYEXTENSION_OBJECT_H
#define MYEXTENSION_OBJECT_H

namespace net
{
  class Object {
  public:
    explicit Object(ecs::EntityId eid_);
    static Object *getByEid(ecs::EntityId, ecs::EntityManager*);
    bool deserializeComps(const BitStream &bs, Connection *conn);
  protected:
    ecs::EntityId eid;
  };


}
ECS_DECLARE_BASE_TYPE(net::Object, "net::Object", true);
#endif //MYEXTENSION_OBJECT_H
