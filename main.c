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
    WINDOW *w_pid, *wl_pid /*, *w_user, *w_cpu, *w_mem, *w_time */; 
    initscr();
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    // MISC FOR HEADER
    const char* TITLE = ":: myHTOP v0.1 ::";
    const char* SUB[] = {
        "Under development, come back later...",
        "Inside of it, there's C. Inside of me, there's pain.",
        "Welcome home, chosen undead.",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        "Homebrewed Task Manager",
        "Hi Rodolphe ! :wave:",
        "NOT Copyrighted Thibault Kine",
        "ZA WARUDO! Toki wo tomare!",
        "The numbers, Mason! What do they mean?"
    };
    srand(time(NULL));
    int arr_len = sizeof(SUB) / sizeof(SUB[0]);
    int rand_id = 0 + rand() % (arr_len + 1 - 0);
    const double HEADER_W = strlen(SUB[rand_id]) + 6;

    // CREATE SUBWINDOWS
    w_main = subwin(stdscr, LINES, COLS, 0, 0);
    w_header = subwin(w_main, 5, HEADER_W, 0, (COLS - HEADER_W) / 2);
    w_body = subwin(w_main, LINES - 5, COLS, 5, 0);

    w_pid = subwin(w_body, LINES - 5, COLS / 6, 5, 1);
    wl_pid = subwin(w_body, 1, (COLS / 6) - 2, 6, 2);

    // PRINT THE TITLE + SUBTITLE
    mvwprintw(w_header, 1, (HEADER_W - strlen(TITLE)) / 2, "%s", TITLE);
    mvwprintw(w_header, 3, (HEADER_W - strlen(SUB[rand_id])) / 2, "%s", SUB[rand_id]);

    // PRINT INFOS
    mvwprintw(wl_pid, 0, 2, "PID");

    // DISPLAY A LIST OF PIDs
    // TODO: put a limit so it doesn't go further than the border
    int i = 2;
    while((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if(pid != 0) {
            mvwprintw(w_pid, i, 2, "%d", pid);
            i++;
        }
    }

    // DISPLAY THE WINDOW BORDERS
    box(w_header, ACS_VLINE, ACS_HLINE);
    box(w_pid, ACS_VLINE, ACS_HLINE);

    // COLOR THE SUBWINS
    wbkgd(wl_pid, COLOR_PAIR(1));

    // REFRESH THE WINDOWS
    wrefresh(w_main);
    wrefresh(w_header);
    wrefresh(w_body);
    wrefresh(wl_pid);

    getch();
    endwin();

    free(w_main);
    free(w_header);
    free(w_body);

    return 0;
}