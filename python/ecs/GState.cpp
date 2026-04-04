#include "modules/ecs/GState.h"
#include "ecs/EntityManager.h"

PyGState py_global_state;

void PyGState::include(py::module_ &m) {
  DO_INCLUDE()
  auto ecs = m.def_submodule("ecs");

}