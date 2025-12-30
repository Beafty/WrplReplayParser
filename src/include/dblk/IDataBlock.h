

#ifndef MYEXTENSION_IDATABLOCK_H
#define MYEXTENSION_IDATABLOCK_H

#include "NameMap.h"
#include "memory"

class DataBlockOwned;
class DataBlockShared;



/**
 * Internal DataBlock interface, is the base class for DataBlockOwned and DataBlockShared
 * Not to be used externally
 *
 */
class IDataBlock {
  enum {
    TYPE_NONE = 0,
    TYPE_SHARED = 1,
    TYPE_OWNED = 2,
  } block_type;
  std::shared_ptr<NameMap> nm;
};


#endif //MYEXTENSION_IDATABLOCK_H
