(テーマ)ログの確認（状態遷移の確認）
====================================

ログはコンポーネントごとに作成され、/tmp/daqmw/log.(コンポーネント名)Compに置かれる。
ここではSampleReaderのログを確認し、SampleReaderの状態遷移の確認を行う。


ログの表示
--------------------------------
ターミナルを開いて、下記を実行する。

    % tail -f /tmp/daqmw/log.SampleReaderComp

すると、SampleReaderのログが表示される。tailコマンドに-fオプションを追加させているので、ログが
更新されるとその分の表示も行われる。

別のターミナルを開いて、ex01で行ったようにSampleReaderComp、SampleMonitorCompおよびこれらを
統括するDaqOperatorを起動する。

    % cd ~/MyDaq
    % run.py -cl sample.xml


数値入力待ちになっているので0を押しエンターキーを押すとconfiugredになる。すると、別ターミナルで開いていた
ログに下記のようなメッセージが追加される。

    *** SampleReader::configure

さらに、1を押すとラン番号を入力するよう、うながされるので適当に番号(1とか2とか)
を入力する。ラン番号を入力するとデータ収集を開始する。
すると、以下のようなメッセージがログに追加される。

    *** SampleReader::start

2を押してデータ収集システムを終了させ、3を押しunconfigured状態にしたあと control-cを押すとシステムが終了する。

ログ出力方法
--------------------------------
次にプログラムのどの箇所でログ出力を定義しているのかを確認する。
~/MyDaq/SampleReaderにあるSampleReader.cppの中を見ると関数daq_configure()がある。configured状態に遷移すると、
この関数が一度だけ実行される。この関数の初めに

    std::cerr << "*** SampleReader::configure" << std::endl;

と記載されているが、この箇所がログを出力するための記述である。また、関数daq_start()はデータ収集が開始された直後に
一度だけ実行される関数であり、この関数にも同様にログ出力の記述がされている。
これらのように、

    std::cerr << "◯◯◯" << std::endl;

と記載することで、◯◯◯の部分がログファイルに出力される。

ログはコンポーネントのプログラムを作る上で、役に立つことがよくある。例えば、何らかの理由でプログラムが
止まってしまった場合、ログを利用することで止まってしまう場所を特定することができる。


SampleReaderの編集
--------------------------------
関数daq_run()の開始直後に以下のような記述を追加してほしい。

    std::cerr << "*** SampleReader::run" << std::endl;

Makeしたあと、DAQ Middlewareを起動する。

    % cd ~/MyDaq
    % run.py -cl sample.xml

データ収集を開始すると、

    *** SampleReader::run

というメッセージがログに繰り返し表示されることが確認できる。
これはデータ収集がされている間は関数daq_run()が繰り返し、実行されるからである。

課題
----

daq_pause, daq_stopでも、daq_configureやdaq_runと同様にログの確認を行うこと。
また、SampleMonitorコンポーネントでも同様になることを確認する。






