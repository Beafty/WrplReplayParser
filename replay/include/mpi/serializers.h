

#ifndef MYEXTENSION_SERIALIZERS_H
#define MYEXTENSION_SERIALIZERS_H
#include "reflection.h"
#include "DataBlock.h"
#include "ecs/entityId.h"

#define REPL_VER(x) \
  if (!(x))         \
    return false

namespace danet {
  struct Uid {
    uint64_t pid = 0;
    std::string PLayerName{};
  };

  struct RoundScore {
    uint32_t combined;
    std::vector<uint32_t> scores;
  };

  struct dummyForFootballStat {
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
  };

  struct Crew {
    ecs::entity_id_t e1;
    ecs::entity_id_t e2;
    uint8_t v1;
    uint8_t v2;
  };
  struct CrewUnitsList {
    std::vector<Crew> crew;
  };



  struct dummyForPlayerStat {
    uint16_t v1;
    uint16_t v2;
    uint16_t v3;
    uint16_t v4;
    uint16_t v5;
    uint16_t v6;
    uint16_t v7;
    uint16_t v8;
    uint16_t v9;
    uint16_t v10;
    uint16_t v11;
    uint16_t v12;
    uint32_t v13;
    uint32_t v14;
    uint16_t v15;
    uint16_t v16;
    uint16_t v17;
    uint16_t v18;
    uint32_t v19;
    uint16_t v20;
    uint16_t v21;
    uint16_t v22;
    uint16_t v23;
    uint16_t v24;
  };

  struct dummyForKillStreaksProgress {
    struct streak {
      uint8_t v1;
      bool v2;
      bool v3;
    };
    std::vector<streak> vals;
  };

  int DefaultCoder(DANET_ENCODER_SIGNATURE);

  int StringCoder(DANET_ENCODER_SIGNATURE);

  int TranslatedCoder(DANET_ENCODER_SIGNATURE);

  int boolCoder(DANET_ENCODER_SIGNATURE);

  int UidCoder(DANET_ENCODER_SIGNATURE);

  int DataBlockCoder(DANET_ENCODER_SIGNATURE);

  int uint64Coder(DANET_ENCODER_SIGNATURE);

  int uint32Coder(DANET_ENCODER_SIGNATURE);

  int uint16Coder(DANET_ENCODER_SIGNATURE);

  int uint8Coder(DANET_ENCODER_SIGNATURE);

  int ULEB64Coder(DANET_ENCODER_SIGNATURE);

  int ULEB32Coder(DANET_ENCODER_SIGNATURE);

  int dummyForSupportPlanesCoder(DANET_ENCODER_SIGNATURE);

  int dummyForFootballStatCoder(DANET_ENCODER_SIGNATURE);

  int RoundScoreCoder(DANET_ENCODER_SIGNATURE);

  int U8VectorCoder(DANET_ENCODER_SIGNATURE);

  int U16VectorCoder(DANET_ENCODER_SIGNATURE);

  int U32VectorCoder(DANET_ENCODER_SIGNATURE);

  int EntityIdCoder(DANET_ENCODER_SIGNATURE);

  int PlayerStatCoder(DANET_ENCODER_SIGNATURE);

  int dummyForKillStreaksProgressCoder(DANET_ENCODER_SIGNATURE);

  int dummyForCrewUnitsListCoder(DANET_ENCODER_SIGNATURE);

  struct Nothing {};
  template <>
  struct DefaultEncoderChooser<Nothing> {
    static constexpr reflection_var_encoder coder = DefaultCoder;
  };

  template <>
  struct DefaultEncoderChooser<std::string> {
    static constexpr reflection_var_encoder coder = StringCoder;
  };

  template <>
  struct DefaultEncoderChooser<bool> {
    static constexpr reflection_var_encoder coder = boolCoder;
  };

  template <>
  struct DefaultEncoderChooser<Uid> {
    static constexpr reflection_var_encoder coder = UidCoder;
  };

  template <>
  struct DefaultEncoderChooser<DataBlock> {
    static constexpr reflection_var_encoder coder = DataBlockCoder;
  };

  template <>
  struct DefaultEncoderChooser<uint64_t> {
    static constexpr reflection_var_encoder coder = uint64Coder;
  };

  template <>
  struct DefaultEncoderChooser<uint32_t> {
    static constexpr reflection_var_encoder coder = uint32Coder;
  };

  template <>
  struct DefaultEncoderChooser<uint16_t> {
    static constexpr reflection_var_encoder coder = uint16Coder;
  };

  template <>
  struct DefaultEncoderChooser<uint8_t> {
    static constexpr reflection_var_encoder coder = uint8Coder;
  };

  template <>
  struct DefaultEncoderChooser<std::array<ecs::EntityId, 20>> {
    static constexpr reflection_var_encoder coder = dummyForSupportPlanesCoder;
  };

  template <>
  struct DefaultEncoderChooser<dummyForFootballStat> {
    static constexpr reflection_var_encoder coder = dummyForFootballStatCoder;
  };

  template <>
  struct DefaultEncoderChooser<RoundScore> {
    static constexpr reflection_var_encoder coder = RoundScoreCoder;
  };

  template <>
  struct DefaultEncoderChooser<std::vector<uint8_t>> {
    static constexpr reflection_var_encoder coder = U8VectorCoder;
  };

  template <>
  struct DefaultEncoderChooser<std::vector<uint16_t>> {
    static constexpr reflection_var_encoder coder = U16VectorCoder;
  };

  template <>
  struct DefaultEncoderChooser<std::vector<uint32_t>> {
    static constexpr reflection_var_encoder coder = U32VectorCoder;
  };

  template <>
  struct DefaultEncoderChooser<ecs::EntityId> {
    static constexpr reflection_var_encoder coder = EntityIdCoder;
  };

  template <>
  struct DefaultEncoderChooser<CrewUnitsList> {
    static constexpr reflection_var_encoder coder = dummyForCrewUnitsListCoder;
  };

  template <>
  struct DefaultEncoderChooser<dummyForPlayerStat> {
    static constexpr reflection_var_encoder coder = PlayerStatCoder;
  };

  template <>
  struct DefaultEncoderChooser<dummyForKillStreaksProgress> {
    static constexpr reflection_var_encoder coder = dummyForKillStreaksProgressCoder;
  };









// #define DANET_ENCODER_SIGNATURE int op, ReflectionVarMeta *meta, const ReflectableObject *ro, BitStream *bs
}

#endif //MYEXTENSION_SERIALIZERS_H
