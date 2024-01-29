#include <stdlib.h>
#include <ncurses.h>

int main() {
    initscr();
    printw(":: myHTOP ::\nConstruction en cours...");
    refresh();
    getch();
    endwin();

    return 0;
}