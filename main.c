#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>

int main() {

    DIR* dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if(dir == NULL) {
        perror("Cannot open /proc");
        return 1;
    }

    WINDOW *main, *header, *body;
    initscr();

    const char* TITLE = ":: myHTOP v0.1 ::";
    const char* SUB = "Construction en cours...";

    const double HEADER_W = strlen(SUB) + 6;

    main = subwin(stdscr, LINES, COLS, 0, 0);
    header = subwin(main, 5, HEADER_W, 0, (COLS - HEADER_W) / 2);
    body = subwin(main, LINES - 5, COLS, 5, 0);

    mvwprintw(header, 1, (HEADER_W - strlen(TITLE)) / 2, "%s", TITLE);
    mvwprintw(header, 3, (HEADER_W - strlen(SUB)) / 2, "%s", SUB);

    mvwprintw(body, 1, 1, "PID");

    int i = 2;
    while((entry = readdir(dir)) != NULL) {
        int pid = atoi(entry->d_name);
        if(pid != 0) {
            mvwprintw(body, i, 1, "%d", pid);
            i++;
        }
    }

    box(header, ACS_VLINE, ACS_HLINE);
    box(body, ACS_VLINE, ACS_HLINE);

    wrefresh(main);
    wrefresh(header);
    wrefresh(body);

    getch();
    endwin();

    free(main);
    free(header);
    free(body);

    return 0;
}