[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 190.002
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、RSX(R)搭載のハードウェア(Reference Tool)による
アプリケーションソフトの開発を行うために必要なソフトウェアが含まれています。

パッケージには、以下のコンポーネントは含まれません。
  - Performance Analyzer Tools  
    (PS3_PASuite-190_002.zipとPS3_SPUsim-190_002.zipに分離し、
     近日中にリリース致します)

infoディレクトリ内に以下のものが含まれますので、必ず参照してください。

  - ソフトウェアセットアップガイド
  - Important_Changes： SDK190の新規機能、変更点、注意事項
  - Notice: 既知の不具合、制限事項
  - Bugfix: 不具合修正
  - Referenceシステム 活用ガイド
  - PLAYSTATION(R)3 Debugging Station ソフトウェアセットアップガイド
  - Reference Tool Operating Instructions Appendix
  - RefToolHW_operating_instructions/DECR/ Reference Tool(DECR)の取扱説明書
  - text/ 以下: テキスト版のImportant_Changes、Notice、Bugfix

このパッケージに含まれるソフトウェアをご使用の場合、別パッケージで
提供している以下のパッケージを必ずダウンロードしてください。
SDK190より、cell/samples以下のサンプルプログラムは、従来のサンプルデータ
パッケージ(PS3_SDK_SampleData-XXX_XXX.zip)と統合し、PS3_SDK_Sample-190_002.zip
として提供することになりました。

     PS3_CP_Update-190_002.zip         Reference Tool CP update file
     PS3_SDK_Sample-190_002.zip        サンプルプログラムとサンプルデータ 
     PS3_Toolchain-Win_402_190_002.zip   gcc 402/gdb Windows版
     PS3_Toolchain-Linux_402_190_002.zip gcc 402/gdb Linux版
     PS3_Toolchain-Win_411_190_002.zip   gcc 411/gdb Windows版
     PS3_Toolchain-Linux_411_190_002.zip gcc 411/gdb Linux版
     PS3_SDKDoc-190_01-Latest-*.zip    ドキュメント
     PS3_Generator-150.zip             Disc Image Generator
     PS3_Checker-150.zip               Disc Image Checker
     PS3_Eclipse_Plugins-180_002.zip   Eclipse Plugins
     PS3_at3tool-2000.zip              at3tool
     PS3_PSSG-120.zip                  PSSG
     PS3_PSSGArtTools-120.zip          PSSGArt Tools
     PS3_COLLADA-Pipeline-190_002.zip  COLLADA Pipeline
     MultiStream_Tools-136.zip         MultiStream Tools
     SEdit-1200.zip                    SEdit
     VAGconverter-1.0.zip              VAG Converter
     SCREAM_Tools-101.zip              SCREAM Tools
     PAMF_Tools-250-*.zip              PAMF Tools
     PAMF_Tools-Command_Line-250.zip   PAMF Tools(Command Line) 

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
SDK190の環境設定
----------------------------------------------------------------------
パッケージを展開したら、はじめにコミュニケーションプロセッサ用
ソフトウェア更新を行う必要があります。
具体的な更新の手順については「Reference Toolソフトウェアセットアップガイド」
に記載されておりますので、以下の点に注意し、更新してください。

- SDK100以前の環境をご使用の場合は、一旦SDK100に含まれるebootromで
  Flash Memoryの書き込みを行ってください。

- SDK100以前の環境から直接SDK190へアップデートした場合の動作についての
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

