

#ifndef MYEXTENSION_OBJECTDISPATCHER_H
#define MYEXTENSION_OBJECTDISPATCHER_H

#include "codegen/ReflIncludes.h"
#include "types.h"

struct ParserState;
namespace mpi
{
  IObject * ObjectDispatcher(ObjectID oid, ObjectExtUID extUid,  ParserState*state);

}



#endif //MYEXTENSION_OBJECTDISPATCHER_H
