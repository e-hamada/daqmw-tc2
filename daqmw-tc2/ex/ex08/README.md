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

    % cp ~/daqmw-tc2/daqmw-pc2/daqmw/utils/change-SampleMonitor-name.sh .
    % sh change-SampleMonitor-name.sh
    % cd ..
    % mv SampleMonitor RawDataMonitor
    % cd RawDataMonitor
    % make (名前を変えただけなので正常にコンパイルできるはず)

最後のmakeでエラーが出ないことを確認する。

このままではロジックはSampleMonitorのままなのでRawDataフォーマット
にあわせる、描画するものを変更するなどの作業が必要になる。

デコードルーチンは昨日の課題の[ex06]で書いたものを使うので
RawDataPacket.hおよびRawDataPacket.cppをコピーし、さらに
Makefileで

    SRCS += RawDataPacket.cpp

の行を追加する。これでコードを書く準備ができた。あとはロジックを
書き直せばよい。

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

上流コンポーネントから送られてくるデータを読むところは変更する必要は
ない。

デコードしてグラフデータをセットする部分はdaq_run()から呼ばれている
fill_data()関数を使う。
SampleMonitorではfill_data()関数内でSampleMonitor::decode_data()関数
を使ってデコードを行っている。
今回実習で作成するRawDataMonitorではすでに実装ずみの
デコードルーチンを使うのでdecode_data()関数は必要ではない(消すときは
RawDataMonitor.cppでの実装部だけではなくRawDataMonitor.hの
decode_data()関数を宣言しているところも消す必要がある)。

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

#### daq_stop()

daq_stop()に以降した直前に読んだデータを使って画面に図を出す処理を書く。

#### daq_unconfigure()

SampleReaderではヒストグラムデータ、RawDataMonitorではグラフデータを
deleteする(daq_start()でnewしていたのでdeleteしないと多数回start/stopを
繰り返したときにメモリーを浪費する)。

コンフィギュレーションファイルの作成
---------------------------------------

コンフィギュレーションファイルを作る。

    % cd ~/RawData
    % cp ~/daqmw-tc/daqmw/sample.xml read-monitor.xml
    % reader-monitor.xmlを編集


システム起動、ラン
---------------------

    % run.py -cl reader-monitor.xml

でシステムを起動し、0, 1とキーを押してシステムを動かす。

trigger.pyでボードにトリガーを送る
-------------------------------------

    % ~/daqmw-tc/trigger/trigger.py

としてstartボタンを押しトリガーを送る。読みだしシステムのイベントバイト数が
増えるのを確認する。

trigger停止
--------------

trigger GUIのstopボタンを押しトリガーを送るのを停止する。
2を押してシステムを止める。

モニターの改造
-----------------

できあがったら全てのチャンネルを表示するようにする。

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

4個のDAQコンポーネントを使う場合のコンフィギュレーションファイルを編集する

    % cd ~/RawData
    % cp ~/daqmw-tc/daqmw/4comps.xml .
    % 4compis.xmlを編集


4compsからDAQ-Middlwareを起動させ、モニターしながらデータも保存していることを確認する。
