(テーマ) ボードを読むシステムを動かしてみる(Reader - Logger)
================================================================================

ここではボードからデータを読んでデータを保存するシステムを動かしてみる。
作業の順番としては

1. RawDataLoggerコンポーネントの作成
2. RawDataReaderコンポーネントの作成
3. コンフィギュレーションファイルの作成
4. システム起動、ラン
5. ボード(エミュレータ)を起動
6. データがセーブされていることを確認





RawDataLoggerコンポーネントの作成
------------------------------------

ほとんどのシステムではデータをファイルに保存するDAQコンポーネントは
DAQ-Middlewareに例題として付属しているSampleLoggerコンポーネントを
使うことができる。SampleLoggerコンポーネントは

- データをコンフィギュレーションファイルのdirNameパラメータで指定したディレクトリに保存する
- ファイルがmaxFileSizeInMegaByteパラメータで指定するバイト数を越えたら次のファイルに書く
  (イベント途中で次のファイルに移ることはない。Readerコンポーネントが1回に送ったデータを途中
  で分断することはしない)。
- 保存するファイル名はYYYYMMDDTHHMMSS_RRRRR_BBB.dat (RRRRRはrun番号、BBBは000から始まる連番)。
  例:20110202T143748_000000_000.dat
- パラメータのisLoggingがyesになっている場合に実際にデータを保存する。
  noになっている場合はデータは保存しない(テストに使ったりする)

と動作する。

コピーしてそのままの名前で使ってもよいが、少し改造する場合もある
(たとえば「イベントデータのなかに時刻情報が入っていないのでLoggerコンポーネントが
動いているPCから時刻情報を取得して時刻情報を追加して書くようにするなど)。
その場合は動作仕様が変わってくるので名前をSampleLoggerから変更したほうがよい。
ここでSample*コンポーネントの名前を変更する方法を学ぶ。SampleLoggerからRawDataLoggerに
名前を変更することにする。

コード上では大文字小文字を以下のように使い分けている。かならずこのように
しなければならないわけではないが、習慣上そうなっているので引継が生じるなどの場面を
考えると守っておいたほうがよい。

- samplelogger (全部小文字) InPort、OutPortの名前
- SampleLogger (大文字小文字ミックス。最初の文字、および単語の切れ目は大文字) ファイル名、コンポーネント名
- SAMPLELOGGER (全部大文字) インクルードガード

SampleLoggerコンポーネントは /usr/share/daqmw/example/SampleLogger/ ディレクトリに
ある。これをコピーして、手で修正してもよいが間違いが生じる可能性が大きいので
sedコマンドを使ったスクリプトを用意したのでこれを使う(現在のDAQ-Middlewareにはまだないが今後
追加する予定になっている)。

    (ホームディレクトリにRawDataディレクトリを作成。作るシステムはこの下にいれる)
    % mkdir RawData 
    % cd ~/RawData
    % cp -r /usr/share/daqmw/examples/SampleLogger .
    % mv SampleLogger RawDataLogger (ディレクトリ名の書き換え)
    % cd RawDataLogger
    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/utils/change-SampleLogger-name.sh .
    % chmod +x change-SampleLogger-name.sh
    (change-SampleLogger-name.shのなかみを見てみる)
    (RawDataLoggerではない名前に変更する場合はnew_name_camel_caseを変更する)
    % ./change-SampleLogger-name.sh
    % grep -i sample * などしてSample, sampleの文字が残っていないかどうか確認
    % make

これでRawDataLoggerCompという実行ファイルができる。



RawDataReaderコンポーネントの作成
------------------------------------

同様に、ReaderもSampleReaderも利用する。

    % cd ~/RawData
    % cp -r /usr/share/daqmw/examples/SampleReader .
    % mv SampleReader RawDataReader (ディレクトリ名の書き換え)
    % cd RawDataReader
    % cp ~/daqmw-tc2/daqmw-tc2/daqmw/utils/change-SampleReader-name.sh .
    % chmod +x change-SampleReader-name.sh
    % ./change-SampleReader-name.sh


課題
----


このRawDataReaderをボードからデータを受信できるように編集すること。


SampleReaderの場合、read_data_from_detectors関数でreadAll関数で1024BYTEだけ読み込み、
そのデータをm_dataに格納していた。

今回の場合は、まずreadAll関数でヘッダのデータ長(12BYTE)の分だけデータを読み込む。
次に、取得したヘッダデータから「データ部分のバイト長」を取り出し、その分だけreadAll関数
で読み込むよむ。
最後にreadAll関数でフッタのデータ長(4BYTE)の分だけデータを読み込む。
それらのデータをm_dataに格納すれば良い。

SampleLoggerでは、上段から受け取ったデータをすべて保存していた。今回もその処理で問題ないので、
RawDataLogger（SampleLoggerをコピーしたコンポーネント）についてはコンポーネント名の変更以外は編集する必要はない。


コンフィギュレーションファイルの作成
---------------------------------------

コンフィギュレーションファイルを作る。ex04でSampleReaderとSampleLoggerを使用するコンフィグレーションファイルを作成していたので、それを利用する。

    % cd ~/RawData
    % cp ~/MyDaq/reader-logger.xml .
    
reader-logger.xmlを今回作成したRawDataReaderとRawDataLoggerの環境にあわせて編集すること(パスの設定など）。
また、ボードのアドレスはに対応させること。
（デフォルトでは"127.0.0.1"、ポート番号は"1234")
    

ソフトウェアシミュレータ
--------------------
シミュレータは以下のコマンドで起動できる。

    % daqmw-tc-server

IPアドレスは127.0.0.1でポート番号は1234になる。

このシミュレータのオプションについては以下のURLを参照すること。

https://github.com/h-sendai/daqmw-tc-network-2/tree/master/emulator


システム起動、ラン
---------------------

    % run.py -cl reader-logger.xml

でシステムを起動し、0, 1とキーを押してシステムを動かす。



データがセーブされていることを確認
-------------------------------------

/tmp/ディレクトリにデータが保存されているのを確認する。
hexdumpコマンド、あるいは以前作ったファイル（昨日の講義のex08で作成したread_file_decode_graph）
を読んでデコードするプログラムで正当性を確認する。
    


