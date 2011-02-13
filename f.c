#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LINE_BUF 2048
#define MAX_FIELDS 256
#define DEFAULT_IFS " \t\n"

int main(int argc, char **argv)
{
    int idx[MAX_FIELDS], nidx;
    char *f[MAX_FIELDS]; int nf;
    char buf[LINE_BUF], *sep, *w, *nl;
    int i, a;

    if (argc == 1) {
        fprintf(stderr, "usage: f N [N...]\n");
        exit(2);
    }

    if (argc > MAX_FIELDS + 1) {
        fprintf(stderr, "error: more than %d fields specified\n", MAX_FIELDS);
        exit(2);
    }

    for (i = 1, nidx = 0; i < argc; i++) {
        if ((a = strtol(argv[i], NULL, 10)) > 0) {
            idx[nidx++] = a - 1;
        } else {
            fprintf(stderr, "error: cannot parse number: %s\n", argv[i]);
            exit(2);
        }
    }

    if ((sep = getenv("IFS")) == NULL)
        sep = DEFAULT_IFS;

    while (fgets(buf, sizeof buf, stdin)) {
        if ((nl = strstr(buf, "\n")) != NULL)
            *nl = '\0';
        else
            fprintf(stderr, "warning: line truncated to %ld bytes\n", sizeof buf);
        for (nf = 0, w = strtok(buf, sep); w && nf < MAX_FIELDS; w = strtok(NULL, sep))
            f[nf++] = w;
        for (i = 0; i < nidx; i++)
            printf("%s%s", idx[i] < nf ? f[idx[i]] : "", i < nidx - 1 ? "\t" : "\n");
    }

    return 0;
}
