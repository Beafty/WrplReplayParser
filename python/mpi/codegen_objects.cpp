#include "modules/codegenObjects.h"
#include "modules/mpi.h"
#include "mpi/codegen/ReflIncludes.h"

void PyCodeGenObjects::include(py::module_ &m) {
  DO_INCLUDE()
  py_mpi.include(m);
  auto mpi = m.def_submodule("mpi");
  py::class_<danet::Uid>(mpi, "Uid")
      .def_readonly("player_uid", &danet::Uid::player_id)
      .def_readonly("name", &danet::Uid::name);
  py::class_<MPlayer, danet::ReflectableObject, std::unique_ptr<MPlayer, py::nodelete>>(mpi, "MPlayer")
      .def_property_readonly("uid", [](MPlayer &self){return &self.uid.data;});
}

PyCodeGenObjects py_CodeGen;