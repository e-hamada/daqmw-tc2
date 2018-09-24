#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int usage(void)
{
    fprintf(stderr, "Usage: sample filename\n");

    return 0;
}

int main(int argc, char *argv[])
{
    size_t n;
    char buf[1024];
    
    if (argc != 2) {
        usage();
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        err(EXIT_FAILURE, "fopen error");
    }
    
    for ( ; ; ) {
        n = fread(buf, 1 /*byte*/, sizeof(buf), fp);
        if (n == 0) {
            if (feof(fp)) {
                break;
            }
            else if (ferror(fp)) {
                fprintf(stderr, "fread error\n");
                exit(EXIT_FAILURE);
            }
            else {
                fprintf(stderr, "fread unkonwn error\n");
                exit(EXIT_FAILURE);
            }
        }
        /* fread returns successfully */
        if (n != sizeof(buf)) {
            // use long cast to run both 32bit and 64bit system
            fprintf(stderr, "paritial read: %ld bytes (should be %ld bytes)\n",
                (long) n, (long) sizeof(buf));
        }
        /* do something */
    }
    return 0;
}
