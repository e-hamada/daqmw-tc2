#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "RawDataPacket.h"

using namespace std;
int usage()
{
    cerr << "Usage: readfile filename" << std::endl;

    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    RawDataPacket r;
    char *filename;
    unsigned char buf[1024*1024];
    int n;

    if (argc != 2) {
        usage();
        exit(1);
    }

    filename = argv[1];
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        err(1, "fopen for %s", filename);
    }

    for ( ; ; ) {
        // Read Header Part
        n = fread(buf, 1, RawDataPacket::HEADER_SIZE, fp);
        if (n == 0) {
            if (feof(fp)) {
                break;;
            }
            else if (ferror(fp)) {
                exit(0);
            }
        }
        else if (n != RawDataPacket::HEADER_SIZE) {
            errx(1, "partial read %d bytes.  Should be %d bytes", n, RawDataPacket::HEADER_SIZE);
        }

        // Set header part to decode
        r.set_buf(buf, n);
        // Decode.  Verify Type
        if (! r.is_raw_data_packet()) {
            cout << "Not a RawDataPacket" << endl;
            exit(1);
        }

        // Get Data length
        int data_length   = r.get_data_length();
        //cout << "data_length:   " << data_length   << endl;

        // Read Data Part
        n = fread(&buf[RawDataPacket::HEADER_SIZE], 1, data_length, fp);
        if (n == 0) {
            if (feof(fp)) {
                break;;
            }
            else if (ferror(fp)) {
                exit(0);
            }
        }
        else if (n != data_length) {
            errx(1, "partial read %d bytes.  Should be %d bytes", n, data_length);
        }
            
        // Get window size, trigger count, number of channels
        int window_size   = r.get_window_size();
        int trigger_count = r.get_trigger_count();
        int n_ch          = r.get_num_of_ch();
        
        //cout << "window_size:   " << window_size   << endl;
        //cout << "trigger_count: " << trigger_count << endl;
        
        // Decode data
        for (int w = 0; w < window_size; w++) {
            for (int ch = 0; ch < n_ch; ch ++) {
                unsigned short data = r.get_data_at(ch, w);
                cout << "trg: "     << trigger_count;
                cout << " ch: "      << ch;
                cout << " window: " << w;
                cout << hex << " data: "   << data;
                cout << endl;
            }
        }
        r.reset_buf();
    }

    return 0;
}
