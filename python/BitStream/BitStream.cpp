#include "danet/BitStream.h"
#include "modules/BitStream.h"

using ssize_t = Py_ssize_t;
PyBitStream py_bitstream{};

void PyBitStream::include(py::module_ &m) {
  DO_INCLUDE()
  py::class_<BitStream>(m, "BitStream")
      .def("readBytes", [](BitStream &bs, uint32_t byte_count) -> std::optional<py::bytes> {
        if((byte_count<<3) > bs.GetNumberOfUnreadBits())
          return {};
        std::string payload{};
        payload.resize(byte_count);
        if(bs.ReadBits((uint8_t*)payload.data(), byte_count<<3)) {
          return {payload};
        }
        return {};
      })
      .def("GetData", [](BitStream &bs) {
        return py::memoryview::from_memory(
            bs.GetData(),
            (ssize_t)bs.GetNumberOfBytesUsed(),
            true
        );
      });
}