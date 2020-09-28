#include "RawDataPacket.h"

RawDataPacket::RawDataPacket(): m_buf(NULL), m_buf_len(-1)
{

}

RawDataPacket::~RawDataPacket()
{

}

int RawDataPacket::set_buf(const unsigned char *buf, const int buf_len)
{
    m_buf = buf;
    m_buf_len = buf_len;

    return 0;
}

bool RawDataPacket::is_valid_magic()
{
    unsigned int *int_p;
    unsigned int magic;

    int_p = (unsigned int *) &m_buf[MAGIC_POS];
    magic = ntohl(*int_p);
    if (magic == 0x01234567) {
        return true;
    }
    else {
        return false;
    }
}

bool RawDataPacket::is_valid_footer()
{
    unsigned int *int_p;
    unsigned int footer;

    int_p = (unsigned int *) &m_buf[FOOTER_POS];
    footer = ntohl(*int_p);
    if (footer == 0x89abcdef) {
        return true;
    }
    else {
        return false;
    }
}

int RawDataPacket::get_word_size()
{
    return 2;
}

int RawDataPacket::get_data_length()
{
    unsigned int *len;
    int rv;
    len = (unsigned int *)&m_buf[LENGTH_POS];
    rv = ntohl(*len);

    return rv;
}

int RawDataPacket::get_trigger_count()
{
    unsigned int *trig;
    int rv;
    trig = (unsigned int *)&m_buf[TRIGGER_POS];
    rv = ntohl(*trig);

    return rv;
}

int RawDataPacket::get_num_of_ch()
{
    return 4;
}

unsigned int RawDataPacket::get_data_at(int ch, int window)
{
    unsigned short *data;

    int word_size = get_word_size();
    int n_ch      = get_num_of_ch();
    
    int offset = HEADER_SIZE + n_ch*word_size*window + ch*word_size;
    data = (unsigned short *)&m_buf[offset];
    unsigned short rv = ntohs(*data);
    rv = (rv & 0x0fff);
    return rv;
}

int RawDataPacket::get_window_size()
{
    int word_size = 2;
    int n_ch      = 4;
    int data_length = get_data_length();

    int window_size = data_length / (word_size*n_ch);

    return window_size;
}

int RawDataPacket::reset_buf()
{
    m_buf = NULL;
    m_buf_len = -1;
    
    return 0;
}
