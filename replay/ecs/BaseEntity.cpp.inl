#include "ecs/ComponentTypesDefs.h"
#include "Unit.h"
#include "ecs/query/coreEvents.h"


static void
unit_tank_create_es(const ecs::EventEntityCreated &evt, const HeavyVehicleModelStorageComponent &unit_storage__tank, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager *manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Tank(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__tank);
}

static void
unit_aircraft_create_es(const ecs::EventEntityCreated &evt, const FlightModelWrapStorageComponent &unit_storage__aircraft, const int &uid, unit::UnitRef &unit__ref, ecs::EntityManager *manager) {
  G_ASSERT(unit__ref.unit == nullptr);
  unit__ref.unit = new unit::Aircraft(static_cast<uint16_t>(uid));
  unit::LoadFromStorage(unit__ref.unit, unit_storage__aircraft);
}

