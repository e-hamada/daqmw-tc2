(テーマ) Mergerを利用して複数台のネットワークノードからデータを収集する
===============================================================


ex01で~/MyDaq/にSampleReaderとSampleMonitorから構成されるシステムを作成した。
このときは、データを出力するネットワークノードはエミュレータ１個であった。

ここでは、３台のボードからデータを取得することを想定するため、
3個のエミュレータからデータを取得することを学ぶ。


このシステムに２つのSampleReaderとMergerを追加し、以下のようなシステムを作成する。

emulator(port num = 2222) --- SampleReader  ---\

emulator(port num = 2223) --- SampleReader2 --- Merger --- SampleMonitor

emulator(port num = 2224) --- SampleReader3 ---/



エミュレータの起動
-----------------

1つのターミナル上で以下を実行する。

    % daqmw-emulator

別のターミナル上で以下を実行する。

    % daqmw-emulator -p 2223 -t 16k

別のターミナル上で以下を実行する。

    % daqmw-emulator -p 2224 -t 4k


このようにすることで、３個のエミュレータが起動される。
それぞれで、ポート番号が異なっている。（2222と2223と2224）
また、データ送信のスピードも異なっている。


SampleReaderのコピー
--------------------------------
SampleReaderをコピーし、名前を変える

    % cd ~/MyDaq/
    % cp -r /usr/share/daqmw/examples/SampleReader/ SampleReader2
    % cp -r /usr/share/daqmw/examples/SampleReader/ SampleReader3
    % cd SampleReader2
    % cp /usr/share/daqmw/examples/change-SampleComp-name/change-SampleReader-name.sh .

change-SampleReader-name.shの中を修正

修正前）　new_name_camel_case=SampleReader

修正後）　new_name_camel_case=SampleReader2


    % sh change-SampleReader-name.sh
    % make
    % cd ../SampleReader3
    % cp /usr/share/daqmw/examples/change-SampleComp-name/change-SampleReader-name.sh .

change-SampleReader-name.shの中を修正

修正前）　new_name_camel_case=SampleReader

修正後）　new_name_camel_case=SampleReader3

    % sh change-SampleReader-name.sh
    % make



Mergerの取得
--------------------------------

    % cd ~/MyDaq/
    % wget http://research.kek.jp/people/ehamada/Merger.tar
    % tar xvf Merger.tar 
    % cd Merger

Merger.hを少しだけ修正する

MergerのInport数をInPortNumとして明記する。
今回はMergerのInportの数が3個であるので、

修正前）    static const int InPortNum = 2;  

修正後）    static const int InPortNum = 3;  

    % make



コンフィグレーションファイルの取得
--------------------------------
ReaderとMonitorの時のコンフィグレーションファイルにsample.xmlを利用した。
このsample.xmlをsample3.xmlにコピーし、今回のシステムに対応できるように編集する。


SampleReaderはポート番号2222から、
SampleReader2はポート番号2223から、
SampleReader3はポート番号2224からデータを取得できるようにする。


なお、正解のコンフィグレーションファイルをdaqmw/の下においてある。

実行
--------------------------------

以下のコマンドを実行

    % run.py -cl sample3.xml

あとは通常どおりに動かすことができる



（ちなみに...）2つのリードアウトモジュールでDAQしたい場合のコンフィグレーションファイルも用意している。

    % cd ~/MyDaq/
    % wget http://research.kek.jp/people/ehamada/sample2.xml

この場合、SampleReader3を利用していない。

（注）MonitorのMonitor.hのInPortNumを2とし、makeを行うこと



課題
-----

上記のSampleMonitorは、３個のエミュレータから受け取ったデータを区別せずにヒストグラム化している。
SampleMonitorを修正し、別々のヒストグラムに表示させるようにすること。
（ポート番号2222のエミュレータ用のヒストグラム、ポート番号2223のエミュレータ用のヒストグラム、
ポート番号2224のエミュレータ用のグラフの３つのヒストグラムを表示させる、ということ）

（ヒント）

３個のSampleReaderでMergerに渡すデータをセットする際、DAQ-Middleware用のヘッダ（8BYTE)
の中のreserved領域（2BYTE)にポート番号を入れる。

Monitorはそのreserved領域に格納されたポート番号を見て、どのヒストグラムにfillすべきか
を判断させる。










