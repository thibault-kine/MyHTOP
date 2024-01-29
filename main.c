#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

int main() {

    WINDOW *main, *header;
    initscr();

    const char* TITLE = ":: myHTOP v0.1 ::";
    const char* SUB = "Construction en cours...";

    const double HEADER_W = strlen(SUB) + 6;

    main = subwin(stdscr, LINES, COLS, 0, 0);
    header = subwin(main, 5, HEADER_W, 0, (COLS - HEADER_W) / 2);
    mvwprintw(header, 1, (HEADER_W - strlen(TITLE)) / 2, "%s", TITLE);
    mvwprintw(header, 3, (HEADER_W - strlen(SUB)) / 2, "%s", SUB);

    box(header, ACS_VLINE, ACS_HLINE);
    wrefresh(header);
    getch();
    endwin();

    free(header);

    return 0;
}