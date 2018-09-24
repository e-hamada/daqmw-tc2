#!/usr/bin/env python

import os
import sys
import time # for sleep
import struct

HEADER_LEN = 12

debug = True

def usage():
    print 'Usage: decode data_file'

def main():
    if len(sys.argv) != 2:
        usage()
        sys.exit(1)

    try:
        f = open(sys.argv[1], 'r')
    except IOError, e:
        sys.exit(e)

    while True:
        header_data = f.read(HEADER_LEN)
        if header_data == '':
            break

        h = struct.unpack('>BBBBII', header_data)
        type_and_one_event_size = h[0]
        n_ch                    = h[1]
        zero_0                  = h[2] # should be zero always
        zero_1                  = h[3] # should be zero always
        data_length             = h[4]
        trigger_count           = h[5]
        type           = (type_and_one_event_size & 0xf0) >> 4
        one_event_size = (type_and_one_event_size & 0x0f)

        # check the header
        if type != 0xf:
            sys.exit('type mismatch: %02x (should be 0xf)' % (type))

        window_size = data_length / one_event_size / n_ch
        for w in range(0, window_size):
            data = f.read(one_event_size*n_ch)
            # XXX: fixed format for event data (2 bytes)
            fmt = '>%dH' % (n_ch)
            d = struct.unpack(fmt, data)
            for v in d:
                print 'trg:', trigger_count, 'ch:', (v & 0xf000) >> 12, 'window:', w, 'data:', v & 0x0fff

if __name__ == '__main__':
    main()
