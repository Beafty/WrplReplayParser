

#ifndef MYEXTENSION_REPLAY_H
#define MYEXTENSION_REPLAY_H

#include "reader.h"
#include <filesystem>
#include "DataBlock.h"
#include "Replay/ReplayReader.h"

namespace fs = std::filesystem;

struct ReplayData {
  uint8_t *data;
  std::size_t size;

  ReplayData() {
    data = nullptr;
    size = 0;
  }

  template<typename T>
  ReplayData(T *data, size_t size) {
    assert(sizeof(T) == 1);
    this->data = data;
    this->size = size;
  }

  ReplayData(IGenReader &rdr) {
    this->size = rdr.getSize() - rdr.readOffset();
    this->data = (unsigned char *) malloc(this->size);
    rdr.read(data, (int) size);
  }

  ReplayData(std::span<uint8_t> data) {
    this->size = data.size();
    auto t_ptr = (uint8_t *) malloc(this->size);
    memcpy(t_ptr, data.data(), data.size());
    this->data = t_ptr;
  }

  [[nodiscard]] std::span<uint8_t> getData() const {
    return {(unsigned char *) data, size};
  }

  [[nodiscard]] std::span<uint8_t> getData(uint64_t offset) const {
    G_ASSERT(data);
    return {(uint8_t *) data + offset, size - offset};
  }

  [[nodiscard]] std::string_view getStr(uint64_t offset) const {
    return {((char *) data + offset)};
  }

  template<typename T>
  [[nodiscard]] T *getObj(uint64_t offset) {
    return reinterpret_cast<T *>(data + offset);
  }

  ~ReplayData() {
    if (data)
      free(data);
    data = nullptr;
    size = 0;
  }
};


class Replay {
#define MAGIC_OFFS 0x00000003
#define FOOTER_BLK_OFFSET_LOC 0x000002AC // where the integer that stores the footer blk offset is stored
#define LEVEL_BIN_PATH_OFFS 0x00000008 // where xxx.bin starts
#define MISSION_BLK_OFFS 0x00000088
#define MAIN_DATA_START 0x000004D2 // where the 'replay' starts, can either be the header blk or the zlib data
#define PLAYER_COUNT_OFFS 0x000002D8
#define SESSION_ID_OFFS 0x000002DC

  fs::path replay_path;
  ReplayData *data;
  uint32_t footerBlkOffset;
  uint32_t zlib_start = MAIN_DATA_START;
  uint32_t zlib_size = 0;
  bool valid = false;
public:
  uint32_t magic;
  const uint32_t current_magic = 25951248;
  std::string_view level_bin; // 128 bytes
  std::string_view level_blk; //260 bytes
  DataBlock HeaderBlk;
  DataBlock FooterBlk;
  int PlayerCount;
  uint64_t session_id;


  Replay(const std::string &path) {
    replay_path = path;
    FileReader rdr(path);
    if (!rdr.isValid()) {
      EXCEPTION("Replay given invalid file path: {}", path.data());
    }
    data = new ReplayData(rdr);
    parse();
  }

  Replay(std::span<uint8_t> data) {
    replay_path = "";
    this->data = new ReplayData(data);
    parse();
  }


  ReplayReader *getRplReader() {
    auto dat = data->getData(zlib_start);
    auto *rdr = new BaseReader(reinterpret_cast<char *>(dat.data()), zlib_size, false);
    auto *payload = new ReplayReader(new ZlibLoadCB(*rdr, zlib_size), rdr);
    return payload;
  }

  ReplayReader *getStreamReader(uint32_t time_wait = 10) {
    auto *rdr = new FileStreamReader(this->replay_path.string(), time_wait);
    rdr->seekto(zlib_start);
    auto *payload = new ReplayReader(new ZlibLoadCB(*rdr, 0xFFFFFFF, false, false), rdr);
    return payload;
  }

  FullDecompressReplayReader *getFullDecompressReplayReader() {
    ZoneScoped;
    auto dat = data->getData(zlib_start);
    auto *rdr = new FullDecompressReplayReader(std::span(dat.data(), zlib_size));
    return rdr;
  }

  ~Replay() {
    delete data;
  }

  bool check_magic() {
    return this->magic == this->current_magic;
  }

private:
  void parse() {
    magic = *data->getObj<uint32_t>(MAGIC_OFFS);
    valid = this->check_magic();
    if(!valid)
      return;
    level_bin = data->getStr(LEVEL_BIN_PATH_OFFS);
    level_blk = data->getStr(MISSION_BLK_OFFS);
    PlayerCount = *data->getObj<int>(PLAYER_COUNT_OFFS);
    footerBlkOffset = *data->getObj<uint32_t>(FOOTER_BLK_OFFSET_LOC);
    session_id = *data->getObj<uint64_t>(SESSION_ID_OFFS);
    if (*data->getObj<uint8_t>(MAIN_DATA_START) == 1) // the BLK always starts with 0x01, zlib 0x78
    {
      auto span = data->getData(MAIN_DATA_START);
      BaseReader rdr(reinterpret_cast<char *>(span.data()), (int) span.size(), false);
      HeaderBlk.loadFromStream(rdr, nullptr, nullptr);
      zlib_start += rdr.readOffset();
    }
    if (footerBlkOffset) {
      zlib_size = footerBlkOffset - zlib_start;
      auto span = data->getData(footerBlkOffset);
      BaseReader rdr(reinterpret_cast<char *>(span.data()), (int) span.size(), false);
      FooterBlk.loadFromStream(rdr, nullptr, nullptr);
    } else {
      zlib_size = (uint32_t) data->size - zlib_start;
    }
  }

};

void readFilesFromDirectory(const fs::path &dirPath, std::vector<fs::path> &fileSet);

std::string file_exists(std::string path, const std::vector<fs::path> &paths);

class ServerReplay {

  void init_from_path(fs::path &path) {
    std::vector<fs::path> files;
    readFilesFromDirectory(path, files);
    std::vector<std::string> repl_paths;
    if (auto p = file_exists("0000.wrpl", files); !p.empty()) {
      repl_paths.emplace_back(p);
    }
    for (int i = 1; i > 0; i += 2) {
      if (auto p = file_exists(fmt::format("{:0>4}.wrpl", i), files); !p.empty()) {
        repl_paths.emplace_back(p);
      } else {
        break;
      }
    }
    if (!repl_paths.empty()) {
      replay_files.reserve(repl_paths.size());
      for (auto &path_: repl_paths) {
        replay_files.emplace_back(path_);
      }
    }
  }
public:
  std::vector<Replay> replay_files;
  ServerReplay(fs::path &path) {
    init_from_path(path);
  }

  ServerReplay(const std::string &path) {
    fs::path t_path{path};
    init_from_path(t_path);
  }

  ServerReplay(std::span<std::span<uint8_t>> data) {
    replay_files.reserve(data.size());
    for(auto & spn : data) {
      replay_files.emplace_back(spn);
    }
  }
  ServerReplayReader *getRplReader() {
    return new ServerReplayReader(this->replay_files);
  }

};


#endif //MYEXTENSION_REPLAY_H
