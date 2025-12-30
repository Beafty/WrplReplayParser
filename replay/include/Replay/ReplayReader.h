

#ifndef MYEXTENSION_REPLAYREADER_H
#define MYEXTENSION_REPLAYREADER_H

#include "reader.h"
#include "basic_replay_structs.h"
#include "consts.h"

uint32_t getPacketSize(GenReader &cb);
class IReplayReader
{
public:
  virtual ~IReplayReader();
  virtual bool getNextPacket(ReplayPacket *packet) = 0;
};

class ReplayReader : public IReplayReader
{
  ZlibLoadCB *crd;
  GenReader *baseRdr;
  uint32_t curr_time;
public:
  explicit ReplayReader(ZlibLoadCB *crd)
  {
    this->crd = crd;
    this->baseRdr = nullptr;
  }

  explicit ReplayReader(ZlibLoadCB *crd, GenReader *rdr)
  {
    this->crd = crd;
    this->baseRdr = rdr;
  }
  

  bool getNextPacket(ReplayPacket *packet) override
  {
    
    uint32_t pkt_sz = getPacketSize(*crd);
    if(pkt_sz == 0)
      return false;
    packet->stream.~BitStream();
    packet->stream = BitStream();
    packet->stream.reserveBits(BYTES_TO_BITS(pkt_sz));
    if(crd->tryRead(packet->stream.GetData(), (int)pkt_sz) != pkt_sz)
      return false;
    packet->stream.SetWriteOffset(BYTES_TO_BITS(pkt_sz));
    uint16_t type_t;
    packet->stream.Read(type_t);
    // if two packets have the same timestamp, then only the first one encodes the timestamp
    if((type_t & 0x10) == 0)
    {
      packet->stream.Read(curr_time);
    }
    else
    {
      type_t ^= 0x10;
    }
    packet->timestamp_ms = curr_time;
    packet->type = (ReplayPacketType)type_t;
    return true;
  }

  ~ReplayReader() override
  {
    if(crd)
    {
      crd->ceaseReading();
    }
    delete crd;
    delete baseRdr;
  }
};
class Replay;
class ServerReplayReader : public IReplayReader
{
  std::vector<ReplayReader*> readers;
  int index = 0;
public:
  explicit ServerReplayReader(std::vector<Replay> &rdrs);
  bool getNextPacket(ReplayPacket *packet) override;
  ~ServerReplayReader() override;
};

#endif //MYEXTENSION_REPLAYREADER_H
