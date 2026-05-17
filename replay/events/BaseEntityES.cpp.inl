#include "ecs/EntityManager.h"
#include "ecs/ComponentTypesDefs.h"
#include "Unit.h"
#include "ecs/query/coreEvents.h"
#include "ecs/ecsCodegen.h"



static void
on_tank_appear_es(const ecs::EventEntityCreated &evt, const HeavyVehicleModelStorageComponent &unit_storage__tank, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager *manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Tank(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__tank);
}

static void
on_aircraft_appear_es(const ecs::EventEntityCreated &evt, const FlightModelWrapStorageComponent &unit_storage__aircraft, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager *manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Aircraft(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__aircraft);
}


ECS_AFTER(on_aircraft_appear_es, on_tank_appear_es)
static void uid_entity_es(const ecs::EventEntityCreated &evt, const ecs::EntityId eid, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager *manager) {
  if (uid == -1) {
    LOGE("Entity {:#x} has no uid, normal?", eid.get_handle());
    return;
  }
  ENTITY_LOGD3("Entity {:#x} requested uid {}", eid.get_handle(), uid);
  G_ASSERT(manager->uid_lookup[uid] == ecs::INVALID_ENTITY_ID);
  manager->uid_lookup[uid] = eid;
  if(unit__ref.unit)
    manager->uid_unit_lookup[uid] = unit__ref.unit;
}


ECS_AFTER(on_aircraft_appear_es, on_tank_appear_es)
static void uid_entity_es(const ecs::EventEntityDestroyedBasic &evt, const int &uid, ecs::EntityManager *manager) {
  G_ASSERT(manager->uid_lookup[uid] != ecs::INVALID_ENTITY_ID);
  manager->uid_lookup[uid] = ecs::INVALID_ENTITY_ID;
  manager->uid_unit_lookup[uid] = nullptr;
  //LOG("removing entity at uid {}", *uid);
}