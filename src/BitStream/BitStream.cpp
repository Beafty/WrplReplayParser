#include "danet/Bitstream.h"
#include "DataBlock.h"
#include "reader.h"

bool BitStream::Read(DataBlock &blk) const
{
  uint32_t bytesToRead = 0;
  if (!ReadCompressed(bytesToRead))
    return false;
  if (!bytesToRead)
    return true;
  AlignReadToByteBoundary();
  if (readOffset + bytes2bits(bytesToRead) > bitsUsed)
    return false;

  // ok look like this cast is ugly as fuck, and it's my fault and I don't feel like fixing it
  BaseReader rdr{(char*)const_cast<uint8_t*>(GetData() + bits2bytes(readOffset)), (int)bytesToRead, false};
  bool ret = false;
  if (blk.loadFromStream(rdr, nullptr, nullptr))
  {
    readOffset += bytes2bits(bytesToRead);
    ret = true;
  }
  return ret;
}

// gaijins idea not mine
void BitStream::swap(BitStream &bs)
{
#define _SWAP(type, l, r) \
  {                       \
    type t = l;           \
    l = r;                \
    r = t;                \
  }
  _SWAP(uint32_t, this->bitsUsed, bs.bitsUsed)
  _SWAP(uint32_t, this->dataOwner, bs.dataOwner)
  _SWAP(uint32_t, this->bitsAllocated, bs.bitsAllocated)
  _SWAP(uint32_t, this->readOffset, bs.readOffset)
  _SWAP(uint8_t *, this->data, bs.data)
#undef _SWAP
}
