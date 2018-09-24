(テーマ) ボードを読むシステム(DAQ-Middleware使用)を動かしてみる(Reader - Logger)
================================================================================

ここではボードからデータを読んでデータを保存するシステムを動かしてみる。
作業の順番としては

1. RawDataReaderコンポーネントの作成
2. RawDataLoggerコンポーネントの作成
3. コンフィギュレーションファイルの作成
4. システム起動、ラン
5. trigger.pyでボードにトリガーを送る
6. trigger停止
7. データがセーブされていることを確認

1. RawDataReaderコンポーネントの作成
------------------------------------

Readerはすでにdaqmw/RawDataReader/ディレクトリにあるのでこれをコピーして
使う。

    % cd
    % mkdir RawData (ホームディレクトリにRawDataディレクトリを作成。作るシステムはこの下にいれる)
    % cd RawData
    % cp -r ~/daqmw-tc/daqmw/RawDataReader .
    % cd RawDataReader
    % make

2. RawDataLoggerコンポーネントの作成
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

    % cd ~/RawData
    % cp -r /usr/share/daqmw/examples/SampleLogger .
    % mv SampleLogger RawDataLogger (ディレクトリ名の書き換え)
    % cd RawDataLogger
    % cp ~/daqmw-tc/daqmw/utils/change-SampleLogger-name.sh .
    % chmod +x change-SampleLogger-name.sh
    (change-SampleLogger-name.shのなかみを見てみる)
    (RawDataLoggerではない名前に変更する場合はnew_name_camel_caseを変更する)
    % ./change-SampleLogger-name.sh
    % grep -i sample * などしてSample, sampleの文字が残っていないかどうか確認
    % make

これでRawDataLoggerCompという実行ファイルができる。

3. コンフィギュレーションファイルの作成
---------------------------------------

コンフィギュレーションファイルを作る。ほぼ完成された雛型を
~/daqmw-tc/daqmw/reader-logger.xmlに用意してあるのでそれを編集して使う。

    % cd ~/RawData
    % cp ~/daqmw-tc/daqmw/reader-logger.xml .
    % reader-logger.xmlのexecPathを自分の環境にあわせて編集

4. システム起動、ラン
---------------------

    % run.py -cl reader-logger.xml

でシステムを起動し、0, 1とキーを押してシステムを動かす。

5. trigger.pyでボードにトリガーを送る
-------------------------------------

    % ~/daqmw-tc/trigger/trigger.py

としてstartボタンを押しトリガーを送る。読みだしシステムのイベントバイト数が
増えるのを確認する。

6. trigger停止、システム停止
--------------

trigger GUIのstopボタンを押しトリガーを送るのを停止する。
2を押してシステムを止める。


7. データがセーブされていることを確認
-------------------------------------

/tmp/ディレクトリにデータが保存されているのを確認する。
hexdumpコマンド、あるいは以前作ったファイルを読んでデコードするプログラムで
正当性を確認する。
    


