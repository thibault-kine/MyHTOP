#include "../headers/get_proc.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

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

char* get_name(int pid) {
    char* res = malloc(sizeof(char) * MAX_BUFFER_SIZE);

    char path[MAX_BUFFER_SIZE];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);

    FILE* f = fopen(path, "r");
    if(f != NULL) {
        fgets(res, MAX_BUFFER_SIZE, f);
        fclose(f);
        res[strcspn(res, "\n")] = 0;
    }
    else {
        strcpy(res, "N/A");
    }

    return res;
}

