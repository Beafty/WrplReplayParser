#include "replay/Replay.h"
#include "Replay/ReplayReader.h"
#include "libdeflate.h"

uint32_t getPacketSize(IGenReader &cb) {
  uint8_t first_byte;
  if(!cb.readInto(first_byte))
    return 0;
  if(first_byte & 0x80)
  {
    return first_byte & 0x7f;
  }
  else
  {
    uint8_t byte_count = 1;
    if((first_byte & 0x40) == 0)
    {
      byte_count = 2;
      if ((first_byte & 0x20) == 0)
      {
        byte_count = 3;
        if ((first_byte & 0x10) == 0)
        {
          byte_count = 4;
        }
      }
    }
    uint8_t payload[4];
    if(!cb.tryRead(&payload, byte_count))
    {
      return 0;
    }

    if((first_byte & 0x40) == 0)
    {
      if ((first_byte & 0x20) == 0)
      {
        if ((first_byte & 0x10) == 0)
        {
          return payload[0] + (payload[1] << 8) + (payload[2] << 16) + (payload[3] << 24);
        }
        else
        {
          EXCEPTION("reached unkown point");
        }
      }
      else
      {
        return (payload[1] + (first_byte << 0x10) + (payload[0] << 0x8)) ^ 0x200000;
      }
    }
    else
    {
      return ((first_byte << 8) + payload[0]) ^ 0x4000;
    }
  }
}

void readFilesFromDirectory(const fs::path& dirPath, std::vector<fs::path>& fileSet) {
  if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
    return;
  }

  for (const auto& entry : fs::directory_iterator(dirPath)) {
    if (fs::is_regular_file(entry.status())) {
      fileSet.emplace_back(fs::absolute(entry.path()));
    }
  }
}


std::string file_exists(std::string path, const std::vector<fs::path>& paths)
{
  for(auto &path_ : paths)
  {
    if(path_.filename().string() == path)
      return path_.string();
  }
  return {};
}

ServerReplayReader::ServerReplayReader(std::vector<Replay> &rdrs) {
  ZoneScoped;
  this->readers.reserve(rdrs.size());
  for(auto &rpl : rdrs)
  {
    this->readers.push_back(rpl.getFullDecompressReplayReader());
  }
}

ServerReplayReader::~ServerReplayReader() {
  for(auto &rpl : this->readers)
  {
    delete rpl;
  }
}

bool ServerReplayReader::getNextPacket(ReplayPacket *packet) {
  if(this->index >= this->readers.size())
    return false;
  auto curr_rpl = this->readers[this->index];
  if(curr_rpl->getNextPacket(packet))
    return true;
  this->index++;
  return this->getNextPacket(packet);
}


IReplayReader::~IReplayReader() = default;

FullDecompressReplayReader::FullDecompressReplayReader(std::span<uint8_t> zlib_data)  {
  ZoneScoped;
  auto ptr = (uint8_t*)malloc(zlib_data.size()*3);
  size_t dest_len;
  auto ctx = libdeflate_alloc_decompressor();
  libdeflate_result ret;
  {
    ZoneScopedN("Replay uncompress")
    ret = libdeflate_zlib_decompress(ctx, zlib_data.data(), zlib_data.size(), ptr, zlib_data.size()*3, &dest_len);
    //ret = uncompress(ptr, reinterpret_cast<unsigned long *>(&dest_len), zlib_data.data(), zlib_data.size());
  }
  G_ASSERT(ret == LIBDEFLATE_SUCCESS);
  //G_ASSERT(ret == Z_OK);
  libdeflate_free_decompressor(ctx);
  crd = new BaseReader(reinterpret_cast<char *>(ptr), dest_len, true);
}
