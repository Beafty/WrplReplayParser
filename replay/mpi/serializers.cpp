#include "mpi/serializers.h"
#include "math/dag_mathBase.h"

namespace danet {


  int TranslatedCoder(DANET_ENCODER_SIGNATURE) {
    return stringCoder(op, meta, ro, bs);
  }

  int WeirdFloatSerializer(int op, ReflectionVarMeta *meta, const ReflectableObject *ro, BitStream *bs) {
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
}