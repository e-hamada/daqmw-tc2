// -*- C++ -*-
/*!
 * @file
 * @brief
 * @date
 * @author
 *
 */

#include "RawDataMonitor.h"

using DAQMW::FatalType::DATAPATH_DISCONNECTED;
using DAQMW::FatalType::INPORT_ERROR;
using DAQMW::FatalType::HEADER_DATA_MISMATCH;
using DAQMW::FatalType::FOOTER_DATA_MISMATCH;
using DAQMW::FatalType::USER_DEFINED_ERROR1;

// Module specification
// Change following items to suit your component's spec.
static const char* rawdatamonitor_spec[] =
{
    "implementation_id", "RawDataMonitor",
    "type_name",         "RawDataMonitor",
    "description",       "RawDataMonitor component",
    "version",           "1.0",
    "vendor",            "Kazuo Nakayoshi, KEK",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
};

RawDataMonitor::RawDataMonitor(RTC::Manager* manager)
    : DAQMW::DaqComponentBase(manager),
      m_InPort("rawdatamonitor_in",   m_in_data),
      m_in_status(BUF_SUCCESS),
      m_monitor_update_rate(30),
      m_event_byte_size(0),
      m_canvas(0),
      m_debug(false)
{
    // Registration: InPort/OutPort/Service

    // Set InPort buffers
    registerInPort ("rawdatamonitor_in",  m_InPort);

    init_command_port();
    init_state_table();
    set_comp_name("RAWDATAMONITOR");

    for (int i = 0; i < N_GRAPH; i++) {
        m_graph[i] = 0;
    }
}

RawDataMonitor::~RawDataMonitor()
{
}

RTC::ReturnCode_t RawDataMonitor::onInitialize()
{
    if (m_debug) {
        std::cerr << "RawDataMonitor::onInitialize()" << std::endl;
    }

    return RTC::RTC_OK;
}

RTC::ReturnCode_t RawDataMonitor::onExecute(RTC::UniqueId ec_id)
{
    daq_do();

    return RTC::RTC_OK;
}

int RawDataMonitor::daq_dummy()
{
    if (m_canvas) {
        m_canvas->Update();
        // daq_dummy() will be invoked again after 10 msec.
        // This sleep reduces X servers' load.
        sleep(1);
    }

    return 0;
}

int RawDataMonitor::daq_configure()
{
    std::cerr << "*** RawDataMonitor::configure" << std::endl;

    ::NVList* paramList;
    paramList = m_daq_service0.getCompParams();
    parse_params(paramList);

    return 0;
}

int RawDataMonitor::parse_params(::NVList* list)
{

    std::cerr << "param list length:" << (*list).length() << std::endl;

    int len = (*list).length();
    for (int i = 0; i < len; i+=2) {
        std::string sname  = (std::string)(*list)[i].value;
        std::string svalue = (std::string)(*list)[i+1].value;

        std::cerr << "sname: " << sname << "  ";
        std::cerr << "value: " << svalue << std::endl;
        
        if (sname == "monitorUpdateRate") {
            if (m_debug) {
                std::cerr << "monitor update rate: " << svalue << std::endl;
            }
            char *offset;
            m_monitor_update_rate = (int)strtol(svalue.c_str(), &offset, 10);
        }
        // If you have more param in config.xml, write here
    }

    return 0;
}

int RawDataMonitor::daq_unconfigure()
{
    std::cerr << "*** RawDataMonitor::unconfigure" << std::endl;
    if (m_canvas) {
        delete m_canvas;
        m_canvas = 0;
    }

    for (int i = 0; i < N_GRAPH; i++) {
        if (m_graph[i]) {
            delete m_graph[i];
            m_graph[i] = 0;
        }
    }

    return 0;
}

int RawDataMonitor::daq_start()
{
    std::cerr << "*** RawDataMonitor::start" << std::endl;

    m_in_status  = BUF_SUCCESS;

    //////////////// CANVAS FOR HISTOS ///////////////////
    if (m_canvas) {
        delete m_canvas;
        m_canvas = 0;
    }
    m_canvas = new TCanvas("c1", "histos", 0, 0, 600, 400);

    int col, row;
    row = N_ROW_IN_CANVAS;
    col = N_GRAPH / row;
    if (N_GRAPH % row != 0) {
        col ++;
    }
    m_canvas->Divide(col, row);

    for (int i = 0; i < N_GRAPH; i++) {
        if (m_graph[i]) {
            delete m_graph[i];
            m_graph[i] = 0;
        }
        m_graph[i] = new TGraph();
    }

    return 0;
}

int RawDataMonitor::daq_stop()
{
    std::cerr << "*** RawDataMonitor::stop" << std::endl;

    for (int i = 0; i < N_GRAPH; i++) {
        m_graph[i]->Draw();
    }
    m_canvas->Update();

    reset_InPort();

    return 0;
}

int RawDataMonitor::daq_pause()
{
    std::cerr << "*** RawDataMonitor::pause" << std::endl;

    return 0;
}

int RawDataMonitor::daq_resume()
{
    std::cerr << "*** RawDataMonitor::resume" << std::endl;

    return 0;
}

int RawDataMonitor::reset_InPort()
{
    int ret = true;
    while(ret == true) {
        ret = m_InPort.read();
    }

    return 0;
}

int RawDataMonitor::fill_data(const unsigned char* mydata, const int size)
{
    rdp.set_buf(mydata, size);
    int window_size   = rdp.get_window_size();
    int n_ch          = rdp.get_num_of_ch();
    int trigger_count = rdp.get_trigger_count();
    
    unsigned short data[n_ch][window_size];
    
    for (int w = 0; w < window_size; w++) {
        for (int ch = 0; ch < n_ch; ch ++) {
            data[ch][w] = rdp.get_data_at(ch, w);
        }
    }
    
    for (int i = 0; i < N_GRAPH; i++) {
        for (int w = 0; w < window_size; w++) {
            m_graph[i]->SetPoint(w, w, data[i][w]);
        }
        m_graph[i]->SetMinimum(0.0);
        m_graph[i]->SetMaximum(5000.0);
        m_graph[i]->SetTitle(Form("CH: %d Trigger: %d", i, trigger_count));
    }

    rdp.reset_buf();

    return 0;
}

unsigned int RawDataMonitor::read_InPort()
{
    /////////////// read data from InPort Buffer ///////////////
    unsigned int recv_byte_size = 0;
    bool ret = m_InPort.read();

    //////////////////// check read status /////////////////////
    if (ret == false) { // false: TIMEOUT or FATAL
        m_in_status = check_inPort_status(m_InPort);
        if (m_in_status == BUF_TIMEOUT) { // Buffer empty.
            if (check_trans_lock()) {     // Check if stop command has come.
                set_trans_unlock();       // Transit to CONFIGURE state.
            }
        }
        else if (m_in_status == BUF_FATAL) { // Fatal error
            fatal_error_report(INPORT_ERROR);
        }
    }
    else {
        recv_byte_size = m_in_data.data.length();
    }

    if (m_debug) {
        std::cerr << "m_in_data.data.length():" << recv_byte_size
                  << std::endl;
    }

    return recv_byte_size;
}

int RawDataMonitor::daq_run()
{
    if (m_debug) {
        std::cerr << "*** RawDataMonitor::run" << std::endl;
    }

    unsigned int recv_byte_size = read_InPort();
    if (recv_byte_size == 0) { // Timeout
        return 0;
    }

    check_header_footer(m_in_data, recv_byte_size); // check header and footer
    m_event_byte_size = get_event_size(recv_byte_size);
    if (m_event_byte_size > DATA_BUF_SIZE) {
        fatal_error_report(USER_DEFINED_ERROR1, "DATA BUF TOO SHORT");
    }

    /////////////  Write component main logic here. /////////////
    memcpy(&m_recv_data[0], &m_in_data.data[HEADER_BYTE_SIZE], m_event_byte_size);

    fill_data(&m_recv_data[0], m_event_byte_size);

    if (m_monitor_update_rate == 0) {
        m_monitor_update_rate = 1000;
    }

    unsigned long sequence_num = get_sequence_num();
    if ((sequence_num % m_monitor_update_rate) == 0) {
        for (int i = 0; i < N_GRAPH; i++) {
            m_canvas->cd(i + 1);
            m_graph[i]->Draw("AC*");
        }
        m_canvas->Update();
    }
    /////////////////////////////////////////////////////////////
    inc_sequence_num();                      // increase sequence num.
    inc_total_data_size(m_event_byte_size);  // increase total data byte size

    return 0;
}

extern "C"
{
    void RawDataMonitorInit(RTC::Manager* manager)
    {
        RTC::Properties profile(rawdatamonitor_spec);
        manager->registerFactory(profile,
                    RTC::Create<RawDataMonitor>,
                    RTC::Delete<RawDataMonitor>);
    }
};
