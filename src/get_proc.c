#include "../headers/get_proc.h"
#include "../headers/proc_types.h"
#include "../headers/trim.h"
#include "../headers/split.h"

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <float.h>

#define MAX_BUFFER_SIZE 256

/**
 * Renvoie une liste de tous les PIDs
 * @param length La longueur dont vous voulez attribuer la liste
 * @return Une liste de tous les PIDs
*/
int* get_pids(int* length) {

    DIR* dir;
    struct dirent *entry;
    int* res = malloc(sizeof(int) * MAX_BUFFER_SIZE);

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
            res[i] = pid;
            i++;
        }
    }

    closedir(dir);

    *length = i;
    return res;
}

/**
 * @return Le nom du processus
*/
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

typedef struct t_pdata {
    char* name;

    int pid;
    int ppid;
    
    char* state;
    double vsize;
    int threads;
} pdata;

pdata get_data(int pid) {
    pdata res;

    char path[MAX_BUFFER_SIZE];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE* f = fopen(path, "r");
    // FILE* out = fopen("output.txt", "w");
    if(f == NULL) {
        fprintf(stderr, "Error: Cannot load file %s\n", path);
        exit(EXIT_FAILURE);
    }

    char* content = malloc(sizeof(char) * 1024);

    int i = 0;
    char c;
    while((c = fgetc(f)) != EOF) {
        if(i >= 1024 - 1) {
            fprintf(stderr, "Error: File too large\n");
            exit(EXIT_FAILURE); 
        }
        // fputc(c, out);
        content[i] = c;
        i++;
    }
    content[i] = '\0';

    fclose(f);
    // fclose(out);

    char** words = split(content);
    for(int j = 0; words[j] != NULL; j++) {
        res.name = words[1];
        res.pid = atoi(words[0]);
        res.ppid = atoi(words[3]);
        res.state = words[2];
        res.threads = atoi(words[19]);
        res.vsize = atoi(words[22]);
    }

    return res;
}