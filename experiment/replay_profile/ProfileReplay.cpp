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

#include <csignal>
#include <cstdlib>

#include "tracy/Tracy.hpp"
//#include "tracy/public/tracy/Tracy.hpp"

//#include <unistd.h>

//TracyNoop;


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


int main()
{
  ZoneScoped;
  //std::signal(SIGSEGV, signal_handler);
  fs::path conf_dir = CONFIG_DIR;
  fs::path config_file = conf_dir / "dagor_replay_test.blk";
  DataBlock conf_blk;
  G_ASSERT(load(conf_blk, config_file.string().c_str()));
  bool is_server_replay = conf_blk.getBool("is_server_replay", false);
  bool source_is_linux_path = conf_blk.getBool("source_is_linux_path", false);
  auto replay_path = conf_blk.getStr("source", nullptr);
  bool bin_is_linux_path = conf_blk.getBool("bin_is_linux_path", false);
  auto bin_path = conf_blk.getStr("bin_path", nullptr);
  std::string rpl_path_str = replay_path;
  std::string bin_path_str = bin_path;
  G_UNUSED(source_is_linux_path);
  G_UNUSED(bin_is_linux_path);
#ifdef Linux
  if(!source_is_linux_path) {
    rpl_path_str = convert_os_path_to_wsl2(replay_path);
  }
  if(!bin_is_linux_path) {
    bin_path_str = convert_os_path_to_wsl2(bin_path);
  }
#endif
  std::string logfile_str = (conf_dir / "logfile.txt").string();
  initialize(bin_path_str, logfile_str);
  g_log_handler.start_thread();
  //auto t = ecs::g_ecs_data->getTemplateDB()->getTemplate("attachable_wear_fast_sf_helmet_item");
  IReplayReader *rdr = nullptr;
  ServerReplay *srv_rpl = nullptr;
  Replay *rpl = nullptr;
  double timer_sum = 0.0;
  {
    ZoneScopedN("Replay Load")
    if(is_server_replay)
    {
      fs::path t{rpl_path_str};
      srv_rpl = new ServerReplay(t);
    }
    else
    {
      rpl = new Replay(rpl_path_str);
    }
  }
  int timer_count = 200;
  for(int i = 0; i < timer_count; i++) {
    auto start = std::chrono::high_resolution_clock::now();
    ZoneScopedN("loop")
    {
      ZoneScopedN("GetReader")
      if(is_server_replay) {
        rdr = srv_rpl->getRplReader();
      } else {
        rdr = rpl->getFullDecompressReplayReader();
      }
    }

    {
      ZoneScopedN("Parsing Overall")
      ReplayPacket pkt{};
      ParserState *state_ptr = new ParserState{};
      ParserState &state = *state_ptr;
      //std::exit(0);
      bool end = false;
      {

        ZoneScopedN("Parsing Only")
        while (!end && rdr->getNextPacket(&pkt)) {

          switch (pkt.type) {
            case ReplayPacketType::EndMarker: {
              LOG("Replay Ending at time {}", ((float) pkt.timestamp_ms) / 1000);
              end = true;
              break;
            }
            case ReplayPacketType::StartMarker: {
              LOGD("Replay StartMarker");
              break;
            }
            case ReplayPacketType::AircraftSmall: {
              break;
            }
            case ReplayPacketType::Chat:
              break;
            case ReplayPacketType::MPI: {
              auto m = mpi::dispatch(pkt.stream, &state, false);
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
              state.onPacket(&pkt);
              break;
            }
            case ReplayPacketType::Snapshot: // useless
              break;
            case ReplayPacketType::ReplayHeaderInfo:
              break;
          }
          //std::cout.flush();
        }
      }
      {
        ZoneScopedN("Object destruction")
        delete state_ptr;
        delete rdr;
      }
      {
        ZoneScopedN("End Stuff")
        auto ended = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = ended - start;
        timer_sum += duration.count();
        {
          //ZoneScopedN("Printing times");
          //std::cout << i << " profile time: " << duration.count() << " : " << (timer_sum / (double) (i + 1)) << "\n";
        }
      }
      int * bad_ptr = 0x0;
      *bad_ptr = 5;
    }
  }

  std::cout << "Average time: " << (timer_sum / (double) (timer_count)) << std::endl;

  delete srv_rpl;
  delete rpl;

  //auto ptr = &mpi::players;
  //for(auto &data : mpi::mpi_data)
  //{
  //  LOG("OID: {:#x}", data.first);
  //  for(auto &data2: data.second)
  //  {
  //    LOG("    mid: {:#x}; count: {}", data2.first, data2.second);
  //  }
  //}

  // Output the result
  //rpl.HeaderBlk.printBlock(0, std::cout);
  //rpl.FooterBlk.printBlock(0, std::cout);
  //ecs::g_entity_mgr->debugPrintEntities();
  //rpl.HeaderBlk.printBlock(0, std::cout);
  //ecs::g_entity_mgr->getTemplateDB()->DebugPrint();
  return 0;
}