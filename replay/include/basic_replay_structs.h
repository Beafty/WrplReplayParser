
#pragma once
#include "danet/BitStream.h"

enum class ReplayPacketType : uint16_t {
  EndMarker = 0, // last packet in a replay
  StartMarker = 1, // not fully sure what represents
  AircraftSmall = 2, // Aircraft packet
  Chat = 3, // chat message
  MPI = 4, // MPI message
  NextSegment = 5, // this is used to communicate the next server replay file, I dont use it. usually last packet
  ECS = 6, // net ECS message
  Snapshot = 7,
  ReplayHeaderInfo = 8, // sometimes first packet written, holds the ECS message hashes for synchronization. I haven't seen this on client replays in a while (they were there like a year ago?) but they have been in server replays I think
};
extern std::string packet_names[];



struct ReplayPacket {
public:
  ReplayPacketType type{};
  uint32_t timestamp_ms{};
  // (Runtime state, not file data)
  //uint64_t runtimePayloadSizeBytes{};
  BitStream stream;
};
