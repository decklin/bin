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
    long started, beats;
    float dur;
    struct tms t;

    initscr();
    cbreak();
    noecho();

    while (!isendwin()) {
        started = beats = 0;
        while (1) {
            if (beats) {
                dur = (times(&t) - started) / (float) sysconf(_SC_CLK_TCK);
                mvprintw(0, 0, "%.2f BPM\n", 60 * beats / dur);
            } else {
                mvprintw(0, 0, "%s...\n", started ? "Starting" : "Ready");
            }
            if (getch() == ESC) {
                if (!started) endwin();
                break;
            } else {
                if (started) beats++;
                else started = times(&t);
            }
        }
    }
}
