

#ifndef MYEXTENSION_INITIALIZE_H
#define MYEXTENSION_INITIALIZE_H
#include <string>
#include "mpi/mpi.h"
#include "ecs/EntityManager.h"
#include "utils.h"
#include "dag_assert.h"


void initialize(std::string &VromfsPath, bool fonts=false);


#endif //MYEXTENSION_INITIALIZE_H
