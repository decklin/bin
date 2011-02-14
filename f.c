#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LINE_MAX 2048
#define FIELD_MAX 256
#define DEFAULT_IFS " \t"

int main(int argc, char **argv)
{
    int idx[FIELD_MAX], nidx, nf, i, tmp;
    char *f[FIELD_MAX], buf[LINE_MAX], *sep, *nl;

    if (argc == 1) {
        fprintf(stderr, "usage: [IFS=sep] f N [N...]\n");
        exit(2);
    }

    for (i = 1, nidx = 0; i < argc; i++) {
        if ((tmp = strtol(argv[i], NULL, 0)) > 0) {
            if (nidx < FIELD_MAX) {
                idx[nidx++] = tmp - 1;
            } else {
                fprintf(stderr, "f: can only handle %d fields\n", FIELD_MAX);
                break;
            }
        } else {
            fprintf(stderr, "f: invalid field number: %s\n", argv[i]);
            exit(2);
        }
    }

    if ((sep = getenv("IFS")) == NULL)
        sep = DEFAULT_IFS;

    while (fgets(buf, sizeof buf, stdin) != NULL) {
        if ((nl = strchr(buf, '\n')) != NULL) {
            *nl = '\0';
        } else {
            fprintf(stderr, "f: truncating line at %d bytes\n", LINE_MAX);
            while ((tmp = getchar()) != EOF) {
                if (tmp != '\n')
                    break;
            }
        }

        nf = 0;
        f[nf] = strtok(buf, sep);
        while (f[nf] != NULL && nf < FIELD_MAX)
            f[++nf] = strtok(NULL, sep);

        for (i = 0; i < nidx; i++) {
            if (idx[i] < nf)
                printf("%s", f[idx[i]]);
            if (i < nidx - 1)
                printf("\t");
        }
        printf("\n");
    }

    return 0;
}
