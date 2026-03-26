#include "modules/State.h"
#include "state/ParserState.h"
#include "init/initialize.h"
#include "modules/mpi/codegen_objects.h"
#include "modules/replay/replay.h"
#include "Replay/Replay.h"

void PyReplayState::include(py::module_ &m) {
  DO_INCLUDE()
  py_replay_state.include(m);
  py_codegen_objects.include(m);

  m.def("initialize", initialize);
  m.def("g_log_flush", []() {
    g_log_handler.wait_until_empty();
    g_log_handler.flush_all();
  });
  py::class_<ParserState>(m, "ParserState")
      .def(py::init<>())
      .def(py::init<Replay *>())
      .def(py::init<ServerReplay *>())
      .def_readonly("players", &ParserState::players)
      .def_readonly("teams", &ParserState::teams)
      .def_readonly("gen_state", &ParserState::gen_state)
      .def_readonly("glob_elo", &ParserState::glob_elo)
      .def_readonly("zones", &ParserState::Zones)
      .def("set_player_count", &ParserState::setPlayerCount)
      .def("LoadFromReader", [](ParserState &state,
                                IReplayReader &rdr) { // the temporary exists for the entire call of this function, unlike __iter__
        //auto rdr = py_reader.cast<IReplayReader*>();
        ReplayPacket pkt{};
        bool end = false;
        while (!end && rdr.getNextPacket(&pkt)) {
          switch (pkt.type) {
            case ReplayPacketType::EndMarker: {
              end = true;
              break;
            }
            case ReplayPacketType::StartMarker: {
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
      });
}

PyReplayState py_replay_state{};