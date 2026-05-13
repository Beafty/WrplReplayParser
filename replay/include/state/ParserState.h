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
  explicit ParserState(Replay *replay): players(replay->PlayerCount) {}
  explicit ParserState(ServerReplay *replay): players(replay->replay_files[0].PlayerCount) {}
  explicit ParserState(MemoryEfficientServerReplay *replay): players(replay->base_replay->PlayerCount) {}
private:

  //friend mpi::IObject *mpi::ObjectDispatcher(mpi::ObjectID oid, mpi::ObjectExtUID extUid, ParserState *state);
public:
  net::CNetwork conn{this};
  mpi::GeneralObject main_dispatch{this};
  net_delta_t NetDelta;
  std::vector<MPlayer> players;
  ecs::EntityManager g_entity_mgr{(ParserState*)this}; // this order is required as g_entity_mgr needs to be destroyed before players
  std::vector<BaseZone*> Zones{};
  std::array<TeamData, 3> teams{}; // team[0] is global data, teams[1] is first team, teams[2] is second team
  std::vector<ChatMessage> chatMessages{};
  GlobalElo glob_elo{};
  GeneralState gen_state{};
  std::vector<const mpi::IBattleMessage*> BattleMessages{};
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
    for(auto v : BattleMessages) {
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
        LOG("NextSegment");
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
};


#endif //MYEXTENSION_PARSERSTATE_H
