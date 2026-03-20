#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include "python/module_def.h"


class Compressor {
public:
  Compressor() = default;

  [[nodiscard]] py::bytes compress(const py::buffer& b) const {
    py::buffer_info info = b.request();
    if (info.ndim != 1)
      throw std::runtime_error("Input must be a 1D buffer (bytes, bytearray, etc)");
    const char* src = static_cast<const char*>(info.ptr);
    std::vector<uint8_t> payload{};
    payload.resize(LZ4_compressBound((int)info.size)+1);
    int ret = LZ4_compress_HC(src, reinterpret_cast<char *>(payload.data()), (int)info.size, (int)payload.size(), LZ4HC_CLEVEL_DEFAULT);
    if(ret == 0)
      throw std::runtime_error("LZ4_compress_HC returned 0");
    return {reinterpret_cast<const char*>(payload.data()), ret};
  }

  [[nodiscard]] py::bytes decompress(const py::buffer& b, uint64_t decomp_size) const {
    py::buffer_info info = b.request();
    if (info.ndim != 1)
      throw std::runtime_error("Input must be a 1D buffer (bytes, bytearray, etc)");
    const char* src = static_cast<const char*>(info.ptr);
    std::vector<uint8_t> payload{};
    payload.resize(decomp_size);
    int ret = LZ4_decompress_safe(src, reinterpret_cast<char *>(payload.data()), (int)info.size, (int)payload.size());
    if(ret < 0) {
      std::ostringstream strm{};
      strm << "LZ4_compress_HC returned ";
      strm << ret;
      std::cout.flush();
      throw std::runtime_error(strm.str());
    }
    return {reinterpret_cast<const char*>(payload.data()), ret};
  }
};

PYBIND11_MODULE(lz4pyBindings, m) {

  py::class_<Compressor>(m, "LZ4")
      .def(py::init<>())
      .def("compress", &Compressor::compress)
      .def("decompress", &Compressor::decompress);
}