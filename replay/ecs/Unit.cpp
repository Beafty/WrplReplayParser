
#include "Unit.h"

namespace unit {
  Tank *Unit::AsTank() {
    if(unitType == TankType)
      return (Tank*)this;
    return nullptr;
  }

  Aircraft *Unit::AsAircraft() {
    if(unitType == AircraftType)
      return (Aircraft*)this;
    return nullptr;
  }
}

