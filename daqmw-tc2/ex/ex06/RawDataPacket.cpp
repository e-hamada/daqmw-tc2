#include "RawDataPacket.h"

RawDataPacket::RawDataPacket(): m_buf(NULL), m_buf_len(-1)
{
// no code required.  leave this method empty
}

RawDataPacket::~RawDataPacket()
{
// no code required.  leave this method empty
}

int RawDataPacket::set_buf(const unsigned char *buf, const int buf_len)
{
    m_buf = buf;
    m_buf_len = buf_len;

    return 0;
}

bool RawDataPacket::is_raw_data_packet()
{
    // write this method
}

int RawDataPacket::get_word_size()
{
    // write this method
}

int RawDataPacket::get_data_length()
{
    // write this method
}

int RawDataPacket::get_trigger_count()
{
    // write this method
}

int RawDataPacket::get_num_of_ch()
{
    // write this method
}

unsigned int RawDataPacket::get_data_at(int ch, int window)
{
    // write this method
}

int RawDataPacket::get_window_size()
{
    // write this method
}

int RawDataPacket::reset_buf()
{
    m_buf = NULL;
    m_buf_len = -1;
    
    return 0;
}
