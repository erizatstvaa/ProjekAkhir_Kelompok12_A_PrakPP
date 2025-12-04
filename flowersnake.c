#include <curses.h>
#include <unistd.h>

int main() {
    int x = 10, y = 5;  
    int ch;

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); 
    nodelay(stdscr, TRUE); 

    // Draw border arena
    for (int i = 0; i < 50; i++) {
        mvprintw(0, i, "#");
        mvprintw(20, i, "#");
    }
    for (int i = 0; i <= 20; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, 49, "#");
    }

    while (1) {
        ch = getch();

        // kontrol movement
        if (ch == KEY_UP || ch == 'w') y--;
        else if (ch == KEY_DOWN || ch == 's') y++;
        else if (ch == KEY_LEFT || ch == 'a') x--;
        else if (ch == KEY_RIGHT || ch == 'd') x++;
        else if (ch == 'q') break; // keluar

        clear();

        // redraw border
        for (int i = 0; i < 50; i++) {
            mvprintw(0, i, "#");
            mvprintw(20, i, "#");
        }
        for (int i = 0; i <= 20; i++) {
            mvprintw(i, 0, "#");
            mvprintw(i, 49, "#");
        }

        // snake head
        mvprintw(y, x, "🌸");

        refresh();
        usleep(80000); 
    }

    endwin();
    return 0;
}
