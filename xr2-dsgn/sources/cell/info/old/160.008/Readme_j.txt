[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 160.008
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、RSX(R)搭載のハードウェア(Reference Tool)による
アプリケーションソフトの開発を行うために必要なソフトウェアが含まれています。

今回より、別パッケージ（PS3_SDK_HDD_GAME-154_004.zip）でリリースしていた
HDD起動ゲームユーティリティ をSDKパッケージにて提供いたします。
ドキュメントについてもPS3_SDKDoc-160_01-Latest-*.zipに含めて提供いたします。

また、今回のパッケージには、以下のコンポーネントは含まれません。
  - PSSG
  - COLLADA_Pipeline

infoディレクトリ内に以下のものが含まれますので、必ず参照してください。

  - ソフトウェアセットアップガイド
  - Important_Changes： SDK160の新規機能、変更点、注意事項
  - Notice: 既知の不具合、制限事項
  - Bugfix: 不具合修正
  - PLAYSTATION(R)3 FAQ (*)
  - Referenceシステム 活用ガイド
  - Reference Tool Operating Instructions Appendix (*)
  - RefToolHW_operating_instructions/  以下
      DEH/    Reference Tool(DEH)の取扱説明書(日英同一ドキュメント)
      DECR/   Reference Tool(DECR)の取扱説明書
  - text/ 以下: テキスト版のImportant_Changes、Notice、Bugfix

(*) 内容の変更がありませんので、SDK154のドキュメントと同じファイルと
    なります。

このパッケージに含まれるソフトウェアをご使用の場合、別パッケージで
提供している以下のパッケージを必ずダウンロードしてください。

     PS3_CP_Update-150_004.zip         Reference Tool CP update file
     PS3_SDK_SampleData-160_008.zip    sample data 
     PS3_Toolchain-Win_160_008.zip     gcc/gdb Windows版
     PS3_Toolchain-Linux_160_008.zip   gcc/gdb Linux版
     PS3_SDKDoc-160_01-Latest-*.zip    ドキュメント
     PS3_Generator-115.zip             Disc Image Generator
     PS3_Eclipse_Plugins-150_004.zip   Eclipse Plugins
     PS3_PA_150_004.zip                Performance Analyzer Tools 
     PS3_at3tool-2000.zip              at3tool
     PAMF_Tools-100.zip                PAMF Tools
     MultiStream_Tools-135.zip         MultiStream Tools
     SEdit-1200.zip                    SEdit
     VAGconverter-1.0.zip              VAG Converter

Cell： IBMコーポレーション、ソニー株式会社、
       株式会社ソニー・コンピュータエンタテインメントおよび
       株式会社東芝の４社が共同開発中の高性能プロセッサのコードネーム。

----------------------------------------------------------------------
動作確認環境
----------------------------------------------------------------------
Linux環境
   Red Hat Professional Workstation (Linux 2.4.21)
   Red Hat Enterprise Linux 4 WS (Linux 2.6.9)
   SUSE Linux Professional 9.2 (Linux 2.6.8)

Windows環境
   Windows XP Professional Service Pack 2

----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
cell/
|---Readme_*.txt
|---version-SDK             SDKリリースバージョンのファイル
|---license                 商標およびライセンスファイル
|    |---license_*.txt
|    +---others
|---info
|    |---RefToolHW_operating_instructions
|    +---text
|---host-common
|---host-linux              Linux版 ユーティリティ
|---host-win32              Windows版 ユーティリティ
|---target
|    |---updater            Flash Memoryアップデートファイル
|    |---images             実行用イメージファイル（システムファイル類）
|    |---tool               libprof.sprx
|    |---common
|    |    +---include
|    |---ppu
|    |    |---include
|    |    +---lib
|    +---spu
|         |---include
|         +---lib
+---samples                 サンプルディレクトリ

----------------------------------------------------------------------
SDK160の環境設定
----------------------------------------------------------------------
パッケージを展開したら、はじめにコミュニケーションプロセッサ用
ソフトウェア更新を行う必要があります。
具体的な更新の手順については「Reference Toolソフトウェアセットアップガイド」
に記載されておりますので、以下の点に注意し、更新してください。

- SDK100以前の環境をご使用の場合は、一旦SDK100に含まれるebootromで
  Flash Memoryの書き込みを行ってください。

- SDK100以前の環境から直接SDK160へアップデートした場合の動作についての
  保障はいたしませんので、ご注意ください。

----------------------------------------------------------------------
使用許諾・制限
----------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

----------------------------------------------------------------------
商標およびライセンスについて
----------------------------------------------------------------------
開発ツールおよび本パッケージに含まれるソフトウェアの商標および
ライセンスについては、licenseフォルダ内のファイルを参照してください。

