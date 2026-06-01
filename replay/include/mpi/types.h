

#ifndef WTFILEUTILS_TYPES_H
#define WTFILEUTILS_TYPES_H

namespace unit {
  class Unit;
}

#include "string"
#include "cstdint"
#include "dag_assert.h"
#include "vector"
#include "array"
#include "danet/daNetTypes.h"
#include "ecs/entityId.h"
#include "math/dag_Point3.h"
#include "reflection.h"
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

  enum AccountType: uint8_t {
    None = 0,
    PC = 2,
    Xbox = 12,
    Psn = 15,
  };

#pragma pack(push, 1) // FUCK OFF COMPILER THIS IS 90 BYTES not 96 CAUSE GAIJIN SAID SO
  struct Uid {
    uint64_t player_id{};
    char name[81]{};
    AccountType account_type{};

    std::string_view get_player_name() const {
      return std::string_view(name, strnlen(name, sizeof(name)));
    }
    bool operator==(const Uid& other) const {
      return std::memcmp(this, &other, sizeof(Uid)) == 0;
    }
  };
#pragma pack(pop)
G_STATIC_ASSERT(sizeof(danet::Uid) == 90);
class WeaponsMask;

// a WeaponMask uses a bitfield to represent what weapons still visually exist
// maybe integrate with ammunition from unit::Unit
class WeaponMask {
  union {
    uint64_t raw;
    uint8_t *ptr;
  };

  uint16_t ammo_count{}; // number of visible ammunition
  uint16_t weapon_index{}; // index of weapon in weapon array

  [[nodiscard]] uint8_t *get_mask() {
    if (ammo_count > BYTES_TO_BITS(sizeof(uint64_t)))
      return ptr;
    return (uint8_t *) &raw;
  }

  friend WeaponsMask;

  friend int WeaponsCoder(DANET_ENCODER_SIGNATURE);

public:
  [[nodiscard]] const uint16_t get_num_weapons() const { return ammo_count; }
  [[nodiscard]] const uint16_t get_weapon_index() const { return weapon_index; }

  [[nodiscard]] const uint8_t *get_mask_c() const {
    if (ammo_count > BYTES_TO_BITS(sizeof(uint64_t)))
      return ptr;
    return (uint8_t *) &raw;
  }

  explicit WeaponMask(uint16_t ammo_count, uint16_t weapon_index) {
    this->ammo_count = ammo_count;
    this->weapon_index = weapon_index;
    if (ammo_count > BYTES_TO_BITS(sizeof(uint64_t))) {
      ptr = (uint8_t *) malloc(BITS_TO_BYTES(ammo_count));
    }
  }

  ~WeaponMask() {
    if (ammo_count > BYTES_TO_BITS(sizeof(uint64_t)))
      free(ptr);
  }

  WeaponMask(WeaponMask &other) = delete;

  WeaponMask &operator=(const WeaponMask &other) = delete;

  WeaponMask(WeaponMask &&other) noexcept {
    this->ammo_count = other.ammo_count;
    this->raw = other.raw;
    other.raw = 0;
    other.ammo_count = 0;
  };

  WeaponMask &operator=(WeaponMask &&other) noexcept {
    this->ammo_count = other.ammo_count;
    this->raw = other.raw;
    other.raw = 0;
    other.ammo_count = 0;
    return *this;
  }


  bool operator==(const WeaponMask &other) const {
    return this->weapon_index == other.weapon_index &&
           this->ammo_count == other.ammo_count &&
           memcmp(get_mask_c(), other.get_mask_c(), BYTES_TO_BITS(ammo_count)) == 0;
  };
};

class WeaponsMask {
public:
  std::vector<WeaponMask> weapons;

  bool operator==(const WeaponsMask &other) const = default;
};
}


#endif //WTFILEUTILS_TYPES_H
