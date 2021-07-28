[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                  Copyright (C) 2007 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.


＜サンプル概要＞
	このサンプルはlibgcmオフラインツールライブラリを使ったサンプルです。
	テンプレートクラスに記述されたコマンドをバイナリファイルに生成します。
	Measureモードにより必要なコマンドサイズを取得しUnsafeモードでコマンドを生成します。


＜ファイル構成＞
	オフラインツールライブラリのファイル構成を下記に示します。

	$(CELL_SDK)/host-common/include/gcm_tool.h	共通ヘッダーファイル
	$(CELL_SDK)/host-linux/lib/libgcm_tool.a	Linux用ライブラリ
	$(CELL_SDK)/host-win32/lib/libgcm_tool.lib	Windows用ライブラリ
	$(CELL_SDK)/host-common/samples/gcm_tool/	サンプルディレクトリ

	オフラインツールライブラリが提供するコマンド生成関数はSPU用ライブラリと同一です。
	またコマンド生成関数の使い方はランタイムライブラリと同一です。
	詳細に関してはlibgcmのドキュメントを参照してください。


＜コンパイル方法＞

	下記の手順に従いコンパイルを行ってください。

	Linuxの場合：

	・環境変数の設定
		CELL SDK ガインストールされているパスを設定してください。
	    % export CELL_SDK=/usr/local/cell

	・コンパイルの実行
		% cd host-common/samples/gcm_tool
		% make

	Windowsの場合：

	・CELL SDK のインストール
		c:\usr\local\cell に CELL SDK がインストールされている必要があります。
		インストール先のパスが異なる場合はVisualStudioのProjectファイルのパスを
		環境に合うよう修正してください。

	・ソリューションファイルのロード
		host-common/samples/gcm_tool に移動します。
		フォルダにあるSolutionファイル"simple.sln"をVisualStudioからロードします。
		VisualStudioの「ソリューションのビルド」によりサンプルをビルドします。


＜プログラムの実行＞
	実行ファイル simple をコマンドラインから実行します。
	引数なしで実行した場合、デフォルトの出力ファイル "out.gcm" をカレントディレクトリに出力します。
	第１引数には任意の出力ファイル名を指定することができます。

	% simple <ファイル名>

	第１引数にオプション -h を指定するとヘルプを出力します。

	% simple -h


＜サンプルプログラムのカスタマイズ＞
	サンプルプログラムではテンプレートクラスを定義している"template.h"に生成するコマンドが記述されています。
	生成するコマンドを変更する場合はこのテンプレートクラスに記述されているコマンドをカスタマイズすることで可能です。
	より複雑な処理をホストツールとして行う場合はこのサンプルプログラムを参考にして独自にカスタマイズしてください。
