[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 160.008
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージには、汎用のバイナリデータフォーマットCXML変換ツール用の
ライブラリ、コンパイラ及びドキュメントが含まれています。

CXMLファイルは、XML ファイルを変換して作成します。変換に使用する
コンパイラはPython スクリプトとしてcell/samples/cxml/compiler/cxml.py
で提供されています。
使用方法は、cell/samples/cxml/compile/readme.txtおよび「CXML 概要」の
ドキュメントを参照して下さい。

また、ゲームアプリケーションが実行時にCXMLファイルにアクセスするために、
cxmlaccess ライブラリおよびcxmlutil ライブラリを使用します。
詳細は「CXML 概要」のドキュメントを参照して下さい。

PLAYSTATION(R)3 Programmer Tool Runtime Library SDK 160.008環境に
本パッケージを展開してご使用ください。

----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
cell/
|---Readme_CXML_*.txt
|---SDK_doc/     ドキュメント
|    +---jp/en
|          +---pdf/cxml
|                |---CXML-Overview_*.pdf
|                +---CXML-Reference_*.pdf
+---samples
     +---cxml
          |---compiler
          |    +---cxml.py  CXML コンパイラ（Python スクリプト）
          |---compile       CXML コンパイラ作成用サンプル
          |---include
          |    |---cxmlaccess.h
          |    +---cxmlutil.h
          |---lib           ライブラリのソースコード
          |    +---src
          |         |---cxmlaccess   cxmlaccess ライブラリ
          |         +---cxmlutil     cxmlutil ライブラリ
          |---makefile
          |---makefile_base
          |---read
          +---write

----------------------------------------------------------------------
動作確認環境
----------------------------------------------------------------------
Linux環境
   Red Hat Professional Workstation (Linux 2.4.21)
   Red Hat Enterprise Linux 4 WS (Linux 2.6.9)
   SUSE Linux Professional 9.2 (Linux 2.6.8)

Windows環境
   Windows XP Professional Service Pack 2

PLAYSTATION(R)3 Programmer Tool Runtime Library 160.008

----------------------------------------------------------------------
使用許諾・制限
----------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

----------------------------------------------------------------------
商標に関する注意書き
----------------------------------------------------------------------
"PLAYSTATION"は株式会社ソニー・コンピュータエンタテインメントの
登録商標です。
パッケージ内に記載されている会社名、製品名は一般に各社の商標または
登録商標です。
なお、本文中に(R)、(TM)マークは明記していない場合があります。
