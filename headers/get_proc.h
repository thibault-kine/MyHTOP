#ifndef GET_PROC_H
#define GET_PROC_H
#include <float.h>

int* get_pids(int* length);

char* get_name(int pid);

typedef struct t_pdata {
    char* name;

    int pid;
    int ppid;
    
    char* state;
    double vsize;
    int threads;
} pdata;

pdata get_process_data(int pid);

#endif