#include "../headers/get_proc.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int* get_pids(int* length) {

    DIR* dir;
    struct dirent *entry;

    // GET ALL PROCS
    dir = opendir("/proc");
    if(dir == NULL) {
        perror("Cannot open /proc");
        return NULL;
    }

    int i = 0;
    while((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if(pid != 0) {
            i++;
        }
    }

    rewinddir(dir);

    int* res = malloc(sizeof(int) * i);
    if(res == NULL) {
        perror("Cannot allocate memory");
        return NULL;
    }

    int j = 0;
    while((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if(pid != 0) {
            res[j] = pid;
            j++;
        }
    }

    closedir(dir);

    *length = i;
    return res;
}