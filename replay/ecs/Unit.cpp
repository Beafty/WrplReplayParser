
#include "Unit.h"

namespace unit {
  Tank *Unit::isTank() {
    if(unitType == TankType)
      return (Tank*)this;
    return nullptr;
  }

  Aircraft *Unit::isAircraft() {
    if(unitType == AircraftType)
      return (Aircraft*)this;
    return nullptr;
  }
}

