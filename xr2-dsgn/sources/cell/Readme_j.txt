[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 320.001
                    Copyright(C) 2010 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、RSX(R)搭載のハードウェア(Reference Tool)による
アプリケーションソフトの開発を行うために必要なソフトウェアが含まれています。

このパッケージに含まれるソフトウェアをご使用の場合、SDK310の関連パッケージを
展開した環境で、本パッケージを展開してご使用ください。

cell\info、cell\SDK_doc ディレクトリ内に以下のものが含まれますので、
必ず参照してください。

  [cell\info\以下のドキュメント]
   - Important_Changes： SDK320の新規機能、変更点、注意事項
   - Notice: 既知の不具合、制限事項
   - Bugfix: 不具合修正

  [cell\info\old\XXX 以下のドキュメント]  (XXX:SDKリリースバージョン)
   - SDK310以前にリリースしたSDKリリースバージョン毎の変更点に関するファイル
      - Readme, Important_Changes, Notice_SDK, Bugfix_SDK, 
        Changes_Toolchain, Notice_Toolchain

  [cell\SDK_doc\jp(en)\pdf\development_basic以下のドキュメント]
   - Referenceシステム 活用ガイド
   - Reference Tool ソフトウェアセットアップガイド
   - Reference Tool DECR-1400 ソフトウェアセットアップガイド
   - Debugging Station ソフトウェアセットアップガイド
   - Reference Tool Instruction Manual Appendix
   - Reference Tool(DECR-1000)の取扱説明書
   - Reference Tool(DECR-1400)の取扱説明書
   - Debugging Station(DECHJ00J/DECHJ00A)の取扱説明書
   - Debugging Station(DECHA00J/DECHA00A)の取扱説明書
   - Debugging Station(DECH-2000J/DECH-2000A)の取扱説明書

このパッケージに含まれるソフトウェアをご使用の場合、別パッケージで
提供している以下のパッケージを必ずダウンロードしてください。

  プログラマ向けパッケージ
  ==========================
     PS3_CP_Update-240_001.zip                  Reference Tool CP update file
     PS3_SDK-310_001.zip                        ライブラリパッケージ
     PS3_SDK_Samples-310_001.zip                サンプルプログラムとサンプルデータ 
     PS3_SDK_Samples-320_001.zip                サンプルプログラムとサンプルデータ 
     PS3_SDK_CGTUTORIAL-240_001.zip             CgTutorialサンプル
     PS3_Toolchain_411-Win_310_001.zip          gcc 411/gdb Windows版
     PS3DECRUpdateData.320.001.zip              Reference Tool用アップデートデータ
     PS3_SDKDoc-320_01-Latest-*.zip             ドキュメント
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins

     edge_1_1_1.zip                             PlayStation(R)Edge
     PS3_PhyreEngine-260.zip                    PhyreEngine(TM)
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  アーティスト向けパッケージ
  ==========================
  - グラフィックス関連
      PS3_PhyreEngineArtTools-260.zip           PhyreEngine(TM)Art Tools
      PS3_GPAD-SDK310.2_8.5_Installer.zip       GPAD
      FX_Composer2-242.zip                      FX Composer2.4.2

   - サウンド関連
      Sulpha-10.zip                     Sulpha MultiStream Debugger
      PS3_VAGPreviewer-1100.zip         VAG Previewer 
      PS3_celptool-110.zip              celptool
      PS3_at3tool-3000.zip              at3tool
      MultiStream_Tools-141.zip         MultiStream Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      VAGconverter-1.0.zip              VAG Converter
      SEdit-1200.zip                    SEdit


  - Codec関連
      PAMF_Tools-510.zip                PAMF Tools
      PAMF_Tools-Command_Line-510.zip   PAMF Tools(Command Line) 

  ディスク作成用パッケージ
  ========================
     PS3_Generator_Tools-271.zip        Disc Image Generator Tools
     PS3_Checker-271.zip                Disc Image Checker


Cell： IBMコーポレーション、ソニー株式会社、
       株式会社ソニー・コンピュータエンタテインメントおよび
       株式会社東芝の4社が共同開発中の高性能プロセッサのコードネーム。

----------------------------------------------------------------------
動作確認環境
----------------------------------------------------------------------
Windows環境
   Windows XP Professional Service Pack 2

----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
cell\
|---Readme_*.txt
|---version-SDK             SDKリリースバージョンのファイル
|---license                 商標およびライセンスファイル
|    |---license_*.txt
|    +---others
|---info
|    |---*.txt
|    +---old\XXX
|---SDK_doc\jp(en)\pdf\
|    +---development_basic
|
|---host-common
|---host-win32              Windows版 ユーティリティ
+---target
     |---updater            Flash Memoryアップデートファイル用のディレクトリ
     |---images             実行用イメージファイル（システムファイル類）
     |---common
     |    +---include
     |---ppu
     |    |---include
     |    +---lib
     +---spu
          |---include
          +---lib


----------------------------------------------------------------------
SDK320の環境設定
----------------------------------------------------------------------
パッケージを展開したら、はじめにコミュニケーションプロセッサ用
ソフトウェア更新を行う必要があります。
具体的な更新の手順については「Reference Toolソフトウェアセットアップガイド」
に記載されておりますので、以下の点に注意し、更新してください。

- SDK100以前の環境をご使用の場合は、一旦SDK100に含まれるebootromで
  Flash Memoryの書き込みを行ってください。

- SDK100以前の環境から直接SDK320へアップデートした場合の動作についての
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

