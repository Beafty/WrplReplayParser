#include "modules/mpi/types.h"
#include "modules/mpi/codegen_types.h"
#include "modules/DataBlock/DataBlock.h"
#include "modules/ecs/EntityId.h"
#include "mpi/types.h"


PyMpiTypes py_mpi_types;
void PyMpiTypes::include(py::module_ &m) {
  DO_INCLUDE()
  py_data_block.include(m);
  py_entity_id.include(m);
  auto mpi = m.def_submodule("mpi");
  py::class_<danet::Uid>(mpi, "Uid")
      .def_readonly("player_id", &danet::Uid::player_id)
      .def_property_readonly("player_name", &danet::Uid::get_player_name);
  py::class_<danet::UnitId>(mpi, "UnitId")
      .def_readonly("val", &danet::UnitId::val);
  py::class_<std::unordered_set<ecs::EntityId>>(m, "EntityIdSet")
      .def("__contains__", [](const std::unordered_set<int> &self, int val) { return self.count(val); })
      .def("__len__", [](const std::unordered_set<int> &self) { return self.size(); })
      .def("__iter__", [](const std::unordered_set<int> &self) {
        return py::make_iterator(self.begin(), self.end());
      }, py::keep_alive<0, 1>());
  py_codegen_types.include(m);
}