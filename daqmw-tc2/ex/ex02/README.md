(テーマ) C++の復習
==================
クラスファイルを作りそれを利用するプログラムを作る

ファイル
--------

* MyClass.h   (クラス宣言)
* MyClass.cpp (実装)
* main.cpp    (MyClassを使うプログラム)

C++プログラムはmain()関数からスタートする。main()関数は
main.cppに書いてある(かならずmain.cppというファイル名で
なければいけないわけではない)。

実習内容
--------

ファイルをコピーしてmakeで実行ファイルを作る:

    % cd ~/daqmw-tc/sandbox
    % cp -r ../ex/ex02 .
    % cd ex02
    % make

実行前にコードを見て結果を予想する。次に
実行ファイル main を実行してみる:

    % ./main

またMakefileの中身を見てみる。Makefileについては以下を参照。

    % make clean

で*.oファイル、および実行ファイル(今の場合はmain)が消されることも
確認しておく。

コードの変更
------------

- MyClass.h、MyClass.cppにメンバー変数m_zを追加し、set_z()メソッド、
  get_z()メソッドを追加する。
- main.cppを変更し、set_z()、get_z()を使って値をセット、ゲットする
  プログラムを書く。

Makefileの確認
--------------

    % g++ main.c MyClass.c -o main

のようにコマンドを投入してもよいが、コードの変更後は再びこの
コマンドを投入することになるのでMakefileを使って実行ファイル生成は
makeとだけ入力するとできるようにしておいたほうがよい。

Makefile中での変数は

    CC = g++
    PROG = main

のように定義する。変数を参照するには変数名先頭に $ (dollar)を置き
変数名を () でくくる。

ある変数に文字列を追加するには += で追加することができる。ここでは

    OBJS += main.o
    OBJS += MyClass.o

のようにOBJS変数に2個の文字列を追加している。

Makefileでは

    target: dependencies
        dependenciesからtargetを作成するコマンド

という形式で実行するコマンドを書く(2行目のコマンドを書く行の
先頭はスペースではなくてタブである必要がある)。上のように書くと
targetファイルとdependenciesに書かれたファイル群のタイムスタンプを
比較し、targetファイルのほうが古ければ(あるいはtargetファイルがなければ)
2行目に書いたコマンドを実行しtargetファイルを作成する。

Makefile中には複数のターゲットを書くことができる。引数なしに単にmakeと
入力して実行するとMakefile中の最初のターゲットを生成するコマンドが
実行される。通常、最終目標物ターゲットをallとして

    all: program_name

と書いておく。

このMakefileを見ると

    $(PROG): $(OBJS)

と書いてあり、 $(OBJS) から $(PROG) を作ることを示してあるが、実際に
実行するコマンドは書いていない。これはmakeコマンドが
*.oファイルから実行ファイルを作成するコマンドをしっているからである。
この機能を利用することでMakefileの記述を簡略化することができる。

その他makeについての詳細はコマンドラインから

    info make

あるいはWeb上で http://www.gnu.org/software/make/manual/make.html から
参照できる。


