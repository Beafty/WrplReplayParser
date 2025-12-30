#include "replay/Replay.h"
#include "Replay/ReplayReader.h"


ServerReplayReader::ServerReplayReader(std::vector<Replay> &rdrs) {
  this->readers.reserve(rdrs.size());
  for(auto &rpl : rdrs)
  {
    this->readers.push_back(rpl.getRplReader());
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
