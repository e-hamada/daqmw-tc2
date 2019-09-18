(テーマ)コンポーネント間のデータについて
=======================================

この章では、まずSampleReaderおよびSampleMonitorの処理について説明する。
処理を理解した上で、SampleReader-SampleMonitor間のデータを確認できる
処理を追加する。

DAQ-Middlwareオリジナルのデータ構造があるため、
SampleReader-SampleMonitor間のデータには
エミュレータから受け取ったデータ以外も含まれることに注意すること。


実習内容
--------

シーケンス番号が20で割り切れる時、SampleReaderが送るデータの初めの20Byteを確認する。

プログラムの編集
---------------

まず、SampleReaderのディレクトリをSampleReader_Dataにコピーする。

     % cd ~/MyDaq/
     % cp -r SampleReader SampleReader_Data
     % cd SampleReader_Data

SampleReader_DataのSampleReader.cppのdaq_run関数を次のように編集する。

    （修正前）
    if (m_out_status == BUF_SUCCESS) {   // previous OutPort.write() successfully done
        int ret = read_data_from_detectors();
        if (ret > 0) {
            m_recv_byte_size = ret;
            set_data(m_recv_byte_size); // set data to OutPort Buffer
        }
    }

    if (write_OutPort() < 0) {



    (修正後）
    if (m_out_status == BUF_SUCCESS) {   // previous OutPort.write() successfully done
        int ret = read_data_from_detectors();
        if (ret > 0) {
            m_recv_byte_size = ret;
            set_data(m_recv_byte_size); // set data to OutPort Buffer
        }
    }

    //下の7行を追加
    if(get_sequence_num() % 20 == 0){
        std::cerr << "sequence_num = " << std::dec << get_sequence_num() << std::endl;
        for(int i = 0; i < 20; i++){
            std::cerr << "Data" << i << " = 0x";
            std::cerr << std::hex << (int)m_out_data.data[i] << std::endl;
        }
    }


    if (write_OutPort() < 0) {


編集後は、makeを行う。

コンフィグレーションファイルの作成
---------------------------------

sample.xmlをsample_Data.xmlにコピーする。

    % cd ~/MyDaq
    % cp sample.xml sample_Data.xml

このsample_Data.xmlを編集したコンポーネントプログラムを読み込めるように編集する。

    (編集前）
    <execPath>/home/daq/MyDaq/SampleReader/SampleReaderComp</execPath>

    (編集後）
    <execPath>/home/daq/MyDaq/SampleReader_Data/SampleReaderComp</execPath>

実行
----

    sample_Data.xmlでDAQ-Middlwareを起動させる。
    SampleReaderのログ（/tmp/daqmw/log.SampleReaderComp）を確認して、期待通りになっていることを確認する。


課題
----



シーケンス番号が20で割り切れる時、SampleMonitorが受け取るデータをログに記載されるようにすること。


手順１

SampleMonitorのディレクトリをSampleSampleDataにコピーする。

     % cd ~/MyDaq/
     % cp -r SampleMonitor SampleMonitorData
     % cd SampleMonitorData

    
手順2

SampleMonitorDataのSampleMonitor.cppを編集する。
シーケンス番号が20で割り切れる時、SampleMonitorが受け取るデータをログに記載される処理を追加すること。

手順3

sample_Data.xmlを編集する。
SampleMonitorのディレクトリではなく、SampleMonitorDataのディレクトリのコンポーネントが
呼ばれるようにすること。

手順4

sample_Data.xmlでDAQ-Middlwareを起動させ、SampleReaderが出力するデータとSampleMonitorが
受け取るデータが一致することを確認する。



