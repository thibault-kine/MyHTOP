#ifndef GET_PROC_H
#define GET_PROC_H

#include "proc_types.h"
#include <float.h>

int* get_pids(int* length);

char* get_name(int pid);

typedef struct t_pdata pdata;

pdata get_data(int pid);

#endif