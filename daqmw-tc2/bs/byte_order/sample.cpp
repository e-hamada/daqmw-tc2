#include <cstdio>
#include <fstream>
#include <iostream>

#include <arpa/inet.h>

union my_num {
    int num;
    unsigned char buf[4];
};

int main(int argc, char *argv[])
{
    
    my_num x, y;

    x.num = 0x12345678;
    y.num = htonl(x.num);
    
    printf("0x%x\n", x.num);

    for (unsigned int i = 0; i < sizeof(x.num); i++) {
        printf("x: %p %d 0x%x\n", &x.buf[i], i, x.buf[i]);
    }

    for (unsigned int i = 0; i < sizeof(y.num); i++) {
        printf("y: %p %d 0x%x\n", &y.buf[i], i, y.buf[i]);
    }
    

    return 0;
}
