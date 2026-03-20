#include "modules/replay/replay.h"
#include "modules/codegenObjects.h"
#include "modules/State.h"
PYBIND11_MODULE(PyReplayParser, m) {
  py_replay.include(m);
  py_replay_state.include(m);
}