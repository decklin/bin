/*
 * To build: cc -lncurses -o bpm bpm.c
 *
 * Tap any key except ESC for a beat. ESC will reset if counting and
 * exit if already reset.
 */

#include <unistd.h>
#include <sys/times.h>
#include <curses.h>

#define ESC 27

int main() {
    long cur, last;
    float bpm;
    struct tms t;

    initscr();
    cbreak();
    noecho();

    while (!isendwin()) {
        cur = last = 0;
        while (1) {
            if (last) {
                bpm = 60.0 * sysconf(_SC_CLK_TCK) / (cur - last);
                mvprintw(0, 0, "%.2f BPM\n", bpm);
            } else {
                mvprintw(0, 0, "%s...\n", cur ? "Starting" : "Ready");
            }
            if (getch() == ESC) {
                if (!cur) endwin();
                break;
            } else {
                last = cur;
                cur = times(&t);
            }
        }
    }
}
