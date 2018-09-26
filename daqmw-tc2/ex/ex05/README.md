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

    //追加するのはこの下から
    std::string test;

    if ( sname == "TEST" ) {
        test = svalue;
    }
    std::cerr << "test = " << test << std::endl;


追加した処理では、受け取ったパラメータのpidがTESTのとき、test変数にそのpidに
対する値が格納されるようになっている。
また、test変数の値はログに記載されるようにしている。

次にコンフィグレーションファイルを編集する。
以下のコマンドを実行し、sample.xmlをsamplePara.xmlにコピーする。

    %  cd ~/MyDaq/
    %  cp cp sample.xml samplePara.xml

samplePara.xmlを以下のように編集する。

    execPathの編集（SampleReaderからSampleReaderParaに変更）
    (編集前）<execPath>/home/daq/MyDaq/SampleReader/SampleReaderComp</execPath>
    (編集後）<execPath>/home/daq/MyDaq/SampleReaderPara/SampleReaderComp</execPath>

    パラメータの追加
    (編集前)<params>
                <param pid="srcAddr">127.0.0.1</param>
                <param pid="srcPort">2222</param>
            </params>
    (編集後)<params>
                <param pid="srcAddr">127.0.0.1</param>
                <param pid="srcPort">2222</param>
                <param pid="TEST">testtest</param>
           </params>


編集後、samplePara.xmlからDAQ Middlewareを起動させ、
configureした後にログを見るとtest変数の値がログに記載される。



