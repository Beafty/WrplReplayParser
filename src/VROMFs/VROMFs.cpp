
#include "VROMFs.h"

void VromfsFile::Save(std::ofstream *cb)
{
    DataBlock blk;
    auto loaded = owner->parseFileToDatablock(*this, blk);
    if(loaded)
    {
        std::ostringstream ss;

        blk.writeText(&ss, 0);
        auto s = ss.str();
        cb->write(s.c_str(), s.size());
    }
    else
    {
        auto raw = this->readRaw();
        cb->write(raw.data(), raw.size());
    }
}

bool VromfsFile::loadBlk(DataBlock &blk) {
  return this->owner->parseFileToDatablock(*this, blk);
}

