//
// Data Format
// 31                              0
// +--------------------------------+
// |  (*)  | n_ch | unused | unused |
// +--------------------------------+
// |     Data  Length (data only)   |
// +--------------------------------+
// |          Trigger Count         |
// +--------------------------------+  <-- Window # 0
// |   ADC ch # 0   |  ADC ch # 1   |
// +--------------------------------+
// |   ADC ch # 2   |  ADC ch # 3   |
// +--------------------------------+
// |   ADC ch # 4   |  ADC ch # 5   |
// +--------------------------------+
// |   ADC ch # 6   |  ADC ch # 7   |
// +--------------------------------+  <-- Window # 1
// |   ADC ch # 0   |  ADC ch # 1   |
// +--------------------------------+
// |   ADC ch # 2   |  ADC ch # 3   |
// +--------------------------------+
// |   ADC ch # 4   |  ADC ch # 5   |
// +--------------------------------+
// |   ADC ch # 6   |  ADC ch # 7   |
// +--------------------------------+
//
// (*) Bit 7 - 4: type: (0xf)
//     Bit 3 - 0: one event data byte size (2 bytes)
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
    bool is_raw_data_packet();
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
    const static int FORMAT_POS  = 0;
    const static int N_CH_POS    = 1;
    const static int LENGTH_POS  = 4;
    const static int TRIGGER_POS = 8;

private:
    const unsigned char *m_buf;
    int m_buf_len;

};

#endif
