

#ifndef MYEXTENSION_OBJECTDISPATCHER_H
#define MYEXTENSION_OBJECTDISPATCHER_H

#include "codegen/ReflIncludes.h"
#include "types.h"

DEFINE_HANDLE(handle_object_dispatcher)
#define DISPATCHER_LOGI(format_, ...) ELOGI(handle_object_dispatcher, format_, __VA_ARGS__)
#define DISPATCHER_LOGD1(format_, ...) ELOGD1(handle_object_dispatcher, format_, __VA_ARGS__)
#define DISPATCHER_LOGD2(format_, ...) ELOGD2(handle_object_dispatcher, format_, __VA_ARGS__)
#define DISPATCHER_LOGD3(format_, ...) ELOGD3(handle_object_dispatcher, format_, __VA_ARGS__)
#define DISPATCHER_LOGE(format_, ...) ELOGE(handle_object_dispatcher, format_, __VA_ARGS__)

struct ParserState;
namespace mpi
{
  IObject * ObjectDispatcher(ObjectID oid, ObjectExtUID extUid,  ParserState*state);

}



#endif //MYEXTENSION_OBJECTDISPATCHER_H
