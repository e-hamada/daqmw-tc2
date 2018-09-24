(テーマ) Webモードでシステムを動かす
====================================

DAQ-Middlewareを稼働させる方法にはコンソールモードと
Webモードがある。コンソールモードで動かす方法はすでに
試したので、次にWebモードで動かすのを試してみる。

まずrootになり、apacheが起動しているかどうかを確認する。

    root#  systemctl status httpd.service

Active: active (running)  とでる場合はhttpdは起動しているので
OK。

Active: inactive (dead)とでた場合は起動していないので起動する:

    root# systemctl status httpd.service

OS起動時に自動起動するようにするためには以下のコマンドを実行する:

    root# systemctl enable httpd.service

次にDAQ-Middlewareの起動の説明に移る。

Webモードで起動するにはrun.pyを-cなしで起動する。

    % run.py -l sample.xml

なにも表示されなければ正常に起動できている。
Webブラウザを使って下記サイトにアクセスするとWeb UIが起動する

    http://localhost/daqmw/operatorPanel/operatorPanel0.html

configure, begin, end, unconfigureのボタンがあるので適当にボタンを
押す。状態により押しても無意味なボタンはグレーアウトしている。

ラン番号は自動で1づつ増えていくようになっている。
