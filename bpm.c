/*
 * To build: cc -lncurses -o bpm bpm.c
 *
 * Tap any key except ESC for a beat. ESC will reset if counting and
 * exit if already reset. Pausing for more than RESET_TIME seconds will
 * reset on the next tap.
 */

#include <unistd.h>
#include <sys/time.h>
#include <curses.h>

#define ESC 27
#define MAX_WEIGHT 0.9
#define RESET_TIME 2.0

int main() {
    double cur, last, bpm, avg, weight;
    struct timeval now;

    initscr();
    cbreak();
    noecho();

    while (!isendwin()) {
        cur = last = avg = weight = 0;
        while (1) {
            if (last) {
                bpm = 60 / (cur - last);
                avg = avg * weight + bpm * (1 - weight);
                weight = (MAX_WEIGHT + weight) / 2;
                mvprintw(0, 0, "%.2f BPM\n", avg);
            } else {
                mvprintw(0, 0, "%s...\n", cur ? "Starting" : "Ready");
            }
            if (getch() == ESC) {
                if (!cur) endwin();
                break;
            } else {
                last = cur;
                gettimeofday(&now, NULL);
                cur = now.tv_sec + now.tv_usec / 1000000.0;
                if (last && cur - last > RESET_TIME) last = 0;
            }
        }
    }
}
