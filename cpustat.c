#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/stat.h>

/* In Linux, information in /proc about CPU utilization is provided as
 * the total number of jiffies that have ever been spent in various
 * states. It doesn't make sense to speak of "instantaneous" CPU
 * utilization, as this is always either 0% or 100%; instead, we
 * generally want to consider a given length of time, say 1 second into
 * the past, and count how much of that time was spent in each state.
 *
 * A consequence of this fact is that one cannot spawn a new process
 * and have it return some meaningful CPU utilization figure unless it
 * blocks while allowing enough time to elapse. In many situations this
 * is undesirable. One solution to this problem is to continually store
 * the relevant numbers of jiffies somewhere so that another process may
 * later calculate the difference between two points in time.
 *
 * Rather than saving this state externally at predetermined times,
 * this program creates a FIFO, then holds the last known values in
 * memory until another process reads from it. At that point, the values
 * are reread and the average CPU utilization since the preceding
 * read is calculated. Another process may then obtain useful figures
 * asynchronously every few seconds.
 *
 * USAGE: cpustat path/to/fifo. The FIFO will be created, and removed
 * upon exit. If a FIFO already exists there, it will be reused; if some
 * other file does, the program will exit with an error.
 *
 * DISCLAIMER: CPU utilization is not a meaningful indication of
 * system load. /proc/loadavg may be read, without blocking, for that.
 * The figures produced by this program are primarily appropriate for
 * understanding why or when cpufreq takes given actions, when one has
 * inadvertently coded an infinite loop, which tab in your web browser
 * is rendering inefficiently, etc.
 *
 * BUGS: This program does not read values for individual CPUs, since
 * the author has not found them to be of any interest for non-critical
 * applications. However, it could be trivially extended to do so. We
 * sleep after closing the FIFO to ensure that the reader also closes
 * it, since if we were able to open it again immediately, we would
 * produce meaningless output. It is therefore possible for readers to
 * pile up and overwhelm the system.
 *
 * Written 2007 by Decklin Foster <decklin@red-bean.com>; public domain.
 * ABSOLUTELY NO WARRANTY, to the extent permitted by applicable law. */

char *fifo_path;

clock_t read_cpu(clock_t *user, clock_t *nice, clock_t *sys)
{
    FILE *proc_stat;
    char buf[256];
    struct tms now;

    proc_stat = fopen("/proc/stat", "r");
    while (!feof(proc_stat)) {
        if (fgets(buf, sizeof buf, proc_stat) == NULL) {
            break;
        }
        if (strncmp(buf, "cpu ", 4) == 0) {
            sscanf(buf, "%*s %lu %lu %lu", user, nice, sys);
            break;
        }
    }
    fclose(proc_stat);

    return times(&now);
}

void cleanup(int signum)
{
    unlink(fifo_path);
    exit(0);
}

int main(int argc, char **argv)
{
    FILE *fifo;
    struct stat fs;
    clock_t tick, user, nice, sys, delta;
    clock_t tick_old, user_old, nice_old, sys_old;

    if (argc == 2) {
        fifo_path = argv[1];
    } else {
        fprintf(stderr, "Need name of FIFO to create.\n");
        return 2;
    }

    if (stat(fifo_path, &fs) == -1) {
        if (mkfifo(fifo_path, 0777) == -1) {
            perror("Creating pipe");
            return 3;
        }
    } else {
        if ((fs.st_mode & S_IFMT) != S_IFIFO) {
            fprintf(stderr, "Ceci ne pas un pipe\n");
            return 3;
        }
    }

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    tick_old = read_cpu(&user_old, &nice_old, &sys_old);
    for (;;) {
        fifo = fopen(fifo_path, "w");
        tick = read_cpu(&user, &nice, &sys);
        delta = tick - tick_old;
        fprintf(fifo, "%02.1f total %02.1f user %02.1f nice %02.1f sys\n",
            100.0 * (user+nice+sys-(user_old+nice_old+sys_old)) / delta,
            100.0 * (user-user_old) / delta,
            100.0 * (nice-nice_old) / delta,
            100.0 * (sys-sys_old) / delta);
        fclose(fifo);
        sleep(1);
        tick_old = tick;
        user_old = user;
        nice_old = nice;
        sys_old = sys;
    }
}
