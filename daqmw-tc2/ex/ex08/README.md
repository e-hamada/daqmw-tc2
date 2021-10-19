(テーマ) DAQ-Middlewareでモニターコンポーネントを開発する
=========================================================

ボードからデータを読んで保存できるようになったので、保存ではなくて
オンラインでグラフを書くモニターコンポーネントを開発する。

作業の順番としては

1. RawDataReader - RawDataLoggerコンポーネントを作ったディレクトリに
   RawDataMonitorを置く場所を作る
2. RawDataMonitorコンポーネントの作成
3. コンフィギュレーションファイルの作成
4. システム起動、ラン
5. trigger.pyでボードにトリガーを送りグラフが更新されていることを確認する
6. trigger停止
7. モニターの改造など

となる。

RawDataMonitorコンポーネントの場所の確保
-------------------------------------------

通常、一からRawDataMonitorコンポーネントを書くのではなくて、
すでにあるSampleMonitorをコピーして、データフォーマットを変更、
自分の用途にあうように改良していく。

SampleMonitorコンポーネントはDAQ-Middlewareをセットすると
/usr/share/daqmw/examples/SampleMonitorに入っているのでまず
これをコピーする:

    % cd ~/RawData
    % cp -r /usr/share/daqmw/examples/SampleMonitor .
    % cd SampleMonitor

前回同様コンポーネント名をRawDataMonitorに変更する作業を行う。

    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/utils/change-SampleMonitor-name.sh .
    % sh change-SampleMonitor-name.sh
    % cd ..
    % mv SampleMonitor RawDataMonitor
    % cd RawDataMonitor
    % make (名前を変えただけなので正常にコンパイルできるはず)

最後のmakeでエラーが出ないことを確認する。

このままではロジックはSampleMonitorのままなのでRawDataフォーマット
にあわせる、描画するものを変更するなどの作業が必要になる。

デコードルーチンのプログラムは昨日の課題の[ex06]で作成した。
このプログラムを使う場合、RawDataPacket.hおよびRawDataPacket.cppを利用する。
これらのプログラムは~/daqmw-tc2/daqmw-tc2/daqmw/RawDataMonitorに置いてあるのコピーする。

    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/RawDataMonitor/RawDataPacket.h .
    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/RawDataMonitor/RawDataPacket.cpp .

さらにMakefileでコンパイルソースコードを追加する。

    (修正前）
    SRCS += $(COMP_NAME).cpp
    SRCS += $(COMP_NAME)Comp.cpp

    （修正後）
    SRCS += $(COMP_NAME).cpp
    SRCS += $(COMP_NAME)Comp.cpp
    SRCS += RawDataPacket.cpp

の行を追加し、デコードルーチンのプログラム（RawDataPacket.hおよびRawDataPacket.cpp）を含めるように変更する。

これでコードを書く準備ができた。あとはロジックを書き直せばよい。

RawDataMonitorコンポーネントの作成
------------------------------------

以下変更を要する点のポイントを書いておく。

### RawDataMonitor.hでの変更点

ヘッダファイルはdaqmw-tc2/daqmw-tc2/daqmw/RawDataMonitorの下にあるヘッダファイルを利用すること。

    % cd  /home/daq/RawData/RawDataMonitor/
    % cp  ~/daqmw-tc2/daqmw-tc2/daqmw/RawDataMonitor/RawDataMonitor.h  .


- SampleMonitorではヒストグラムを書いていたのでTH1.hをインクルードし、
  TH1F *m_histとしてヒストグラムへのポインタを宣言していたが、RawDataMonitorでは書くものはグラフなのでTGraph.hをインクルードし、変数名、型もそれにあわせて変更する必要がある。

### RawDataMonitor.cppでの変更点

状態遷移関数(daq_configure(), daq_start()など)および特定の状態にある関数
(daq_run())について状態遷移の順番にコードを修正していくのがよいかと
思う。


#### RawDataMonitor


コンストラクタの変更


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

SampleMonitorでは、m_histなどの値を初期化していた。
グラフ化する場合は、それらの値は必要ないので、省く。
また、配列はコンストラクタ初期化ができないので、関数のなかで初期化を行う。


#### daq_configure()

特に変更はない。パラメータを増やしたときにはdaq_configure()から呼ばれている
parse_params()を変更する。

#### daq_start()

daq_start関数は以下のように変更すること。


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



SampleMonitorではヒストグラムデータ(m_hist)を使っているが、今回は
グラフで変数名も変えているはずなのでそれにあわせて変更する必要がある。


#### daq_run()

1. 上流コンポーネントから送られてきたデータを読み
2. デコードしてグラフデータをセットする(m_graph->SetPoint()などを使う)
3. ときどきグラフを画面に書く

という処理をおこなう。

上流コンポーネント（RawDataReader)から送られてくるデータを読むところは変更する必要は
ない。なお、上流コンポーネント（RawDataReader)から送られてくるデータは1イベント分のデータとなっているので、
このことに合わせてデータ処理を行っていく。

デコードしてグラフデータをセットする部分はdaq_run()から呼ばれている
fill_data()関数を使う。
SampleMonitorではfill_data()関数内でSampleMonitor::decode_data()関数
を使ってデコードを行ってきた。今回は、データをデコードし、そのデータをグラフに記載する処理を行う。

具体的にfill_data関数は以下のようになる。ただし、以下のfill_data関数には全ては書かれていない。
抜けている部分は自身で埋めていくこと。

    int RawDataMonitor::fill_data(const unsigned char* mydata, const int size)
    {

    /////////////////////////////////////////////
    // ・ここでの処理
    // mydataは1イベント分のデータが入っているバッファ、
    // sizeは1ベント分のデータのサイズとなっている。
    // 
    // ここでは、mydataに含まれるヘッダからwindowの数とch数を取得する。
    // 次にmydataに含まれるデータ部からwindow、chごとのデータをdata[][]に代入すること。
    // data[][]が以下の処理で利用されていく。
    //
    // ・ヒント
    // 昨日の課題の[ex06]で書いたクラスやその中の関数を使うと簡単に作ることができる。
    // （昨日の課題の[ex06]で書いたクラスを使わなくても良い。）
    ////////////////////////////////////////////

        for (int i = 0; i < N_GRAPH; i++) {
            for (int w = 0; w < window_size; w++) {
                m_graph[i]->SetPoint(w, w, data[i][w]);
            }
            m_graph[i]->SetMinimum(0.0);
            m_graph[i]->SetMaximum(5000.0);
            m_graph[i]->SetTitle(Form("CH: %d Trigger: %d", i, trigger_count));
        }
    
   
    //////////////////////////////////
    //必要ならここにも処理を書くこと
    //
    /////////////////////////////////
        return 0;
    }
    

SampleMonitorではdecode_data()関数でエミュレータのデータをデコードしていた。
今回、この関数は必要ではないため、消しておくこと。
消すときはRawDataMonitor.cppでの実装部だけではなくRawDataMonitor.hの
decode_data()関数を宣言しているところも消す必要がある。

ときどきグラフを書くというのは以下の部分である:

    if (m_monitor_update_rate == 0) {
        m_monitor_update_rate = 1000;
    } // m_monitor_update_rateが指定されていなかった場合に備えている

    // daq_run()内前半で上流からデータを読んでいるが
    // 読めたらシーケンス番号を1づつ増やしているので
    // その数字を使ってm_monitor_update_rate回に一回画面上の図を更新
    // している
    unsigned long sequence_num = get_sequence_num();
    if ((sequence_num % m_monitor_update_rate) == 0) {
        m_hist->Draw();
        m_canvas->Update();
    }

この部分を以下のようにする。

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


#### daq_stop()

SampleMonitorではヒストグラムのための処理が記載されていたが、その部分を削除する。


さらに、daq_stop()に以降した直前に読んだデータを使って画面に図を出す処理を書く。
具体的には以下のように書く

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




#### daq_unconfigure()

SampleReaderではヒストグラムデータをdeleteしていた。
RawDataMonitorではグラフデータをdeleteする(daq_start()でnewしていたのでdeleteしないと多数回start/stopを
繰り返したときにメモリーを浪費する)。


具体的には以下のように書く

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

#### decode_data
SampleMonitorでは、デコードするための関数としてdecode_dataを定義していた。この関数は必要ないので削除すること。ヘッダファイルにはこの関数を定義していないため、残ったままだとMakeのときにエラーになってしまう。


コンフィギュレーションファイルの作成
---------------------------------------

コンフィギュレーションファイルを作る。
(これまで作ってきたファイルを利用した方が効率が良い。）

システム起動、ラン
---------------------

    % run.py -cl (コンフィグレーションファイル)

でシステムを起動し、0, 1とキーを押してシステムを動かす。


ソフトウェアシミュレータ
--------------------
シミュレータは以下のコマンドで起動できる。

    % daqmw-tc-server

IPアドレスは127.0.0.1でポート番号は1234になる。

このシミュレータのオプションについては以下のURLを参照すること。

https://github.com/h-sendai/daqmw-tc-network-2/tree/master/emulator




できあがったら全てのチャンネルを表示できるようになる。

モニターしながらデータも保存するというシステムにするには
既に試したRawDataLoggerの他にDispatcherコンポーネントが
必要になる。Dispatcherのソースコードは
/usr/share/daqmw/examples/Dispatcherにあるのでコピーして
コンパイルする(Dispatcherを改造することはあまりないので
ここでは名前の変更は行わない)

    % cd ~/RawData
    % cp -r /usr/share/daqmw/examples/Dispatcher .
    % cd Dispatcher
    % make

4個のDAQコンポーネントを使う場合のコンフィギュレーションファイルを利用する。

    % cd ~/RawData
    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/4comps.xml .


4compsからDAQ-Middlwareを起動させ、モニターしながらデータも保存していることを確認する。
