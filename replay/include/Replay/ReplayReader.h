#pragma once
#include "ReplayStructs.h"


uint32_t getPacketSize(IGenReader &cb);
class IReplay;
class Replay;
class ServerReplay;

class IReplayReader
{
protected:
  IReplay * owner;
  IReplayReader(Replay &owner);
  IReplayReader(ServerReplay &owner);
public:
  virtual ~IReplayReader(){}
  virtual bool getNextPacket(ReplayPacket &packet) = 0;
};

// decompress all the replay data into a buffer first
// then create non allocation packets from that
class FullDecompressReplayReader: public IReplayReader
{
  BaseReader crd;
  uint32_t curr_time=0;
  // expected_multiply_size is the expected compression ratio of the replay
  explicit FullDecompressReplayReader(Replay &replay, double expected_multiply_size=3);
  friend Replay;
public:
  ~FullDecompressReplayReader() override;
  bool getNextPacket(ReplayPacket &packet) override;
};

class CompressedReplayReader: public IReplayReader
{
  ZlibLoadCB reader; // reads data from the base reader to stream decompress.  much more memory efficient compared to FullDecompress, but much slower
  IGenReader * base_reader;
  uint32_t curr_time=0;
  explicit CompressedReplayReader(Replay &replay, IGenReader *base_reader, size_t in_size);
  friend Replay;
public:
  ~CompressedReplayReader() override;
  bool getNextPacket(ReplayPacket &packet) override;
};

template <bool streaming>
class ServerReplayReader : public IReplayReader {
  uint32_t replay_index=0;
  IReplayReader * curr_reader = nullptr;
  bool load_replay();
public:
  explicit ServerReplayReader(ServerReplay &replay);
  bool getNextPacket(ReplayPacket &packet) override;
};

