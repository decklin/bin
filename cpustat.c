#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/stat.h>

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
