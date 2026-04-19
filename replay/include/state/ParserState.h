#ifndef MYEXTENSION_PARSERSTATE_H
#define MYEXTENSION_PARSERSTATE_H
#include "ecs/EntityManager.h"
#include "mpi/mpi.h"
#include "mpi/codegen/ReflIncludes.h"
#include "network/CNetwork.h"
#include "mpi/ObjectDispatcher.h"
#include "mpi/GeneralObject.h"
#include "tracy/Tracy.hpp"
#include "Replay/Replay.h"
#include "mpi/PositionSync.h"

struct ParserState {

  explicit ParserState(int player_count=32) : players(player_count) {}
  explicit ParserState(Replay *replay): players(replay->PlayerCount) {}
  explicit ParserState(ServerReplay *replay): players(replay->replay_files[0].PlayerCount) {}
  explicit ParserState(MemoryEfficientServerReplay *replay): players(replay->base_replay->PlayerCount) {}
  std::vector<MPlayer> players;
  ecs::EntityManager g_entity_mgr{(ParserState*)this}; // this order is required as g_entity_mgr needs to be destroyed before players
  std::vector<BaseZone*> Zones;
  std::array<TeamData, 3> teams; // team[0] is global data, teams[1] is first team, teams[2] is second team
  GlobalElo glob_elo{};
  GeneralState gen_state{};
  net::CNetwork conn{this};
  mpi::GeneralObject main_dispatch{this};
  BattleMessageHandler battles_messages{this};
  uint32_t curr_time_ms = 0;
  int current_packet_index=0;
  void setPlayerCount(int player_count) {
    players.resize(player_count);
  }
  ~ParserState() {
    ZoneScoped;
    for(auto v : Zones) {
      delete v;
    }
  }
  void onPacket(ReplayPacket *pkt) {
    conn.onPacket(pkt, pkt->timestamp_ms);
  }

  inline bool ParsePacket(ReplayPacket &pkt) {
    curr_time_ms = pkt.timestamp_ms;
    switch (pkt.type) {
      case ReplayPacketType::EndMarker: {
        return true;
      }
      case ReplayPacketType::StartMarker: {
        break;
      }
      case ReplayPacketType::AircraftSmall: {
        FMSync(this, &pkt.stream);
        break;
      }
      case ReplayPacketType::Chat:
        break;
      case ReplayPacketType::MPI: {
        auto m = mpi::dispatch(pkt.stream, this, false);
        if (m != nullptr) {
          mpi::send(m);
          delete m;
        }
        break;
      }
      case ReplayPacketType::NextSegment: {
        LOG("NextSegment");
        break;
      }
      case ReplayPacketType::ECS: {
        onPacket(&pkt);
        break;
      }
      case ReplayPacketType::Snapshot: // useless
        break;
      case ReplayPacketType::ReplayHeaderInfo:
        break;
    }
    current_packet_index++;
    return false;
  }
};


#endif //MYEXTENSION_PARSERSTATE_H
