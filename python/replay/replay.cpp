#include "modules/replay/replay.h"
#include "modules/DataBlock/DataBlock.h"
#include "modules/BitStream.h"
#include "Replay/Replay.h"
#include "memory.h"
using ssize_t = Py_ssize_t; // msvc doesnt think
struct IReplayReaderIterInto {
  ReplayPacket * into;
  py::object py_reader_ref; // done to keep a py ref so tempory py object doesnt go out of scope during iteration
  IReplayReader * rdr;
  IReplayReaderIterInto(py::object py_reader, ReplayPacket* pkt)
      : into(pkt), py_reader_ref(std::move(py_reader)), rdr(py_reader_ref.cast<IReplayReader*>()) {}
};

template <size_t N>
std::string_view getStr(const char (&arr)[N]) {
  auto length = strlen(arr);
  length = length > 0 ? length : 0;
  return {arr, std::min(length, N)};
}

void PyReplay::include(py::module_ &m) {
  DO_INCLUDE()
  py_data_block.include(m);
  py_bitstream.include(m);
  py::module_ sub = m.def_submodule("replay", "exposes various replay file loading and manipulating apis");

  py::enum_<ReplayPacketType>(sub, "ReplayPacketTypes")
      .value("EndMarker", ReplayPacketType::EndMarker)
      .value("StartMarker", ReplayPacketType::StartMarker)
      .value("Aircraft", ReplayPacketType::AircraftSmall)
      .value("Chat", ReplayPacketType::Chat)
      .value("Mpi", ReplayPacketType::MPI)
      .value("NextSegment", ReplayPacketType::NextSegment)
      .value("ECS", ReplayPacketType::ECS)
      .value("Snapshot", ReplayPacketType::Snapshot)
      .value("ReplayHeaderInfo", ReplayPacketType::ReplayHeaderInfo);

  py::class_<ReplayPacket>(sub, "ReplayPacket")
      .def(py::init<>())
      .def_readonly("type", &ReplayPacket::type)
      .def_readonly("data", &ReplayPacket::stream)
      .def_readonly("time_ms", &ReplayPacket::timestamp_ms);

  py::class_<ReplayHeader>(sub, "ReplayHeader")
      .def_readonly("header", &ReplayHeader::header)
      .def_readonly("magic", &ReplayHeader::magic)
      .def_property_readonly("level_path", [](ReplayHeader &header) {return getStr(header.level_path);})
      .def_property_readonly("mission_path", [](ReplayHeader &header) {return getStr(header.mission_file);})
      .def_property_readonly("environment", [](ReplayHeader &header) {return getStr(header.environment);})
      .def_property_readonly("visibility", [](ReplayHeader &header) {return getStr(header.weather);})
      .def_readonly("difficulty", &ReplayHeader::difficulty)
      .def_readonly("SessionType", &ReplayHeader::SessionType)
      .def_readonly("player_count", &ReplayHeader::player_count)
      .def_readonly("session_id", &ReplayHeader::session_id)
      .def_readonly("replay_part_number", &ReplayHeader::replay_part_number)
      .def_readonly("MSetSize", &ReplayHeader::MSetSize)
      .def_property_readonly("location_name", [](ReplayHeader &header) {return getStr(header.location_name);})
      .def_readonly("start_time", &ReplayHeader::start_time)
      .def_readonly("time_limit", &ReplayHeader::time_limit)
      .def_readonly("score_limit", &ReplayHeader::score_limit)
      .def_property_readonly("battle_class", [](ReplayHeader &header) {return getStr(header.battle_class);})
      .def_property_readonly("battle_kill_streak", [](ReplayHeader &header) {return getStr(header.battle_kill_streak);});

  py::class_<IReplayReaderIterInto>(sub, "IReplayReaderIterInto")
      .def("__iter__", [](IReplayReaderIterInto &rdr) {return rdr;})
      .def("__next__", [](IReplayReaderIterInto &rdr){
        if(rdr.rdr->getNextPacket(*rdr.into)) {
          return py::none{};
        }
        throw py::stop_iteration();
      });

  py::class_<IReplayReader>(sub, "IReplayReader")
      .def("getNextPacket", &IReplayReader::getNextPacket)
      .def("iterInto", [](py::object rdr, ReplayPacket &pkt) {
        return std::make_unique<IReplayReaderIterInto>(rdr, &pkt);
      })
      .def("__iter__", [](IReplayReader &rdr) -> IReplayReader& {return rdr;})
      .def("__next__", [](IReplayReader &rdr) {
        auto pkt = std::make_unique<ReplayPacket>();
        if(rdr.getNextPacket(*pkt.get()))
          return pkt;
        else {
          pkt.reset();
          throw py::stop_iteration();
        }
      });
  py::class_<FullDecompressReplayReader, IReplayReader>(sub, "FullDecompressReplayReader");

  py::class_<CompressedReplayReader, IReplayReader>(sub, "CompressedReplayReader");

  py::class_<ServerReplayReader<true>, IReplayReader>(sub, "CompressedServerReplayReader");

  py::class_<ServerReplayReader<false>, IReplayReader>(sub, "FullDecompressServerReplayReader");

  py::class_<IReplay>(sub, "IReplay")
      .def("getHeaderBlk", &IReplay::getHeaderBlk, py::return_value_policy::reference)
      .def("getFooterBlk", &IReplay::getFooterBlk, py::return_value_policy::reference)
      .def("getReplayReader", &IReplay::getReplayReader, py::return_value_policy::take_ownership, py::keep_alive<0, 1>())
      .def("getCompressedReplayReader", &IReplay::getCompressedReplayReader, py::return_value_policy::take_ownership, py::keep_alive<0, 1>())
      .def_property_readonly("getHeader", &IReplay::getHeader, py::return_value_policy::reference)
      .def_property_readonly("isValid", &IReplay::isValid);

  py::class_<Replay, IReplay>(sub, "Replay")
      .def(py::init<const std::string &>());

  py::class_<ServerReplay, IReplay>(sub, "ServerReplay")
      .def(py::init<const std::string &>());
}

PyReplay py_replay{};
