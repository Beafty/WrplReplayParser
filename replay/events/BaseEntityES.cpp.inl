#include "state/ParserState.h"
#include "ecs/ComponentTypesDefs.h"
#include "Unit.h"
#include "ecs/query/coreEvents.h"
#include "ecs/ecsCodegen.h"



static void
on_tank_appear_es(const ecs::EventEntityCreated &evt, const HeavyVehicleModelStorageComponent &unit_storage__tank, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager &manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Tank(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__tank);
}

static void
on_aircraft_appear_es(const ecs::EventEntityCreated &evt, const FlightModelWrapStorageComponent &unit_storage__aircraft, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager &manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Aircraft(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__aircraft);
}

static void
on_unit_disappear_es(const ecs::EventEntityDestroyed &evt, unit::UnitRef &unit__ref) {
  if(unit__ref.unit)
    delete unit__ref.unit;
}


ECS_AFTER(on_aircraft_appear_es, on_tank_appear_es)
static void uid_entity_es(const ecs::EventEntityCreated &evt, const ecs::EntityId eid, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager &manager) {
  if (uid == -1) {
    LOGE("Entity {:#x} has no uid, normal?", eid.get_handle());
    return;
  }
  ENTITY_LOGD3("Entity {:#x} requested uid {}", eid.get_handle(), uid);
  G_ASSERT(manager.uid_lookup[uid] == ecs::INVALID_ENTITY_ID);
  manager.uid_lookup[uid] = eid;
  if(unit__ref.unit)
    manager.uid_unit_lookup[uid] = unit__ref.unit;
}

ECS_AFTER(on_aircraft_appear_es, on_tank_appear_es)
static void uid_entity_es(const ecs::EventEntityDestroyedBasic &evt, const int &uid, ecs::EntityManager &manager) {
  G_ASSERT(manager.uid_lookup[uid] != ecs::INVALID_ENTITY_ID);
  manager.uid_lookup[uid] = ecs::INVALID_ENTITY_ID;
  manager.uid_unit_lookup[uid] = nullptr;
}

template <typename Callable>
ECS_REQUIRE(ecs::Tag playerUnit)
static void iterate_all_units_ecs_query(ecs::EntityManager &manager, Callable c);

void iterate_all_units(ParserState &state) {
  iterate_all_units_ecs_query(state.g_entity_mgr, [](const unit::UnitRef &unit__ref, const ecs::string &unit__className){
    if(unit__ref.unit) {
      auto &unit = *unit__ref.unit;
      //log_ext("", 0, DEFAULT_SINK_HANDLER, LOGLEVEL::INFO, fmt::format("{}, {}", unit__ref.unit->uid, unit__className));
      uint32_t time_ms = unit.positions.empty() ? 0 : unit.positions[unit.positions.size()-1].time_ms;
      LOGI("{} {} owned by {} last path at {}({})", unit.uid, unit__className, unit.owner_pid, ((float)time_ms)/1000.f, unit.positions.size());
    } else {
      LOGI("unk unit {}", unit__className);
    }
    return ecs::QueryCbResult::Continue;
  });
}

