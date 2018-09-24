/*
 *  0      1      2      3      4      5      6      7
 * +------+------+------+------+------+------+------+------+
 * | 0x01 | 0x23 | 0x45 | 0x67 | 0x89 | 0xab | 0xcd | 0xef |
 * +------+------+------+------+------+------+------+------+
 * <---------------------------><------------><------------>
 *              int                 short         short
 */

#include <sys/time.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    unsigned int   * int_p;
    unsigned short * short_p;
    unsigned int   x;
    unsigned short y;

    unsigned char buf[8];

    buf[0] = 0x01;
    buf[1] = 0x23;
    buf[2] = 0x45;
    buf[3] = 0x67;
    buf[4] = 0x89;
    buf[5] = 0xab;
    buf[6] = 0xcd;
    buf[7] = 0xef;

    int_p = (unsigned int *)&buf[0];
    x = *int_p;
    printf("x (buf[0, 3]): 0x%x (dec: %d)\n", x, x);

    short_p = (unsigned short *)&buf[4];
    y = *short_p;
    printf("y (buf[4, 5]): 0x%x (dec: %d)\n", y, y);

    short_p = (unsigned short *)&buf[6];
    y = *short_p;
    printf("y (buf[6, 7]): 0x%x (dec: %d)\n", y, y);

    return 0;
}
