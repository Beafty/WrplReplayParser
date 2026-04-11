#include "state/ParserState.h"
#include "mpi/PositionSync.h"
#include "Unit.h"
#include "danet/dag_netUtils.h"

bool separateServerSideDetection_g = true;

bool SensorsControlStates::deserialize(BitStream *bs) {
  ZoneScoped;
  first_bool = bs->ReadBit();
  uint8_t sensor_type;
  bs->Read(sensor_type);
  sensor_type_maybe = sensor_type;
  sensor_type >>= 4;
  switch (sensor_type) {
    case 1: {
      ZoneScopedN("Took case 1")
      bool bool_2 = bs->ReadBit();
      if (!bool_2)
        return true;
      int16_t bit_packed;
      int16_t packed_val_1;
      int16_t packed_val_2;
      int16_t packed_val_3;
      bs->Read(bit_packed);
      field136_0x88 = (uint8_t) bit_packed & 0xf;
      field137_0x89 = (uint8_t) (bit_packed >> 4) & 0x3f;
      field138_0x8a = (uint8_t) (bit_packed >> 10) & 0xf;
      bs->Read(some_data_1);
      bs->Read(packed_val_1);
      bs->Read(packed_val_2);
      bs->Read(packed_val_3);
      some_data_2 = netutils::UNPACKS<int16_t>(packed_val_1, 1.0f);
      some_data_4 = netutils::UNPACKS<int16_t>(packed_val_2, PI);
      some_data_5 = netutils::UNPACKS<int16_t>(packed_val_2, PI);
      if (bit_packed < 0) {
        ZoneScopedN("Read some bit packed")
        bs->Read(some_data_6[0]);
      }
      bool bool_thing = bs->ReadBit();
      if (bool_thing) {
        ZoneScopedN("did bool thing")
        uint8_t bVar7;
        bs->Read(bVar7);
        this->some_data_6[1] = bVar7 & 1;
        this->field149_0xa4 = (int) ((char) (bVar7 << 5) >> 6);
        this->field150_0xa8 = (int) ((char) (bVar7 << 3) >> 6);
      }
      break;
    }
    case 2: {
      ZoneScopedN("Case 2")
      if (first_bool == 0) {
        TracyMessage("exited early", strlen("exited early") + 1);
        return true;
      }
      field136_0x88 = bs->ReadBit();
      int16_t v;
      bs->Read(v);
      some_data_1 = netutils::UNPACKS<int16_t>(v, PI);
      bs->ReadBits(reinterpret_cast<uint8_t *>(&some_data_2), 0x60);
      bs->ReadBits(reinterpret_cast<uint8_t *>(&some_data_5),
                   0x60); // ok for some fucking reason some_data_6 is a float here
      bs->Read(field147_0xa8);
      break;
    }
    case 3: {
      ZoneScopedN("Case 3")
      G_ASSERT(false); // uglugluglug
      break;
    }
    case 4: {
      ZoneScopedN("Case 4")
      bool stuff_is_read = bs->ReadBit();
      if (stuff_is_read) {
        ZoneScopedN("Case 4 read data")
        bs->ReadBits(reinterpret_cast<uint8_t *>(&some_data_1), 0x60);
      }
      break;
    }
  }
  bool stores_vec_thang = bs->ReadBit();
  if (stores_vec_thang) {
    ZoneScopedN("Reading weird vec data")
    bs->ReadBits(&field132_0x84, 6);
    field4_0x4.resize(field132_0x84);
    for (int i = 0; i < field132_0x84; i++) {
      bs->Read(field4_0x4[i]);
    }
    bs->ReadBits(&field133_0x85, 6);
  }
  return true;
}



bool TargetDesignationControlState::deserialize(BitStream *bs) {
  ZoneScoped;
  bs->Read(v1);
  bs->Read(v2);
  v3 = bs->ReadBit();
  if (v3) {
    ZoneScopedN("Read v3 data")
    bs->Read(v4);
  }
  write_compressed = bs->ReadBit();
  if (write_compressed) {
    ZoneScopedN("compressed vector read")
    bs->Read(v6);
    netutils::read_vector(*bs, v9, 4000.0f);
  } else {
    ZoneScopedN("vector read")
    bs->Read(v7);
    bs->Read(v9);
  }
  bs->Read(v5);
  v8 = bs->ReadBit();
  v10 = bs->ReadBit();
  auto read_some_float = bs->ReadBit();
  if (read_some_float) {
    ZoneScopedN("read some float")
    uint8_t temp;
    bs->Read(temp);
    v11 = netutils::UNPACK<uint8_t>(temp, 1);
  }
  v12 = bs->ReadBit();
  bool has_v14 = bs->ReadBit();
  v13 = bs->ReadBit();
  if (has_v14) {
    ZoneScopedN("Reading v14")
    bs->Read(v14);
  } else
    v14 = 0;
  bool has_v15 = bs->ReadBit();
  if (has_v15) {
    bs->Read(v15);
  }
  return true;
}

G_STATIC_ASSERT(sizeof(TargetDesignationControlState) == 0x50);


void DeserializeSeekerData(BitStream *bs) {
  // this will probably never be implemented unless if I actually go through and try to understand what this complex bullshit is
  ZoneScoped
  uint32_t val;
  bs->Read(val);
  auto read_offs = bs->GetReadOffset();
  bs->SetReadOffset(read_offs + val);
  //bool does_read_call_one = bs->ReadBit();

  return;
}

bool FMSync(ParserState *state, BitStream *bs) {
  static int curr_aircraft_index = 0;
  uint16_t uid = 0;
  do {
    bool uid_serialized = bs->ReadBit();
    if (uid_serialized) {
      bs->ReadCompressed(uid);
      uid &= 0x7FF;
    } else {
      uid += 1;
    }
    if (uid == 0x7FF)
      break;
    bool has_data = bs->ReadBit();
    if (!has_data) {
      ZoneScopedN("Parsing aircraft")
      auto str = fmt::format("uid: {:#x}", uid);
      TracyMessage(str.c_str(), str.size() + 1);
      auto offs = bs->GetReadOffset();
      //LOG("yes to data");
      if (separateServerSideDetection_g == false) {
        G_ASSERT(false); // be ready for if this ever happens
      } else {
        bool lVar30lessThan0 = bs->ReadBit();
        bool cVar4NE0 = bs->ReadBit();
        // if both true, then we do nothing
        if (!(lVar30lessThan0 && cVar4NE0)) {
          auto unit_ref = state->g_entity_mgr.uid_unit_ref_lookup[uid];
          if(!unit_ref) return false;
          auto unit = unit_ref->unit->AsAircraft();
          if(!unit) return false;
          bool fVar41LessThan0p5 = bs->ReadBit();
          uint32_t some_uint;
          bs->Read(some_uint);
          bool _0x7330_val = bs->ReadBit();
          bool v1 = bs->ReadBit();
          if (v1) {
            bool v2 = bs->ReadBit();
            bool unk1 = bs->ReadBit();
            bool unk2 = bs->ReadBit();
            uint8_t count = 0;
            bs->ReadBits(&count, 4);
            std::vector<bool> temp_bits{};
            temp_bits.reserve(count);
            for (int i = 0; i < count; i++) {
              temp_bits.push_back(bs->ReadBit());
            }
          }
          int zig_val;
          bs->ReadZigZag(zig_val);
          if(zig_val < 0) { // no negatives
            return false;
          }
          std::vector<uint32_t> vals;
          vals.resize(zig_val);
          for (int i = 0; i < zig_val; i++) {
            bs->ReadCompressed(vals[i]);
          }
          float positions[3];
          uint32_t some_packed_val;
          uint32_t vals_4;
          SpaceTime st{state->curr_time_ms, {}};
          bs->Read(st.location);
          unit->positions.push_back(st);
          bs->Read(some_packed_val);
          bs->Read(vals_4);
          uint64_t val_5; // holds lots of important data, more like a char[7] instead of a uint64_t, data is read as bytes
          bs->AlignReadToByteBoundary();
          bs->ReadBits(reinterpret_cast<uint8_t *>(&val_5), 0x38);
          uint8_t number_of_engines;
          bs->Read(number_of_engines);
          if (number_of_engines > 0xf) return false; // "FMsync: numEngines >= MAX_AIRCRAFT_MOTORS"
          for (int i = 0; i < number_of_engines; i++) {
            bool is_some_data_serialized = bs->ReadBit();
            uint8_t v;
            bs->Read(v);
            float probably_engine_power = -1;
            float packed_v_2 = -1;
            if (is_some_data_serialized) {
              int16_t temp_packed;
              bs->Read(temp_packed);
              probably_engine_power = netutils::UNPACKS<int16_t>(temp_packed, 1.1);
            }

            auto is_some_packed_2 = bs->ReadBit();
            if (is_some_packed_2) {
              int8_t v_;
              bs->Read(v_);
              packed_v_2 = netutils::UNPACKS<int8_t>(v_, 1.0);
            }

            uint8_t v10;
            uint8_t v11;
            bs->Read(v10);
            bs->Read(v11);
            float v10f = netutils::UNPACK<uint8_t>(v10, 1.0);
            float v11f = netutils::UNPACK<uint8_t>(v11, 1.0);
          }
          uint8_t sensorsCount;
          bs->Read(sensorsCount);
          G_ASSERT(sensorsCount <= 4);
          std::vector<SensorsControlStates> sensors{sensorsCount};
          for (auto &s: sensors) {
            G_ASSERT(s.deserialize(bs));
          }
          uint8_t v;
          if (sensorsCount != 0) {
            bs->Read(v);
          }
          uint8_t targetsNum = 0;
          bs->ReadBits(&targetsNum, 4);
          G_ASSERT(targetsNum <= 8);
          std::vector<TargetDesignationControlState> targets{targetsNum};
          for (auto &t: targets) {
            t.deserialize(bs);
          }
          bool bit_thing = bs->ReadBit();
          if (bit_thing) {
            uint16_t vt_1;
            uint16_t vt_2;
            bs->Read(vt_1);
            bs->Read(vt_2);
          }
          bool bit_thing_2 = bs->ReadBit();
          if (bit_thing_2) {
            DeserializeSeekerData(bs);
          }
        }
        curr_aircraft_index++;
      }
    } else {
      //LOG("no aircraft data for {}", uid);
    }
    G_ASSERT(uid != 0);
  } while (uid != 0x7FF);
  float maybe_floats[3];
  bs->Read(maybe_floats);
  // some extra data here
  return true;
}