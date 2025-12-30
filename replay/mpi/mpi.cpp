
// Copyright (C) Gaijin Games KFT.  All rights reserved.

#include <mpi/mpi.h>
#include "math/dag_Point3.h"
#include "fstream"

struct ParserState;

template <typename T, uint8_t shift>
T shld(T shifted, T bit_filler)
{
  constexpr uint32_t bit_size = sizeof(T)*8;
  T out = shifted << shift;
  out |= bit_filler >> (bit_size-shift);
  return out;
}

void ParseLocation(uint32_t x1, uint32_t x2, Point3 &point) {
  constexpr float xmm13 = 1.0f; // asummed to always be 1.0f
  constexpr float xmm12 = -99.9;
  constexpr float FLOAT_146020698 = 4.768372718899627e-07;
  constexpr float FLOAT_146020650 = 36000.0;
  constexpr float FLOAT_14602069c = 0.007629402;
  uint32_t eax = x1;
  uint32_t ecx = x2;
  uint32_t edx = eax; // move edx, eax
  edx = shld<uint32_t, 0xa>(edx, ecx); // shld edx, ecx, A
  edx &= 0xFFFFF; // and edx, FFFFF
  eax >>= 0xa; // shr eax, A
  float xmm0 = 0; // xorps xmm0, xmm0
  xmm0 = (float)eax; // cvtsi2ss xmm0, eax
  float xmm3 = FLOAT_146020698; // movss xmm3,dword ptr ds:[7FF7266E0698]
  xmm0 = xmm0 * xmm3; //mulss xmm0,xmm3
  float xmm2 = -1.0f; // movss xmm2,dword ptr ds:[7FF7265FF220]
  xmm0 = xmm0 +  xmm2; // addss xmm0, xmm2
  float xmm1 = xmm2; // movaps xmm1, xmm2
  xmm1 = std::max(xmm1, xmm0); // maxss xmm1, xmm0
  xmm1 = std::min(xmm1, xmm13); // minss xmm1, xmm13
  xmm0 = 0; // xoprs, xmm0, xmm0
  xmm0 = (float)edx; // cvtsi2ss xmm0, eax
  float xmm4 = FLOAT_146020650; // movss xmm4,dword ptr ds:[7FF7266E0650]
  xmm1 *= xmm4; // mulss xmm1,xmm4
  point.x = xmm1; // movss dword ptr ss:[rsp+C8],xmm1
  xmm0 *=FLOAT_14602069c; // mulss xmm0,dword ptr ds:[7FF7266E069C]
  xmm0 += xmm12; // addss xmm0,xmm12
  ecx &= 0x3FFFFF; // and ecx,3FFFFF
  xmm1 = 0; // xorps xmm1,xmm1
  xmm1 = (float)ecx; // cvtsi2ss xmm1,ecx
  point.y = xmm0; // movss dword ptr ss:[rsp+CC],xmm0
  xmm1 *= xmm3; // mulss xmm1,xmm3
  xmm1 += xmm2; // addss xmm1,xmm2
  xmm0 = xmm2; // movaps xmm0,xmm2
  xmm0 = std::max(xmm0, xmm1);
  xmm0 = std::min(xmm0, xmm13);
  xmm0 *= xmm4;
  point.z = xmm0;
  LOG("%f; %f; %f\n", point.x, point.y, point.z);
}

namespace mpi {
#define MAX_CALL_DEPTH (16)
  static IMessageListener *message_listeners = nullptr;
  object_dispatcher obj_dispatcher = nullptr;
  static thread_local int curr_call_depth = 0;

  IObject *dispatch_object(ObjectID oid, ObjectExtUID ext, ParserState *state) {
    return static_cast<IObject *>((oid != INVALID_OBJECT_ID && obj_dispatcher) ? obj_dispatcher(oid, ext, state) : nullptr);
  }
  std::unordered_map<int, std::unordered_map<int, int>> mpi_data{};
  Message *dispatch(const BitStream &bs, ParserState *state, bool copy_payload) {
    static std::ofstream strm{R"(D:\Python\ReplayerParser\test\plots.txt)"};
    ObjectID oid;
    ObjectExtUID extUid;
    MessageID mid;
    if (obj_dispatcher && read_object_ext_uid(bs, oid, extUid) && bs.Read(mid)) {
      //LOG("oid: 0x%04X; extUid: 0x%08X; mid: 0x%04X;\n", oid, extUid, mid);
      mpi_data[oid][mid] += 1;
      if(mid == 0xF0CC)
      {

      }
      if(mid == 0xF0E9)
      {
        //bs.IgnoreBytes(7);
        //double x1;
        //double x2;
        //double x3;
        //bs.Read(x1);
        //bs.Read(x2);
        //bs.Read(x3);
        //LOG("%f, %f, %f\n", x1, x2, x3);
        //std::ostringstream oss;
        //std::ostringstream hexoss;
        //FormatBytesToStream(oss, std::span<char>((char*)bs.GetData(), bs.GetNumberOfBytesUsed()));
        //LOG("%s\n", oss.str().data());
        //FormatHexToStream(hexoss, std::span<char>((char*)bs.GetData(), bs.GetNumberOfBytesUsed()));
        //LOG("%s\n", hexoss.str().data());
      }

      if (oid == 0x5802 && (mid == 0xF074 | mid == 0xF074))
      {
        //auto start = bs.GetReadOffset();
        //uint32_t length_maybe;
        //bs.ReadCompressed(length_maybe);

        //bs.SetReadOffset(bs.GetReadOffset()+155);
        //uint32_t s1, s2;
        //bs.Read(s1);
        //bs.Read(s2);
        //Point3 point{};
        //bs.SetReadOffset(start);
        //ParseLocation(s1, s2, point);
        //strm << std::format("{} {} {}\n", point.x, point.y, point.z);
      }
      IObject *o = dispatch_object(oid, extUid, state);
      if (o) {
        Message *m = (mid != INVALID_MESSAGE_ID) ? o->dispatchMpiMessage(mid) : nullptr;
        if (m) {
          m->payload.~BitStream();
          G_ASSERT(!(bs.GetReadOffset() & 7)); // bit offset
          uint32_t bytesReaded = BITS_TO_BYTES(bs.GetReadOffset());
          new(&m->payload)
              BitStream(bs.GetData() + bytesReaded, bs.GetNumberOfBytesUsed() - bytesReaded, copy_payload);
          if (m->readPayload())
            return m;
          else
            m->destroy();
        }
      }
    }
    return nullptr;
  }

  void sendto(Message *m, SystemID receiver) {
    if (++curr_call_depth >= MAX_CALL_DEPTH) {
      G_ASSERTF(0, "{} call depth is {}! Infinite recursion?!", __FUNCTION__, (int) curr_call_depth);
      LOG("{} call depth is {}! Infinite recursion?!", __FUNCTION__, (int) curr_call_depth);
      --curr_call_depth;
      return;
    }

    if (!m);
    else if (!m->obj)
      LOG("{} ignore attempt to send message {} ({:#x}) without recepient object\n", __FUNCTION__, fmt::ptr(m), m->id);
    else
      for (IMessageListener *l = message_listeners; l; l = l->next) // in reality, only one IMessageListener
        if (m->isApplicable(l))
          l->receiveMpiMessage(m, receiver);
    --curr_call_depth;
  }

  void register_listener(IMessageListener *l) {
    l->next = nullptr;
    IMessageListener *prev = nullptr;
    for (IMessageListener *ll = message_listeners; ll; prev = ll, ll = ll->next);
    (prev ? prev->next : message_listeners) = l;
  }

  void unregister_listener(IMessageListener *l) {
    IMessageListener *prev = nullptr;
    for (IMessageListener *ll = message_listeners; ll; prev = ll, ll = ll->next)
      if (l == ll) {
        if (prev)
          prev->next = l->next;
        if (l == message_listeners)
          message_listeners = l->next;
        break;
      }
  }

  void register_object_dispatcher(object_dispatcher od) { obj_dispatcher = od; }
}; // namespace mpi