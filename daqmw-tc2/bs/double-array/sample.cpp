#include <cstdio>
#include <fstream>
#include <iostream>

int fill_data(unsigned short *data_at_ch_window, int ch, int w)
{
    data_at_ch_window[w] = ch * 256 + w;
    return 0;
}

int main(int argc, char *argv[])
{
    const static int n_window_size = 10;
    const static int n_ch = 8;
    unsigned short data[n_ch][n_window_size];
    
    for (int ch = 0; ch < n_ch; ch ++) {
        printf("value of data[%d]: %p\n", ch, data[ch]);
        for (int w = 0; w < n_window_size; w++) {
            printf("address of data[%d][%d]: %p\n", ch, w, &data[ch][w]);
        }
    }

    for (int ch = 0; ch < n_ch; ch ++) {
        for (int w = 0; w < n_window_size; w++) {
            fill_data(data[ch], ch, w);
        }
    }
    for (int ch = 0; ch < n_ch; ch ++) {
        for (int w = 0; w < n_window_size; w++) {
            printf("ch: %d window: %d data: 0x%04x\n", ch, w, data[ch][w]);
        }
    }
    return 0;
}
