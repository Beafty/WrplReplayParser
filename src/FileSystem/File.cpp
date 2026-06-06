
#include "FileSystem.h"
#include "ioSys/dag_dataBlock.h"
#include "ioSys/dag_memIo.h"

std::span<char> HostFile::readRaw()
{
    if (file_stream.is_open()) {
        auto old_offs = file_stream.tellg();
        file_stream.seekg(0);
        this->buffer.resize(this->length());
        file_stream.read((char *) this->buffer.data(), this->length());
        file_stream.seekg(old_offs);
        return this->buffer;
    }
    return {};
}

void HostFile::Save(std::ofstream *cb)
{
    auto temp = readRaw();
    cb->write(temp.data(), temp.size());
}

int64_t HostFile::read_impl(void *ptr, size_t length) {
    if (!file_stream.is_open())
        return -1;
    if (read_offs >= f_length)
        return 0;
    file_stream.seekg(read_offs);
    file_stream.read((char *) ptr, length);
    int64_t rd = file_stream.gcount();
    if (rd > 0)
        read_offs += rd;
    return rd;
}

bool HostFile::loadBlk(DataBlock &blk) {
  auto data = this->readRaw();
  InPlaceMemLoadCB rdr(data.data(), (int) data.size());
  if (blk.loadFromStream(rdr, nullptr))
      return true;
  return false;
}
