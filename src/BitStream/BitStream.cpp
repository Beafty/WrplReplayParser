#include "Bitstream.h"

/*
bool BitStream::Read(DataBlock &blk) const
{
  blk.incRef();
  uint32_t bytesToRead = 0;
  if (!ReadCompressed(bytesToRead))
    return false;
  if (!bytesToRead)
    return true;
  AlignReadToByteBoundary();
  if (readOffset + bytes2bits(bytesToRead) > bitsUsed)
    return false;
  InPlaceMemLoadCB crd(GetData() + bits2bytes(readOffset), bytesToRead);
  bool ret = false;
  if (blk.load_from_stream(crd))
  {
    readOffset += bytes2bits(bytesToRead);
    ret = true;
  }
  return ret;
}*/
