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
#include "danet/delta/deltaCompression.h"

struct net_delta_t {
  net::DeltaComp netDelta{5, 0xd};
  std::vector<net::DeltaComp::History> histories{};
  net::DeltaComp::History& getHistory(uint16_t uid) {
    if(uid >= histories.size()) {
      histories.resize(uid+1);
    }
    auto &hist = histories[uid];
    if(!hist.isInited()) {
      // use_cache only ever matters in a network context
      netDelta.initHistory(hist, true, false);
    }
    return hist;
  }
};

enum ChatType: uint8_t {
  Team = 0,
  All = 1,
  Squad = 2,
  Direct = 3, // exists, but is currently unused from what I can tell
};

struct ChatMessage {
  uint32_t time_ms;
  std::string player_name; // player name
  std::string message; // message text
  ChatType channel = All; // what channel this message was sent on
  bool is_local_message{}; // a message that only ur client sees (ex: chat spamming message)
  bool is_quick_message{}; // is a message created through the quick message menu
  std::string complaints; // this is basically useless information
  inline bool FromBS(BitStream &bs) {
    bool ok = true;
    ok &= bs.Read(player_name);
    ok &=bs.Read(message);
    ok &=bs.Read(channel);
    ok &=bs.Read(is_local_message);
    ok &=bs.Read(is_quick_message);
    ok &=bs.Read(complaints);
    return ok;
  }
};


struct ParserState {

  explicit ParserState(int player_count=32) : players(player_count) {}
  explicit ParserState(IReplay *replay): players(replay->getHeader()->player_count) {}
private:

  //friend mpi::IObject *mpi::ObjectDispatcher(mpi::ObjectID oid, mpi::ObjectExtUID extUid, ParserState *state);


public:
  uint32_t replay_length_ms = 0;
  uint32_t current_rewind_ms;
  uint32_t curr_time_ms = 0;
  net::CNetwork conn{this};
  mpi::GeneralObject main_dispatch{this};
  net_delta_t NetDelta;
  std::vector<MPlayer> players;
  ecs::EntityManager g_entity_mgr{(ParserState*)this}; // this order is required as g_entity_mgr needs to be destroyed before players
  std::vector<MissionZone*> Zones{};
  std::array<TeamData, 3> teams{}; // team[0] is global data, teams[1] is first team, teams[2] is second team
  std::vector<ChatMessage> chatMessages{};
  GlobalElo glob_elo{};
  GeneralState gen_state{};
  std::vector<const mpi::IBattleMessage*> BattleMessages{};
  std::vector<MissionArea*> missionAreas1{};
  std::vector<MissionArea*> missionAreas2{};
  int current_packet_index=0;
  void setPlayerCount(int player_count) {
    players.resize(player_count);
  }
  ~ParserState() {
    ZoneScoped;
    this->rewindToMs(0xFFFFFFFF);
    for(auto v : Zones) {
      delete v;
    }
    for(auto v : BattleMessages) {
      delete v;
    }
    for (auto v : missionAreas1) {
      delete v;
    }
  }
  void onPacket(ReplayPacket *pkt) {
    conn.onPacket(pkt, pkt->timestamp_ms);
  }

  inline bool ParsePacket(ReplayPacket &pkt) {
    curr_time_ms = pkt.timestamp_ms;
    current_rewind_ms = curr_time_ms;
    switch (pkt.type) {
      case ReplayPacketType::EndMarker: {
        replay_length_ms = pkt.timestamp_ms;
        return false;
      }
      case ReplayPacketType::StartMarker: {
        break;
      }
      case ReplayPacketType::AircraftSmall: {
        FMSync(this, &pkt.stream);
        break;
      }
      case ReplayPacketType::Chat: {
        this->chatMessages.resize(this->chatMessages.size()+1);
        this->chatMessages[this->chatMessages.size()-1].FromBS(pkt.stream);
        this->chatMessages[this->chatMessages.size()-1].time_ms = this->curr_time_ms;
      }
      case ReplayPacketType::MPI: {
        auto m = mpi::dispatch(pkt.stream, this, false);
        if (m != nullptr) {
          mpi::send(m);
          if(m->delete_message)
            delete m;
        }
        break;
      }
      case ReplayPacketType::NextSegment: {
        //LOG("NextSegment");
        break;
      }
      case ReplayPacketType::ECS: {
        onPacket(&pkt);
        break;
      }
      case ReplayPacketType::Snapshot:
        break;
      case ReplayPacketType::ReplayHeaderInfo:
        break;
    }
    current_packet_index++;
    return true;
  }

  inline void rewindToMs(uint32_t time_ms) {
    if (current_rewind_ms == time_ms)
      return;
    //LOGI("rewinding to {} from {}", time_ms, current_rewind_ms);
    current_rewind_ms = time_ms;
    this->g_entity_mgr.rewindTo(time_ms);
    for (auto & p : players)
      p.rewindToTime(time_ms);
    for (auto & p : Zones)
      p->rewindToTime(time_ms);
    for (auto & p : missionAreas2)
      p->rewindToTime(time_ms);
    curr_time_ms = time_ms;
  }
};


#endif //MYEXTENSION_PARSERSTATE_H
