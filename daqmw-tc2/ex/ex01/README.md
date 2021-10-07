(テーマ) DAQ-Middleware付属サンプルコンポーネントを動かしてみる
===============================================================

DAQ-Middlewareでのデータ収集システムが動作をつかむために付属のサンプルコンポーネントを動かしてみる。

以下のような簡単なシステムを試してみる1。

emulator - SampleReader - SampleMonitor

ソースコードのコピーとコンパイル
--------------------------------

サンプルコンポーネントのソースコードは /usr/share/daqmw/examples ディレクトリ
以下に入っているのでそれをコピーする。

    % cd ~/MyDaq
    % cp -r /usr/share/daqmw/examples/SampleReader .
    % cp -r /usr/share/daqmw/examples/SampleMonitor .

続いてコンパイルを行う

    % cd SampleReader
    % make
    (SampleReaderCompというファイル名の実行形式ファイルができる)
    % cd ../SampleMonitor
    % make
    (SampleMonitorCompというファイル名の実行形式ファイルができる)
    % cd ..

コンフィギュレーションファイルの作成
------------------------------------

SampleReader - SampleMonitor システムのコンフィギュレーションは
/usr/share/daqmw/conf/sample.xml にあるのでこれをコピーする。

    % cd ~/MyDaq
    % cp /usr/share/daqmw/conf/sample.xml .

sample.xml中にexecPath(2箇所ある)というタグでSampleReaderComp、SampleMonitorComp
の実行形式ファイルがあるパスを指定しているので正しいパスかどうか
確認する。

コンポーネントの起動
--------------------

SampleReaderComp、SampleMonitorCompおよびこれらを統括するDaqOperator
を起動する。起動はrun.pyというコマンドを使う。run.pyの引数に
コンフィギュレーションファイルを指定する。
run.pyはコンフィギュレーションファイルを解析し、execPathタグで指定してある
ファイルを起動し、/usr/libexec/daqmw/DaqOperator/DaqOperatorCompを起動する。


DaqOperatorCompは通常書き換える必要がないのですでにコンパイル済みのものが入っている。

    % run.py -c -l sample.xml

 あるいはオプションは、まとめることができるので

    % run.py -cl sample.xml

オプションの意味は、-l: run.pyを起動したPCでDAQコンポーネントを起動する。
-c: コンソールモードでシステムを起動する である。

ターミナルにDaqOperatorが出力するパネルがでる。

システム起動
------------

数値入力待ちになっているので0を押しエンターキーを押すとconfiugredになる
(以下数字キーを押したあとは同様にエンターキーを押すこと)。
1を押すとラン番号を入力するようなながされるので適当に番号(1とか2とか)
を入力する。ラン番号を入力するとデータ収集を開始する。
FATALエラーとなるが今の手順ではエミュレータを起動していないのでこれは
正しい動作である。
2を押すとデータ収集を終了する。

エミュレータの起動
------------------

ターミナルを新たに開いて

    % daqmw-emulator

でデータ生成エミュレータを起動する。起動後このターミナルはさわらないので
じゃまならアイコン化しておくなどする。

データ収集再開
--------------

データ生成エミュレータが起動している状態で1を押すとデータ収集がされ、
画面上にヒストグラムが表示される。

システム終了
------------

2を押してデータ収集システムを終了させ、3を押しunconfigured状態にしたあと
control-cを押すとシステムが終了する。
