// -*- C++ -*-
/*!
 * @file 
 * @brief
 * @date
 * @author
 *
 */

#ifndef RAWDATAMONITOR_H
#define RAWDATAMONITOR_H

#include "DaqComponentBase.h"

#include <arpa/inet.h> // for ntohl()

////////// ROOT Include files //////////
#include "TGraph.h"
// #include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TApplication.h"

#include "RawDataPacket.h"

using namespace RTC;

class RawDataMonitor
    : public DAQMW::DaqComponentBase
{
public:
    RawDataMonitor(RTC::Manager* manager);
    ~RawDataMonitor();

    // The initialize action (on CREATED->ALIVE transition)
    // former rtc_init_entry()
    virtual RTC::ReturnCode_t onInitialize();

    // The execution action that is invoked periodically
    // former rtc_active_do()
    virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

private:
    TimedOctetSeq          m_in_data;
    InPort<TimedOctetSeq>  m_InPort;

private:
    int daq_dummy();
    int daq_configure();
    int daq_unconfigure();
    int daq_start();
    int daq_run();
    int daq_stop();
    int daq_pause();
    int daq_resume();

    int parse_params(::NVList* list);
    int reset_InPort();

    unsigned int read_InPort();

    BufferStatus m_in_status;

    int m_monitor_update_rate;
    const static unsigned int DATA_BUF_SIZE = 1024*1024;
    unsigned char m_recv_data[DATA_BUF_SIZE];
    unsigned int  m_event_byte_size;
    int fill_data(const unsigned char* mydata, const int size);

    ////////// ROOT Graph //////////
    TCanvas *m_canvas;
    RawDataPacket rdp;
    const static int N_GRAPH = 4;
    const static int N_ROW_IN_CANVAS = 2;
    TGraph   *m_graph[N_GRAPH];
    bool m_debug;
};


extern "C"
{
    void RawDataMonitorInit(RTC::Manager* manager);
};

#endif // RAWDATAMONITOR_H
