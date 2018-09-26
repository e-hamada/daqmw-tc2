(テーマ) パラメータ取得
===================================

ex04で、コンフィグレーションファイルの中で各コンポーネントに渡すパラメータに触れた。
ここでは、そのパラメータの設定方法について学習する。


コンポーネントプログラムにおけるパラメータ取得処理
--------------------------------------------------

sample.xml等のコンフィグレーションファイルのSampleReaderの設定では、以下のように設定してある。
ここで、srcAddrやsrcPortはpidであり、127.0.0.1や2222はそのpidに対する値を示している。

    <param pid="srcAddr">127.0.0.1</param>
    <param pid="srcPort">2222</param>



次にコンポーネントのプログラムであるSampleReaderのSampleReader.cppの中を見ると、
daq_configure()で

    ::NVList* paramList;
    paramList = m_daq_service0.getCompParams();
    parse_params(paramList);

と記載されている。


コンフィグレーションファイルで指定したパラメータ（pidやそのpidに対する値）はparamListに格納され、
そのparamListを引数にしたparse_params(::NVList* list)が呼ばれる。

この中のfor文の中で

     std::string sname  = (std::string)(*list)[i].value;
     std::string svalue = (std::string)(*list)[i+1].value;

と記載されている。paramListの中では偶数番目にはpidが、
奇数番目にはpidに対する値が格納されることになる。

SampleReaderのの場合だとfor文のi = 0の時に、snameに”srcAddr”、svalueに”127.0.0.1”が入る。
for文のi = 1の時に、snameに”srcAddr”、svalueに”2222”が入る。


パラメータの追加
--------------------------------------------------

    %  cd ~/MyDaq/
    %  cp -r SampleReader SampleReaderPara

と実行し、SampleReaderをコピーしたSampleReaderParaにパラメータを追加する処理を追加する。

ここでは、SampleReaderParaディレクトリの中のSampleReader.cppのparse_params(::NVList* list)を以下のように修正する。

    （修正前）
    if ( sname == "srcPort" ) {
        srcPortSpecified = true;
        if (m_debug) {
            std::cerr << "source port: " << svalue << std::endl;
        }
        char* offset;
        m_srcPort = (int)strtol(svalue.c_str(), &offset, 10);
    }


    （修正後）
    if ( sname == "srcPort" ) {
        srcPortSpecified = true;
        if (m_debug) {
            std::cerr << "source port: " << svalue << std::endl;
        }
        char* offset;
        m_srcPort = (int)strtol(svalue.c_str(), &offset, 10);
    }
    if ( sname == "TEST" ) {
        srcPortSpecified = true;
        if (m_debug) {
            std::cerr << "source port: " << svalue << std::endl;
        }
        char* offset;
        m_srcPort = (int)strtol(svalue.c_str(), &offset, 10);
    }






バイナリファイルを読んでみる。

実習内容
--------

バイナリファイルを読むプログラムを書く。
バイナリファイルはbs/sample.datにある(これは
実習に使うボードから読んだデータ)。

方針
----

サンプル例は ~/daqmw-tc/bs/fread にある。
ここでは、この中を見て学習する。

ファイルのオープン
------------------

バイナリファイルはテキストファイルと違って行の概念がないので
どこまで読めばよいのか自動できめることができない。読むバイト数を指定する必要がある。

ファイルを読む前にfopen()する必要がある(テキストファイルと同様)。

    #include <stdio.h>
    
    FILE *fp = fopen("filename", "r");
    if (fp == NULL) {
        // エラー処理
    }

エラー処理の方法はいろいろあるだろうが、ここではerr()関数を使う。

    err(1, "fopen");

とすると

    progname: fopen: 失敗した理由

が標準エラー出力に出力され、終了ステータス1で終了する。

読みだし
--------

openできたらfread()関数を使って読む。

    char buf[1024];
    int n = fread(buf, 1 /* bytes */, 128, fp);

とすると、「1バイトのオブジェクト」を「128個」読むのを試みる。
戻り値はよみとった「オブジェクト数」。
128バイト読むには、他に

    fread(buf, 8, 16, fp); // 8バイトを16個読む
    fread(buf, 16, 8, fp); // 16バイトを8個読む

というのも考えられる。戻り値がそれぞれ違う(一番下の例では128バイト
読めたときに16を返す)。通常何バイト読めたか知りたいことが多いと思うので
第2引数は1を指定するのがよいかと思う。
(オブジェクトバイト数，読みだし回数)の組合せがいろいろある場合もあるが、
効率よく読めるという組合せは特にないので、自分が欲しい戻り値で決めれば
よい。

ファイル終端などリクエストしたバイト数未満しか読めない場合はエラーとはならず
読み取ったオブジェクト数を返す。指定したバイト数未満しか読めないのは
異常を示していると思う場合にはそれなりにエラーメッセージを出した
ほうがよい。

fread()の戻り値として0が返ってきた場合の状態は

* エラー
* ファイル終端まで達した(EOF)

のふたつがあり、戻り値だけでは区別がつかない。man freadでRETRUN VALUEの
ところを読むこと。エラー、EOFの判定にはferror()、feof()関数を使えと
書いてある。

