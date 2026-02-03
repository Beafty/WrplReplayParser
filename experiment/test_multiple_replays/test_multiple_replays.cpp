#include "ecs/EntityManager.h"
#include "ecs/ComponentTypesDefs.h"
#include "network/CNetwork.h"
#include "FileSystem.h"
#include "init/initialize.h"
#include "mpi/mpi.h"

#include <ctime>
#include "Replay/Replay.h"
#include "Replay/find_rpl_files.h"
#include "mpi/ObjectDispatcher.h"
#include "Logger.h"

#include "state/ParserState.h"
#include <cctype>

std::string convert_os_path_to_wsl2(std::string &str) { // this function assumes a windows os with a wsl2 linux
  G_ASSERTF(str[1] == ':', "must be an absolute path");
  std::string payload = "/mnt/";
  payload += static_cast<char>(std::tolower(str[0]));
  payload += "/";
  payload += str.substr(3);
  std::replace(payload.begin(), payload.end(), '\\', '/');
  return payload;
}

std::string convert_os_path_to_wsl2(const char *str) {
  std::string t(str);
  return convert_os_path_to_wsl2(t);
}
int main() {

  //std::signal(SIGSEGV, signal_handler);
  fs::path conf_dir = CONFIG_DIR;
  g_log_handler.set_default_sink_logfile((conf_dir / "logfile.txt").string());
  g_log_handler.start_thread();
  fs::path config_file = conf_dir / "dagor_replay_test.blk";
  DataBlock conf_blk;
  G_ASSERT(load(conf_blk, config_file.string().c_str()));

  bool bin_is_linux_path = conf_blk.getBool("bin_is_linux_path", false);
  auto bin_path = conf_blk.getStr("bin_path", nullptr);
#ifdef Linux
  std::string bin_path_str = convert_os_path_to_wsl2(bin_path);
#else
  std::string bin_path_str = bin_path;
#endif
  G_UNUSED(bin_is_linux_path);
  initialize(bin_path_str);
  std::string replay_paths[] = {
      R"(D:\SteamLibrary\steamapps\common\War Thunder\Replays\sqbReplay.wrpl)",
      R"(D:\SteamLibrary\steamapps\common\War Thunder\Replays\test_arb.wrpl)",
      R"(D:\SteamLibrary\steamapps\common\War Thunder\Replays\test_grb.wrpl)",
      R"(D:\SteamLibrary\steamapps\common\War Thunder\Replays\sqbReplay.wrpl)", // cause why not

  };
  auto *pkt = new ReplayPacket();
  for(auto & path : replay_paths) {
#ifdef Linux
    std::string true_path = convert_os_path_to_wsl2(path);
#else
    std::string true_path = path;
#endif
    Replay rpl{true_path};
    IReplayReader *rdr = rpl.getRplReader();
    ParserState state{};
    net::CNetwork net{&state};
    bool end = false;
    int AircraftCount = 0;
    while (!end && rdr->getNextPacket(pkt))
    {
      switch(pkt->type)
      {
        case ReplayPacketType::EndMarker:
        {
          LOG("Replay Ending at time {}", ((float)pkt->timestamp_ms)/1000);
          end = true;
          break;
        }
        case ReplayPacketType::StartMarker:
        {
          LOGD("Replay StartMarker");
          break;
        }
        case ReplayPacketType::AircraftSmall:
        {
          AircraftCount++;
          break;
        }
        case ReplayPacketType::Chat:
          break;
        case ReplayPacketType::MPI: {
          auto m = mpi::dispatch(pkt->stream, &state, false);
          if(m != nullptr)
          {
            mpi::send(m);
            delete m;
          }
          break;
        }
        case ReplayPacketType::NextSegment: {
          LOG("NextSegment");
          break;
        }
        case ReplayPacketType::ECS:
        {
          net.onPacket(pkt, pkt->timestamp_ms);
          break;
        }
        case ReplayPacketType::Snapshot: // useless
          break;
        case ReplayPacketType::ReplayHeaderInfo:
          break;
      }
      //std::cout.flush();
    }
    delete rdr;
  }
  delete pkt;
  return 0;
}