//
// Data Format

// 31                                                          0
// +-----------------------------------------------------------+
// |                   Magic Word 0x12345678                   |
// +-----------------------------------------------------------+
// |                    Data Length (bytes)                    |
// +-----------------------------------------------------------+
// |                  Trigger Count (From 0 - )                |
// +----------+------------------+----------+------------------+
// | ch 0 (4) | sample_0 (12)    | ch 1 (4) | sample_0 (12)    |
// +----------+-----------------------------+------------------+
// | ch 2 (4) | sample_0 (12)    | ch 3 (4) | sample_0 (12)    |
// +----------+-----------------------------+------------------+
// | ch 0 (4) | sample_1 (12)    | ch 1 (4) | sample_1 (12)    |
// +----------+-----------------------------+------------------+
// | ch 2 (4) | sample_1 (12)    | ch 3 (4) | sample_1 (12)    |
// +----------+------------------+----------+------------------+
// :
// +----------+-----------------------------+------------------+
// | ch 0 (4) | sample_1023 (12) | ch 1 (4) | sample_1023 (12) |
// +----------+-----------------------------+------------------+
// | ch 2 (4) | sample_1023 (12) | ch 3 (4) | sample_1023 (12) |
// +----------+-----------------------------+------------------+
// |                      Footer 0x89ABCDEF                    |
// +-----------------------------------------------------------+
// struct raw_data {
//     unsigned int   magic_word;
//     unsigned int   data_length;
//     unsigned int   trigger_count;
//     unsigned short event_data[4*1024]; /* 4: number of channels.  1024: number of samples */
//     unsigned int   footer;
// };
// Byte order is network byte order
// total 8208 bytes / trigger
// ADC Data: Bit 15 - 12: channel number
//           Bit 11 -  0: Data

#ifndef _RAWDATAPACKET
#define _RAWDATAPACKET

#include <arpa/inet.h> // for ntohs(), ntohl()
#include <iostream>

class RawDataPacket
{
public:
    RawDataPacket();
    virtual ~RawDataPacket();
    int set_buf(const unsigned char *buf, int buf_len);
    bool is_valid_magic();
    bool is_valid_footer();
    int  get_word_size();
    int  get_num_of_ch();
    int  get_data_length();
    int  get_window_size();
    int  get_trigger_count();
    int  get_buf_pos();
    int  get_buf_len();
    unsigned int get_data_at(int ch, int window);
    int  reset_buf();
    const static int HEADER_SIZE = 12;
    const static int FOOTER_SIZE = 4;
    const static int MAGIC_POS  = 0;
    const static int LENGTH_POS  = 4;
    const static int TRIGGER_POS = 8;
    const static int FOOTER_POS  = 12 + 1024*4*2;

private:
    const unsigned char *m_buf;
    int m_buf_len;

};

#endif
