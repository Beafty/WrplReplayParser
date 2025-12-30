

#ifndef MYEXTENSION_TEAMDATA_H
#define MYEXTENSION_TEAMDATA_H

#include "mpi/serializers.h"
class TeamData : public danet::ReflectableObject {
public:
  TeamData() {
    varList.head = &score;
    varList.tail = &roundScore;
  }
  danet::ReflectionVar<uint16_t> score{"score", &tickets, 2};
  danet::ReflectionVar<uint16_t> tickets{"tickets", &orderCooldownTotal, 3};
  danet::ReflectionVar<uint32_t> orderCooldownTotal{"orderCooldownTotal", &orderCooldownLeft, 4};
  danet::ReflectionVar<uint32_t> orderCooldownLeft{"orderCooldownLeft", &spawnScore, 5};
  danet::ReflectionVar<uint32_t> spawnScore{"spawnScore", &roundScore, 6};
  danet::ReflectionVar<uint32_t> roundScore{"roundScore", nullptr, 7};
};

#endif //MYEXTENSION_TEAMDATA_H
