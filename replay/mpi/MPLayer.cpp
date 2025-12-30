#include "mpi/MPlayer.h"



ECS_REGISTER_CTM_TYPE(MPlayer, nullptr);
ECS_AUTO_REGISTER_COMPONENT(MPlayer, "m_player", nullptr)

mpi::Message *MPlayer::dispatchMpiMessage(mpi::MessageID) {
  { return NULL; }
}

void MPlayer::hello() {
  std::cout << "";
}
