/**
 * NAMING CONVENTIONS
 * w_<name>     window
 * wl_<name>    window label
 * 
*/

#include <unistd.h>         // Universal library
#include <stdlib.h>         // Standard library
#include <stdio.h>          // Standard I/O library
#include <ncurses.h>        // To make windows in the console
#include <string.h>         // To handle strings
#include <sys/resource.h>   // To get resource usages
#include <time.h>           // To set rand() & srand()
#include <dirent.h>         // To explore the /proc directory

#include "headers/get_proc.h"

enum SortBy {
    PID,
    VSIZE
};

int main() {

    WINDOW *w_main, *w_header, *w_body;
    WINDOW *w_pid, *w_ppid, *w_state, *w_vmrss, *w_threads, *w_name; 
    WINDOW *wl_pid, *wl_ppid, *wl_state, *wl_vmrss, *wl_threads, *wl_name;
    initscr();
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    // MISC FOR HEADER
    const char* TITLE = ":: myHTOP v0.1 ::";
    const char* SUB[] = {
        "Under development, come back later...",
        "Inside of it, there's C. Inside of me, there's pain.",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "Homebrewed Task Manager",
        "Hi Rodolphe ! :wave:",
        "NOT Copyrighted Thibault Kine",
        "The numbers, Mason! What do they mean?",
    };
    srand(time(NULL));
    unsigned int arr_len = sizeof(SUB) / sizeof(SUB[0]);
    unsigned int rand_id = 0 + rand() % arr_len;
    double header_w = 0;
    if(rand_id < arr_len && SUB[rand_id] != NULL) {
        header_w = strlen(SUB[rand_id]) + 6;
    }
    else {
        header_w = 6;
    }

    // CREATE SUBWINDOWS
    w_main = subwin(stdscr, LINES, COLS, 0, 0);
    w_header = subwin(w_main, 5, header_w, 0, (COLS - header_w) / 2);
    w_body = subwin(w_main, LINES - 5, COLS, 5, 0);

    const double COL_NUM = COLS / 10;

    int remaining_w = 0;
    int total_w = COLS - 1;
    int used_w = 6 * (COL_NUM - 2);
    remaining_w = total_w - used_w;

    // subwin(window, h, w, ystart, xstart)
    w_pid = subwin(w_body, LINES - 5, COL_NUM, 5, 0);
    w_ppid = subwin(w_body, LINES - 5, (COL_NUM) + 1, 5, COL_NUM);
    w_state = subwin(w_body, LINES - 5, (COL_NUM) + 1, 5, (COL_NUM) * 2);
    w_vmrss = subwin(w_body, LINES - 5, (COL_NUM) + 1, 5, (COL_NUM) * 3);
    w_threads = subwin(w_body, LINES - 5, (COL_NUM) + 1, 5, (COL_NUM) * 4);
    w_name = subwin(w_body, LINES - 5, remaining_w + 4, 5, (COL_NUM) * 5);

    wl_pid = subwin(w_body, 1, (COL_NUM) - 1, 6, 1);
    wl_ppid = subwin(w_body, 1, (COL_NUM) - 1, 6, (COL_NUM) + 1);
    wl_state = subwin(w_body, 1, (COL_NUM) - 1, 6, ((COL_NUM) * 2) + 1);
    wl_vmrss = subwin(w_body, 1, (COL_NUM) - 1, 6, ((COL_NUM) * 3) + 1);
    wl_threads = subwin(w_body, 1, (COL_NUM) - 1, 6, ((COL_NUM) * 4) + 1);
    wl_name = subwin(w_body, 1, remaining_w + 2, 6, ((COL_NUM) * 5) + 1); 

    // PRINT THE TITLE + SUBTITLE
    mvwprintw(w_header, 1, (header_w - strlen(TITLE)) / 2, "%s", TITLE);
    mvwprintw(w_header, 3, (header_w - strlen(SUB[rand_id])) / 2, "%s", SUB[rand_id]);

    // PRINT LABELS
    mvwprintw(wl_pid, 0, 1, "PID");
    mvwprintw(wl_ppid, 0, 1, "PPID");
    mvwprintw(wl_state, 0, 1, "State");
    mvwprintw(wl_vmrss, 0, 1, "VmRSS");
    mvwprintw(wl_threads, 0, 1, "Threads");
    mvwprintw(wl_name, 0, 1, "Name");

    // DISPLAY A LIST OF PIDs
    // TODO: put a limit so it doesn't go further than the border
    int pid_length = 0;
    int* pids = get_pids(&pid_length);

    // For each PID
    if(pids != NULL) {
        for(int i = 0; pids[i]; i++) {
            pdata proc = get_process_data(pids[i]);
            mvwprintw(w_pid, i + 2, 2, "%d", proc.pid);
            mvwprintw(w_ppid, i + 2, 2, "%d", proc.ppid);
            mvwprintw(w_name, i + 2, 2, "%s", proc.name);
            mvwprintw(w_state, i + 2, 2, "%s", proc.state);
            mvwprintw(w_threads, i + 2, 2, "%d", proc.threads);

            double size_mb = proc.vsize / (1024 * 1024);
            mvwprintw(w_vmrss, i + 2, 2, "%.0fMb", size_mb);
        }
        free(pids);
    }

    // DISPLAY THE WINDOW BORDERS
    box(w_header, ACS_VLINE, ACS_HLINE);
    box(w_body, ACS_VLINE, ACS_HLINE);
    box(w_name, ACS_VLINE, ACS_HLINE);

    // COLOR THE SUBWINS
    wbkgd(wl_pid, COLOR_PAIR(1));
    wbkgd(wl_ppid, COLOR_PAIR(1));
    wbkgd(wl_state, COLOR_PAIR(1));
    wbkgd(wl_vmrss, COLOR_PAIR(1));
    wbkgd(wl_threads, COLOR_PAIR(1));
    wbkgd(wl_name, COLOR_PAIR(1));

    int sort = PID;

    bool is_running = true;
    while(is_running) {

        char key = getch();

        // [ESC] = quit
        if(key == 27) {
            is_running = false;
        }

        if(pids != NULL) {
            switch(sort) {

                case PID: {
                    for(int i = 0; pids[i]; i++) {
                        pdata proc = get_process_data(pids[i]);
                        mvwprintw(w_pid, i + 2, 2, "%d", proc.pid);
                        mvwprintw(w_ppid, i + 2, 2, "%d", proc.ppid);
                        mvwprintw(w_name, i + 2, 2, "%s", proc.name);
                        mvwprintw(w_state, i + 2, 2, "%s", proc.state);
                        mvwprintw(w_threads, i + 2, 2, "%d", proc.threads);

                        double size_mb = proc.vsize / (1024 * 1024);
                        mvwprintw(w_vmrss, i + 2, 2, "%.0fMb", size_mb);
                    }
                }
            }
            
            free(pids);
        }

        // [R] = refresh
        if(key == 'r') {
            wrefresh(w_pid);
            wrefresh(w_ppid);
            wrefresh(w_name);
            wrefresh(w_state);
            wrefresh(w_threads);
            wrefresh(w_vmrss);
        }

        // [S] = sort by
        if(key == 's') {
            if(sort == PID) sort = VSIZE;
            if(sort == VSIZE) sort = PID;
        }
    }

    free(w_main);
    free(w_header);
    free(w_body);

    endwin();

    system("clear");

    return 0;
}