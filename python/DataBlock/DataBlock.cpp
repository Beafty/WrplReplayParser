#include "modules/DataBlock/DataBlock.h"
#include "DataBlock.h"

void PyDataBlock::include(py::module_ &m) {
  DO_INCLUDE()

  py::class_<DataBlock>(m, "DataBlock")
      .def(py::init<>())
      .def("toString", [](const DataBlock& self){
        std::ostringstream oss{};
        self.printBlock(0, oss);
        return oss.str();
      });
}

PyDataBlock py_data_block{};
