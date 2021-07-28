[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 192.001
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、RSX(R)搭載のハードウェア(Reference Tool)による
アプリケーションソフトの開発を行うために必要なソフトウェアが含まれています。

パッケージには、以下のコンポーネントは含まれません。

  プログラマー向けパッケージ
  ==========================
    PS3_SPUsim-XXX_XXX.zip              SPU Simulator

infoディレクトリ内に以下のものが含まれますので、必ず参照してください。
  - ソフトウェアセットアップガイド  (*)
  - Important_Changes： SDK192の新規機能、変更点、注意事項
  - Notice: 既知の不具合、制限事項
  - Bugfix: 不具合修正
  - Referenceシステム 活用ガイド  (*)
  - PLAYSTATION(R)3 Debugging Station ソフトウェアセットアップガイド  (*)
  - Reference Tool Operating Instructions Appendix  (*)
  - RefToolHW_operating_instructions/DECR/ Reference Tool(DECR)の取扱説明書  (*)
  - text/ 以下: テキスト版のImportant_Changes、Notice、Bugfix

(*) 内容の変更がありませんので、SDK190のドキュメントと同じファイルと
    なります


このパッケージに含まれるソフトウェアをご使用の場合、別パッケージで
提供している以下のパッケージを必ずダウンロードしてください。

  プログラマ向けパッケージ
  ==========================
     PS3_CP_Update-190_002.zip                  Reference Tool CP update file
     PS3_SDK_Sample-192_001.zip                 サンプルプログラムとサンプルデータ 
     PS3_Toolchain-Win(/Linux)_402_190_002.zip  gcc 402/gdb Windows版 及び Linux版
     PS3_Toolchain-Win(/Linux)_411_190_002.zip  gcc 411/gdb Windows版 及び Linux版
     PS3_SDKDoc-192_01-Latest-*.zip             ドキュメント
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins
     PS3_PASuite-190_002.zip                    Performance analyzer tools

     edge-030.zip                               PLAYSTATION(R)Edge
     PS3_PSSG-120.zip                           PSSG
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  アーティスト向けパッケージ
  ==========================
  - グラフィックス関連
      PS3_PSSGArtTools-120.zip          PSSGArt Tools
      FX_Composer2-20.zip               FX Composer2

   - サウンド関連
      MultiStream_Tools-136.zip         MultiStream Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      SEdit-1200.zip                    SEdit
      VAGconverter-1.0.zip              VAG Converter
      PS3_at3tool-2000.zip              at3tool

  - Codec関連
      PAMF_Tools-250-*.zip              PAMF Tools
      PAMF_Tools-Command_Line-250.zip   PAMF Tools(Command Line) 

  ディスク作成用パッケージ
  ========================
     PS3_Generator-152.zip              Disc Image Generator
     PS3_Checker-152.zip                Disc Image Checker

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
+---target
     |---updater            Flash Memoryアップデートファイル
     |---images             実行用イメージファイル（システムファイル類）
     |---tool               libprof.sprx
     |---common
     |    +---include
     |---ppu
     |    |---include
     |    +---lib
     +---spu
          |---include
          +---lib


----------------------------------------------------------------------
SDK192の環境設定
----------------------------------------------------------------------
パッケージを展開したら、はじめにコミュニケーションプロセッサ用
ソフトウェア更新を行う必要があります。
具体的な更新の手順については「Reference Toolソフトウェアセットアップガイド」
に記載されておりますので、以下の点に注意し、更新してください。

- SDK100以前の環境をご使用の場合は、一旦SDK100に含まれるebootromで
  Flash Memoryの書き込みを行ってください。

- SDK100以前の環境から直接SDK192へアップデートした場合の動作についての
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

