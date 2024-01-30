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

int main() {

    DIR* dir;
    struct dirent *entry;

    // GET ALL PROCS
    dir = opendir("/proc");
    if(dir == NULL) {
        perror("Cannot open /proc");
        return 1;
    }

    WINDOW *w_main, *w_header, *w_body;
    WINDOW *w_pid /*, *w_user, *w_status, *w_cpu, *w_mem, *w_time, *w_name */; 
    WINDOW *wl_pid, *wl_user, *wl_status, *wl_cpu, *wl_mem, *wl_time, *wl_name;
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
    int arr_len = sizeof(SUB) / sizeof(SUB[0]);
    int rand_id = 0 + rand() % (arr_len + 1 - 0);
    const double HEADER_W = strlen(SUB[rand_id]) + 6;

    // CREATE SUBWINDOWS
    w_main = subwin(stdscr, LINES, COLS, 0, 0);
    w_header = subwin(w_main, 5, HEADER_W, 0, (COLS - HEADER_W) / 2);
    w_body = subwin(w_main, LINES - 5, COLS, 5, 0);

    int remaining_w;
    int total_w = COLS - 1;
    int used_w = 7 * ((COLS / 10) - 2);
    remaining_w = total_w - used_w;

    w_pid = subwin(w_body, LINES - 5, COLS / 10, 5, 0);

    wl_pid = subwin(w_body, 1, (COLS / 10) - 1, 6, 1);
    wl_user = subwin(w_body, 1, (COLS / 10) - 1, 6, (COLS / 10) + 1);
    wl_status = subwin(w_body, 1, (COLS / 10) - 1, 6, ((COLS / 10) * 2) + 1);
    wl_cpu = subwin(w_body, 1, (COLS / 10) - 1, 6, ((COLS / 10) * 3) + 1);
    wl_mem = subwin(w_body, 1, (COLS / 10) - 1, 6, ((COLS / 10) * 4) + 1);
    wl_time = subwin(w_body, 1, (COLS / 10) - 1, 6, ((COLS / 10) * 5) + 1);
    wl_name = subwin(w_body, 1, remaining_w, 6, ((COLS / 10) * 6) + 1);

    // PRINT THE TITLE + SUBTITLE
    mvwprintw(w_header, 1, (HEADER_W - strlen(TITLE)) / 2, "%s", TITLE);
    mvwprintw(w_header, 3, (HEADER_W - strlen(SUB[rand_id])) / 2, "%s", SUB[rand_id]);

    // PRINT LABELS
    mvwprintw(wl_pid, 0, 1, "PID");
    mvwprintw(wl_user, 0, 1, "User");
    mvwprintw(wl_status, 0, 1, "Status");
    mvwprintw(wl_cpu, 0, 1, "CPU%%");
    mvwprintw(wl_mem, 0, 1, "Mem%%");
    mvwprintw(wl_time, 0, 1, "Time");
    mvwprintw(wl_name, 0, 1, "Name");

    // DISPLAY A LIST OF PIDs
    // TODO: put a limit so it doesn't go further than the border
    for(int i = 1; (entry = readdir(dir)) != NULL;) {
        int pid = atoi(entry->d_name);
        if(pid != 0) {
            mvwprintw(w_pid, i + 1, 2, "%d", pid);
            i++;
        }
    }

    // DISPLAY THE WINDOW BORDERS
    box(w_header, ACS_VLINE, ACS_HLINE);
    box(w_body, ACS_VLINE, ACS_HLINE);

    // COLOR THE SUBWINS
    wbkgd(wl_pid, COLOR_PAIR(1));
    wbkgd(wl_user, COLOR_PAIR(1));
    wbkgd(wl_status, COLOR_PAIR(1));
    wbkgd(wl_cpu, COLOR_PAIR(1));
    wbkgd(wl_mem, COLOR_PAIR(1));
    wbkgd(wl_time, COLOR_PAIR(1));
    wbkgd(wl_name, COLOR_PAIR(1));

    // REFRESH THE WINDOWS
    wrefresh(w_main);
    wrefresh(w_header);
    wrefresh(w_body);

    getch();
    endwin();

    free(w_main);
    free(w_header);
    free(w_body);

    return 0;
}