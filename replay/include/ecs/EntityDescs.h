#ifndef MYEXTENSION_ENTITYDESCS_H
#define MYEXTENSION_ENTITYDESCS_H
#include "ecs/typesAndLimits.h"
#include <vector>
#include "ecs/entityId.h"

namespace ecs
{
  struct EntityDesc
  {
    EntityId eid = INVALID_ENTITY_ID;
    template_t templ_id = INVALID_TEMPLATE_INDEX;
    archetype_t archetype_id = INVALID_ARCHETYPE;
    chunk_index_t chunk_id = INVALID_CHUNK_INDEX_T;
  };
  // a class that represents
  class EntityDescs
  {
  protected:
    friend EntityManager;

    std::vector<EntityDesc> entDescs;
    unsigned nextFreeSlot=1;

    EntityDescs()
    {
      entDescs.reserve(100); // holds space for 100 entities
      EntityDesc desc{ecs::EntityId(0), 1, 1, 1}; // reserves first slot, an invalid entity id is 0
      entDescs.push_back(desc);
    }
    inline const EntityDesc *getEntityDesc(EntityId eid) const;
    inline EntityDesc *getEntityDesc(EntityId eid);
    bool getEntityTemplateId(EntityId eid, template_t &tmpl) const;
    bool getEntityArchetypeId(EntityId eid, archetype_t &archetype) const;
    bool getEntityChunkId(EntityId eid, chunk_index_t &chunk) const;

    /// ensures that eid has space to exist
    inline void Allocate(EntityId eid)
    {
      auto idx = eid.index();
      if (idx >= entDescs.size())
        entDescs.resize(idx+1);
    }

    inline EntityId GetNextEid()
    {
      auto payload = EntityId(this->nextFreeSlot);
      this->Allocate(payload);
      unsigned new_idx = payload.index()+1;
      for(; new_idx <= this->entDescs.size(); new_idx++)
      {
        if(!doesEntityExist(new_idx))
          break;
      }
      nextFreeSlot = new_idx;
      return payload;
    }


    [[nodiscard]] inline bool doesEntityExist(unsigned idx) const
    {
      return idx < entDescs.size() && entDescs[idx].templ_id != INVALID_TEMPLATE_INDEX;
    }
    [[nodiscard]] bool doesEntityExist(EntityId e) const { return doesEntityExist(e.index()); }


    EntityDesc &operator[](uint32_t i) { return entDescs[i]; }
    const EntityDesc &operator[](uint32_t i) const { return entDescs[i]; }
  };



}

#endif //MYEXTENSION_ENTITYDESCS_H
