(テーマ) Webモードでシステムを動かす
====================================

DAQ-Middlewareを稼働させる方法にはコンソールモードと
Webモードがある。コンソールモードで動かす方法はすでに
試したので、次にWebモードで動かすのを試してみる。

まずapacheが起動しているかどうかを確認する。

    root# service httpd status

httpd (pid 12345) is running... とでる場合はhttpdは起動しているので
OK。

httpd is stoppedとでた場合は起動していないので起動する:

    root# service httpd start

OS起動時に自動起動するようにするためには以下のコマンドを実行する:

    root# chkconfig httpd on

次にDAQ-Middlewareの起動の説明に移る。

Webモードで起動するにはrun.pyを-cなしで起動する。

    % run.py -l sample.xml

なにも表示されなければ正常に起動できている。
Webブラウザを使って下記サイトにアクセスするとWeb UIが起動する

    http://localhost/daqmw/operatorPanel/operatorPanel0.html

configure, begin, end, unconfigureのボタンがあるので適当にボタンを
押す。状態により押しても無意味なボタンはグレーアウトしている。

ラン番号は自動で1づつ増えていくようになっている。
