#include "modules/mpi/battle_messages.h"
#include "modules/ecs/EntityId.h"
#include "mpi/GeneralObject.h"
PyBattleMessages py_battle_messages;
void PyBattleMessages::include(py::module_ &m) {
  DO_INCLUDE()
  py_entity_id.include(m);
  auto mpi = m.def_submodule("mpi");
  py::class_<mpi::IBattleMessage>(mpi, "IBattleMessage")
      .def_readonly("time_ms", &mpi::IBattleMessage::time_ms);

  py::class_<mpi::KillMessage, mpi::IBattleMessage>(mpi, "KillMessage")
      .def_readonly("offender_vehicle", &mpi::KillMessage::offender_vehicle)
      .def_readonly("used_weapon", &mpi::KillMessage::used_weapon)
      .def_readonly("destroyed_weapon", &mpi::KillMessage::destroyed_weapon)
      .def_readonly("offended_entity", &mpi::KillMessage::offended_entity)
      .def_readonly("offender_entity", &mpi::KillMessage::offender_entity)
      .def_readonly("offender_pid", &mpi::KillMessage::offender_pid)
      .def_readonly("VictimPid", &mpi::KillMessage::VictimPid)
      .def_readonly("unitType", &mpi::KillMessage::unitType)
      .def_readonly("is_burav_kill", &mpi::KillMessage::maybe_is_burav_kill)
      .def_readonly("offender_vehicle", &mpi::KillMessage::offender_vehicle);

  py::class_<mpi::SevereDamageMessage, mpi::IBattleMessage>(mpi, "SevereDamageMessage")
      .def_readonly("offended_eid", &mpi::SevereDamageMessage::offended_eid)
      .def_readonly("player_pid", &mpi::SevereDamageMessage::player_pid)
      .def_readonly("vehicle", &mpi::SevereDamageMessage::vehicle)
      .def_readonly("player_eid", &mpi::SevereDamageMessage::player_eid)
      .def_readonly("unitType", &mpi::SevereDamageMessage::unitType);

  py::class_<mpi::CriticalDamageMessage, mpi::IBattleMessage>(mpi, "CriticalDamageMessage")
      .def_readonly("offended_eid", &mpi::CriticalDamageMessage::offended_eid)
      .def_readonly("player_pid", &mpi::CriticalDamageMessage::player_pid)
      .def_readonly("vehicle", &mpi::CriticalDamageMessage::vehicle)
      .def_readonly("player_eid", &mpi::CriticalDamageMessage::player_eid)
      .def_readonly("is_fire", &mpi::CriticalDamageMessage::is_fire)
      .def_readonly("unitType", &mpi::CriticalDamageMessage::unitType);

  py::class_<mpi::AwardMessage, mpi::IBattleMessage>(mpi, "AwardMessage")
      .def_readonly("player_pid", &mpi::AwardMessage::player_pid)
      .def_readonly("award", &mpi::AwardMessage::award)
      .def_readonly("stage", &mpi::AwardMessage::stage)
      .def_readonly("wp", &mpi::AwardMessage::wp)
      .def_readonly("exp", &mpi::AwardMessage::exp);

}