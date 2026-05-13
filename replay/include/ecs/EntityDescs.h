#ifndef MYEXTENSION_ENTITYDESCS_H
#define MYEXTENSION_ENTITYDESCS_H
#include "ecs/typesAndLimits.h"
#include <vector>
#include "ecs/entityId.h"

namespace ecs
{
  static inline entity_id_t make_eid(uint32_t index, uint32_t gen) { return index | (gen << ENTITY_INDEX_BITS); }
  static constexpr uint32_t RESERVED_EID_RANGE = std::numeric_limits<uint16_t>::max();
  static constexpr uint32_t globalGen = 1; // my globalGen is never going to change as an EntityManager is destroyed upon session destruction
  struct EntityDesc
  {
    template_t templ_id = INVALID_TEMPLATE_INDEX;
    archetype_t archetype_id = INVALID_ARCHETYPE;
    chunk_index_t chunk_id = INVALID_CHUNK_INDEX_T;
    uint32_t generation = globalGen;
  };
  // a class that represents
  class EntityDescs
  {
  protected:
    friend EntityManager;
    friend GState;


    std::vector<EntityDesc> entDescs;

    EntityDescs()
    {
      entDescs.resize(RESERVED_EID_RANGE); // holds space for 100 entities
      EntityDesc desc{1, 1, 1}; // reserves first slot, an invalid entity id is 0
      entDescs[0] = desc;
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

    [[nodiscard]] inline bool doesEntityExistInternal(unsigned idx) const
    {
      return idx < entDescs.size() && entDescs[idx].templ_id != INVALID_TEMPLATE_INDEX;
    }

    [[nodiscard]] inline bool doesEntityExist(unsigned idx, uint32_t generation) const
    {
      return idx < entDescs.size() && entDescs[idx].generation == generation;
    }
    [[nodiscard]] bool doesEntityExist(EntityId e) const { return doesEntityExist(e.index(), e.get_generation()); }


    EntityDesc &operator[](uint32_t i) { return entDescs[i]; }
    const EntityDesc &operator[](uint32_t i) const { return entDescs[i]; }

    size_t size() const {return entDescs.size();}

    uint32_t push_back() {
      auto idx = this->entDescs.size();
      entDescs.push_back({});
      return (uint32_t)idx;
    }

    EntityId makeEid(uint32_t idx) { return EntityId(make_eid(idx, idx < entDescs.size() ? entDescs[idx].generation : globalGen)); }
  };



}

#endif //MYEXTENSION_ENTITYDESCS_H
