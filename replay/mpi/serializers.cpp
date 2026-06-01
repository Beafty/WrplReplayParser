#include "mpi/serializers.h"
#include "math/dag_mathBase.h"

namespace danet {


  int TranslatedCoder(DANET_ENCODER_SIGNATURE) {
    return stringCoder(op, meta, ro, bs, state);
  }

  int WeirdFloatSerializer(DANET_ENCODER_SIGNATURE) {
    auto data = meta->getValue<uint8_t>(); // in reality, this maybe 2, 3, or even 4 floats
    G_ASSERT(meta->numBits%0x20 == 0 && meta->numBits <= 0x80); // must be alligned to 4 bytes and be less than 16 bytes (float and max 4 floats)
    if (op == DANET_REFLECTION_OP_ENCODE) {
      bs->WriteBits(data, meta->numBits);
      return true;
    }
    else if (op == DANET_REFLECTION_OP_DECODE) {
      REPL_VER(bs->ReadBits(data, meta->numBits));
      return true;
    }
    return false; // TODO, this can do compact serialization and its scary, figure out when that will occur?
  }

  int InvalidSerializer(DANET_ENCODER_SIGNATURE) {
    return 2;
  }

  int WeaponsCoder(DANET_ENCODER_SIGNATURE) {
    auto data = meta->getValue<danet::WeaponsMask>();
    if (op == DANET_REFLECTION_OP_ENCODE) {
      for (auto &weapon: data->weapons) {
        bs->WriteCompressed(weapon.get_weapon_index());
        bs->WriteCompressed(weapon.get_num_weapons());
        bs->WriteBits(weapon.get_mask_c(), weapon.get_num_weapons());
      }
      uint16_t end_idx = 0xFFFF;
      bs->WriteCompressed(end_idx);
      return true;
    } else if (op == DANET_REFLECTION_OP_DECODE) {
      while (true) {
        uint16_t weap_index, weap_count;
        REPL_VER(bs->ReadCompressed(weap_index));
        if (weap_index == 0xFFFF)
          break;
        REPL_VER(bs->ReadCompressed(weap_count));
        auto &back = data->weapons.emplace_back(weap_count, weap_index);
        REPL_VER(bs->ReadBits(back.get_mask(), weap_count));
      }
      return true;
    }
    return false;
  }
}
