

#ifndef WTFILEUTILS_TYPES_H
#define WTFILEUTILS_TYPES_H
#include "string"
#include "cstdint"
#include "dag_assert.h"
#include "vector"
#include "array"
#include "Unit.h"
#include "ecs/entityId.h"
#include "math/dag_Point3.h"
namespace danet {
  enum AreaFlagsEnum : uint16_t {
    air = 1 << 0,
    unk1 = 1 << 1,
    unk2 = 1 << 2,
    unk3 = 1 << 3,
    ground = 1 << 4,
    mapArea = 1 << 5,
    team1 = 1 << 6,
    team2 = 1 << 7,
    killArea = 1 << 8,
    detectionArea = 1 << 9,
    airMapArea = 1 << 10,
    unk4 = 1 << 11,
    unk5 = 1 << 12,
    unk6 = 1 << 13,
    unk7 = 1 << 14,
    unk8 = 1 << 15,
  };

  struct UnitId { // so that Uid and uint16_t can have different encoders
    uint16_t val{};
    bool operator==(const UnitId& other) const = default;
  };
}

#include "codegen/types.h" // generated types

namespace danet {
#pragma pack(push, 1) // FUCK OFF COMPILER THIS IS 90 BYTES not 96 CAUSE GAIJIN SAID SO
  struct Uid {
    uint64_t player_id{};
    char name[82]{};

    std::string_view get_player_name() const {
      return std::string_view(name, strnlen(name, sizeof(name)));
    }
    bool operator==(const Uid& other) const {
      return std::memcmp(this, &other, sizeof(Uid)) == 0;
    }
  };
#pragma pack(pop)
}
G_STATIC_ASSERT(sizeof(danet::Uid) == 90);


#endif //WTFILEUTILS_TYPES_H
