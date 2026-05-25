#include "modules/mpi/types.h"
#include "modules/mpi/codegen_types.h"
#include "modules/DataBlock/DataBlock.h"
#include "modules/ecs/EntityId.h"
#include "modules/mpi/unit.h"
#include "mpi/types.h"
#include "modules/bind_readonly_vector.h"
#include "Unit.h"

PyMpiTypes py_mpi_types;

void PyMpiTypes::include(py::module_ &m) {
    DO_INCLUDE()
    py_data_block.include(m);
    py_entity_id.include(m);

    auto mpi = m.def_submodule("mpi");
    py::class_<danet::Uid>(mpi, "Uid")
            .def_readonly("player_id", &danet::Uid::player_id)
            .def_property_readonly("player_name", &danet::Uid::get_player_name)
            .def_property_readonly("as_bytes", [](danet::Uid &self) {
                    std::string payload{};
                    payload.resize(sizeof(danet::Uid));
                    memcpy(&payload[0], &self, sizeof(danet::Uid));
                    return payload;
            });
    py::class_<danet::UnitId>(mpi, "UnitId")
            .def_readonly("val", &danet::UnitId::val);
    py::class_<std::unordered_set<ecs::EntityId> >(m, "EntityIdSet")
            .def("__contains__", [](const std::unordered_set<int> &self, int val) { return self.count(val); })
            .def("__len__", [](const std::unordered_set<int> &self) { return self.size(); })
            .def("__iter__", [](const std::unordered_set<int> &self) {
                return py::make_iterator(self.begin(), self.end());
            }, py::keep_alive<0, 1>());
    bind_readonly_vector_no_contain<std::vector<unit::Unit> >(mpi, "UnitList");
    py::enum_<danet::AreaFlagsEnum>(mpi, "AreaFlagsEnum")
            .value("air", danet::AreaFlagsEnum::air)
            .value("ground", danet::AreaFlagsEnum::ground)
            .value("mapArea", danet::AreaFlagsEnum::mapArea)
            .value("team1", danet::AreaFlagsEnum::team1)
            .value("team2", danet::AreaFlagsEnum::team2)
            .value("killArea", danet::AreaFlagsEnum::killArea)
            .value("detectionArea", danet::AreaFlagsEnum::detectionArea)
            .value("airMapArea", danet::AreaFlagsEnum::airMapArea);


    py_codegen_types.include(m);
}
