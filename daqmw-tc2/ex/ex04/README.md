(テーマ)コンフィグレーションファイルの編集（コンポーネント構成）
================================================================================


ここまではSampleReaderとSampleMonitorから構成されるシステムでデータ収集を行ってきた。
次に、コンフィグレーションファイルを編集し、SampleReaderとSampleLoggerを使ったシステムを起動させる。



SampleLoggerコンポーネントの作成
------------------------------------
サンプルコンポーネントのソースコードは /usr/share/daqmw/examples ディレクトリ
以下に入っているのでそれをコピーする。

    % cd ~/MyDaq
    % cp -r /usr/share/daqmw/examples/SampleLogger .

続いてコンパイルを行う

    % cd SampleLogger
    % make
    (SampleLoggerCompというファイル名の実行形式ファイルができる)
    % cd ..

コンフィギュレーションファイルの編集
------------------------------------

以下のコマンドを実行し、sample.xmlをreader-logger.xmlとしてコピーする。

    % cp sample.xml reader-logger.xml

reader-logger.xmlを以下の様に変更し、SampleReaderとSampleMonitorを使ったシステムのための
コンフィグレーションファイルを作成する。

    利用するコンポーネント名の指定
    % （変更前）<component cid="SampleMonitor0">
    % （変更後）<component cid="SampleLogger0">

    コンポーネントのインスタンス名の指定
    % （変更前）<instName>SampleMonitor0.rtc</instName>
    % （変更後）<instName>SampleLogger0.rtc</instName>

    コンポーネントの実行形式ファイルのパスの指定
    % （変更前）<execPath>/home/daq/MyDaq/SampleMonitor/SampleMonitorComp</execPath>
    % （変更後）<execPath>/home/daq/MyDaq/SampleLogger/SampleLoggerComp</execPath>

    インポートの名の指定
    % （変更前）<inPort from="SampleReader0:samplereader_out">samplemonitor_in</inPort>
    % （変更後）<inPort from="SampleReader0:samplereader_out">samplelogger_in</inPort>

    パラメータの指定
    % （変更前）<param pid="monitorUpdateRate">20</param>
    % （変更後）<param pid="dirName">/tmp</param>
    %           <param pid="isLogging">yes</param>
    %           <param pid="maxFileSizeInMegaByte">1024</param>
    

monitorUpdateRateはヒストグラムを更新するレートを表しており、
20の場合は20イベントに1回の頻度でヒストグラムを更新している。
また、dirNameは保存するデータのパス、isLoggingは保存するかどうかの選択（yesはデータを保存、
noはデータを保存しない）、maxFileSizeInMegaByteは1個のデータファイルの最大容量
（この容量を超えたら別のデータに書き込むことになる）を表している。


なお、reader-logger.xmlの正解は
/usr/share/daqmw/conf/reader-logger.xml
にある。


実行
------------------------------------
コンフィグレーションファイルが置いてあるディレクトリで以下のコマンドを実行し、
システムを起動させる。

    % run.py -cl reader-logger.xml


起動後は、これまで行ってきた手順と同様にしてデータ収集を行う。
データは/tmpディレクトリの下に置かれている。ファイル名には20180925T191504_000001_000.dat
のように日付けや番号が記載されている。

[課題]
------------------------------------
SampleReader,Dispatcher,SampleLogger,SampleMonitorの4つのコンポーネントから構成されるシステムを作成すること。


手順1

Dispatcherの機能は1つのコンポーネントからデータを受け取り、
受け取ったデータを2つのコンポーネントに送ることである。

このコンポーネントは、/usr/share/daqmw/examples/Dispatcher/に置いてある。
他のコンポーネントと同様にコピーして利用すること。



手順2

以下のコマンドを実行し、sample.xmlを4comps.xmlとしてコピーする。

    % cp sample.xml 4comps.xml

4comps.xmlを編集して、4つのコンポーネントから構成されるシステムのためのコンフィグレーションファイル
となるよう編集する。


手順3
コンフィグレーションファイルにおいてDispatcherコンポーネントの部分は以下のようになる。

            <component cid="Dispatcher0">
               <hostAddr>127.0.0.1</hostAddr>
               <hostPort>50000</hostPort>
               <instName>Dispatcher0.rtc</instName>
               <execPath>/home/daq/MyDaq/Dispatcher/DispatcherComp</execPath>
               <confFile>/tmp/daqmw/rtc.conf</confFile>
               <startOrd>3</startOrd>
               <inPorts>
                  <inPort from="SampleReader0:samplereader_out">dispatcher_in</inPort>
               </inPorts>
               <outPorts>
                  <outPort>dispatcher_out1</outPort>
                  <outPort>dispatcher_out2</outPort>
               </outPorts>
               <params/>
            </component>



dispatcherコンポーネントは2個のアウトポートがあるため、上のDispatcherコンポーネントの部分には、
outPortが2つ記載されている。このアウトポートがSampleLoggerとSampleMonitorのインポートに接続されることになる。










