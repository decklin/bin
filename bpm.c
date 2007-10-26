#include <unistd.h>
#include <sys/times.h>
#include <curses.h>

int main()
{
    long ticks, start, beats;
    float dur;
    struct tms now;

    initscr();
    cbreak();
    noecho();

    ticks = sysconf(_SC_CLK_TCK);
    start = beats = 0;

    while (1) {
        if (beats) {
            dur = (times(&now) - start) / (float) ticks;
            mvprintw(0, 0, "%.2f bpm (%d, %.2fs)\n", beats * 60 / dur,
                beats, dur);
        } else {
            if (start) mvprintw(0, 0, "Counting (ESC to reset)\n");
            else mvprintw(0, 0, "Tap any key to begin...\n");
        }
        if (getch() == 27) {
            start = beats = 0;
        } else {
            if (start) beats++;
            else start = times(&now);
        }
    }
}
