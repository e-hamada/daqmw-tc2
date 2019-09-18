(テーマ) Webモードでシステムを動かす
====================================

DAQ-Middlewareを稼働させる方法にはコンソールモードと
Webモードがある。コンソールモードで動かす方法はすでに
試したので、次にWebモードで動かすのを試してみる。

まずrootになり、apacheが起動しているかどうかを確認する。

    root# systemctl status httpd

実行後、以下のように表示されていれば、httpdは起動している。
 
    [root@localhost MyDaq]# systemctl status httpd
    ● httpd.service - The Apache HTTP Server
    Loaded: loaded (/usr/lib/systemd/system/httpd.service; enabled; vendor preset: disabled)
    Active: active (running) since 水 2019-09-18 16:40:33 JST; 3s ago


もし、Active: inactive (dead) とでた場合は起動されていない。httpdを起動させるために以下のコマンドを実行する:

    root# systemctl start  httpd


OS起動時に自動起動するようにするためには以下のコマンドを実行する:

    root# systemctl enable httpd.service


次に一般ユーザーに戻り、DAQ-Middlewareの起動の説明に移る。

Webモードで起動するにはrun.pyを-cなしで起動する。

    % run.py -l sample.xml

なにも表示されなければ正常に起動できている。
Webブラウザを使って下記サイトにアクセスするとWeb UIが起動する

    http://localhost/daqmw/operatorPanel/operatorPanel0.html

configure, begin, end, unconfigureのボタンがあるので適当にボタンを
押す。状態により押しても無意味なボタンはグレーアウトしている。

ラン番号は自動で1づつ増えていくようになっている。

終了させるときは

    %  pkill -f Comp

を実行すれば、裏で動いているDAQ-Middlwareがすべて終了される。
