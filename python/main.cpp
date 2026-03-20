#include "python/module_def.h"

ModuleComponentRegister* ModuleComponentRegister::tail = nullptr;

PYBIND11_MODULE(PyReplayParser, m) {
  for(ModuleComponentRegister * start = ModuleComponentRegister::tail; start; start = start->next) {
    start->cb(m);
  }
}