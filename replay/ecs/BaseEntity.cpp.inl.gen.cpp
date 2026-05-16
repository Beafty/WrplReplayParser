// Built with ECS codegen version 1.0
    #include <ecs/query/entitySystem.h>
    #include <ecs/componentTypes.h>
    #include <ecs/ComponentTypesDefs.h>
    #include "BaseEntity.cpp.inl"
ECS_DEF_PULL_VAR(BaseEntity);
static constexpr ecs::ComponentDesc unit_tank_create_es_comps[] =
{
//start of 1 rw components at [0]
  {ECS_HASH("unit__ref"), ecs::ComponentTypeInfo<unit::UnitRef>()},
//start of 2 ro components at [1]
  {ECS_HASH("unit_storage__tank"), ecs::ComponentTypeInfo<HeavyVehicleModelStorageComponent>()},
  {ECS_HASH("uid"), ecs::ComponentTypeInfo<int>()}
};
static void unit_tank_create_es_all_events(ecs::EntityManager *mgr, const ecs::Event &__restrict evt, const ecs::QueryView &__restrict components)
{
  G_FAST_ASSERT(evt.is<ecs::EventEntityCreated>());
  auto comp = components.begin(), compE = components.end(); G_ASSERT(comp!=compE); do
    unit_tank_create_es(static_cast<const ecs::EventEntityCreated&>(evt)
        , ECS_RO_COMP(unit_tank_create_es_comps, "unit_storage__tank", HeavyVehicleModelStorageComponent)
    , ECS_RO_COMP(unit_tank_create_es_comps, "uid", int)
    , ECS_RW_COMP(unit_tank_create_es_comps, "unit__ref", unit::UnitRef)
    , mgr
    );
  while (++comp != compE);
}
static ecs::EntitySystemDesc unit_tank_create_es_es_desc
(
  "unit_tank_create_es",
  "D:\ReplayParser\replay\ecs\BaseEntity.cpp.inl",
  ecs::EntitySystemOps(unit_tank_create_es_all_events),
  ecs::make_span(unit_tank_create_es_comps+0, 1)/*rw*/,
  ecs::make_span(unit_tank_create_es_comps+1, 2)/*ro*/,
  ecs::empty_span(),
  ecs::empty_span(),
  ecs::EventSetBuilder<ecs::EventEntityCreated>::build(),
  0
);
static constexpr ecs::ComponentDesc unit_aircraft_create_es_comps[] =
{
//start of 1 rw components at [0]
  {ECS_HASH("unit__ref"), ecs::ComponentTypeInfo<unit::UnitRef>()},
//start of 2 ro components at [1]
  {ECS_HASH("unit_storage__aircraft"), ecs::ComponentTypeInfo<FlightModelWrapStorageComponent>()},
  {ECS_HASH("uid"), ecs::ComponentTypeInfo<int>()}
};
static void unit_aircraft_create_es_all_events(ecs::EntityManager *mgr, const ecs::Event &__restrict evt, const ecs::QueryView &__restrict components)
{
  G_FAST_ASSERT(evt.is<ecs::EventEntityCreated>());
  auto comp = components.begin(), compE = components.end(); G_ASSERT(comp!=compE); do
    unit_aircraft_create_es(static_cast<const ecs::EventEntityCreated&>(evt)
        , ECS_RO_COMP(unit_aircraft_create_es_comps, "unit_storage__aircraft", FlightModelWrapStorageComponent)
    , ECS_RO_COMP(unit_aircraft_create_es_comps, "uid", int)
    , ECS_RW_COMP(unit_aircraft_create_es_comps, "unit__ref", unit::UnitRef)
    , mgr
    );
  while (++comp != compE);
}
static ecs::EntitySystemDesc unit_aircraft_create_es_es_desc
(
  "unit_aircraft_create_es",
  "D:\ReplayParser\replay\ecs\BaseEntity.cpp.inl",
  ecs::EntitySystemOps(unit_aircraft_create_es_all_events),
  ecs::make_span(unit_aircraft_create_es_comps+0, 1)/*rw*/,
  ecs::make_span(unit_aircraft_create_es_comps+1, 2)/*ro*/,
  ecs::empty_span(),
  ecs::empty_span(),
  ecs::EventSetBuilder<ecs::EventEntityCreated>::build(),
  0
);
