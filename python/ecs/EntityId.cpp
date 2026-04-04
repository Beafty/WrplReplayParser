#include <math/dag_Point2.h>
#include "modules/ecs/EntityId.h"
#include "ecs/entityId.h"
PyEntityId py_entity_id;

void PyEntityId::include(py::module_ &m) {
  DO_INCLUDE()
  auto ecs = m.def_submodule("ecs");
  py::class_<ecs::EntityId>(ecs, "EntityId")
      .def("get_handle", &ecs::EntityId::get_handle)
      .def("index", &ecs::EntityId::index)
      .def("get_generation", &ecs::EntityId::get_generation)
      .def("__bool__", &ecs::EntityId::operator bool);
}