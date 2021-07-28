[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 210.001
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、RSX(R)搭載のハードウェア(Reference Tool)による
アプリケーションソフトの開発を行うために必要なソフトウェアが含まれています。

//////////////////////////////////////////////////////////////////////
(2008/01/07 更新)
- SDKパッケージに含まれているPS3ToolUpdater.self(/usr/local/cell/target/images)
  を更新しました。
  変更点は以下の通りです。
  - Debugging Stationのシステムソフトウェアを正しくアップデート
    出来なかった点を修正しました。
  - システムソフトウェア2.10からダウングレードする際にレジストリ
    の削除を行うようにしました。
    以下の技術情報に対する対策です。
    https://ps3.scedev.net/technotes/view/664
  (注意)
    Debugging Stationのシステムソフトウェアが2.10の状態の時に
    新しいPS3ToolUpdater.selfを用いてダウングレードを行うと
    レジストリの削除と共にネットワーク設定が初期化されます。
    その為、ダウングレード後にDebugging Stationの再起動を行う
    と一時的にデバッガが接続出来ない状態になります（画面はブラ
    ックアウトしたままです）。
    2.10からダウングレードを行った場合は必ずフロントのPOWER
    スイッチを５秒以上押し続けてシステムソフトウェアから起動
    するようにして下さい。
    システムソフトウェアが起動した後はシステムソフトウェアから
    ネットワーク設定を行って頂ければ以前と同様にデバッガを使用
    する事が可能です。
    なお、Reference ToolではDEV LAN(開発用ネットワーク)を使用
    する為、一時的にデバッガが使用出来なくなる事はありません。
    但し、Debugging Stationと同様にLAN側のネットワークは初期化
    されますので、再度、システムソフトウェアからネットワークの
    設定を行って下さい。

(2008/01/07 更新)
- 以下のドキュメントにcgc関連の変更点・制限事項を追記しました。
  info/Important_Changes_*.pdf
  info/Notice_SDK_*.pdf
  info/Bugfix_SDK_*.pdf
  info/text/Important_Changes_*.txt
  info/text/Notice_SDK_*.txt
  info/text/Bugfix_SDK_*.txt
//////////////////////////////////////////////////////////////////////

cell/info、cell/SDK_doc ディレクトリ内に以下のものが含まれますので、
必ず参照してください。

  [cell/info/以下のドキュメント]
   - Important_Changes： SDK210の新規機能、変更点、注意事項
   - Notice: 既知の不具合、制限事項
   - Bugfix: 不具合修正

  [cell/info/text/以下のドキュメント]
   - テキスト版のImportant_Changes、Notice、Bugfix

  [cell/SDK_doc/jp(en)/pdf/development_basic以下のドキュメント]
   - Referenceシステム 活用ガイド
   - ソフトウェアセットアップガイド
   - PLAYSTATION(R)3 Debugging Station ソフトウェアセットアップガイド
   - Reference Tool Instruction Manual Appendix
   - Reference Tool(DECR)の取扱説明書  (*)

   (*) SDK200のドキュメントからの内容の変更はありません。

このパッケージに含まれるソフトウェアをご使用の場合、別パッケージで
提供している以下のパッケージを必ずダウンロードしてください。

  プログラマ向けパッケージ
  ==========================
     PS3_CP_Update-210_001.zip                  Reference Tool CP update file
     PS3_SDK_Samples-210_001.zip                サンプルプログラムとサンプルデータ 
     PS3_Toolchain-Win(/Linux)_402_210_001.zip  gcc 402/gdb Windows版 及び Linux版
     PS3_Toolchain-Win(/Linux)_411_210_001.zip  gcc 411/gdb Windows版 及び Linux版
     PS3DECRUpdateData.210.001.zip              Reference Tool用アップデートデータ
     PS3_SDKDoc-210_01-Latest-*.zip             ドキュメント
     PS3_Eclipse_Plugins-180_002.zip            Eclipse Plugins
     PS3_PASuite-200_002.zip                    Performance analyzer tools

     edge_0_3_4.zip                             PLAYSTATION(R)Edge
     PS3_PSSG-130.zip                           PSSG
     PS3_COLLADA-141_DOM-201.zip                COLLADA DOM
     PS3_COLLADA-Pipeline-190_002.zip           COLLADA Pipeline

  アーティスト向けパッケージ
  ==========================
  - グラフィックス関連
      PS3_PSSGArtTools-130.zip          PSSGArt Tools
      FX_Composer2-24.zip               FX Composer2.4

   - サウンド関連
      MultiStream_Tools-138.zip         MultiStream Offline Tools
      SCREAM_Tools-101.zip              SCREAM Tools
      SEdit-1200.zip                    SEdit
      VAGconverter-1.0.zip              VAG Converter
      PS3_at3tool-2000.zip              at3tool

  - Codec関連
      PAMF_Tools-310-*.zip              PAMF Tools
      PAMF_Tools-Command_Line-310.zip   PAMF Tools(Command Line) 

  ディスク作成用パッケージ
  ========================
     PS3_Generator-175.zip              Disc Image Generator
     PS3_Checker-175.zip                Disc Image Checker

なお、上記パッケージリストには、以下のコンポーネントは含まれません。

  プログラマー向けパッケージ
  ==========================
    PS3_SPUsim-XXX_XXX.zip              SPU Simulator


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
|    |---*.pdf
|    +---text/*.txt
|---SDK_doc/jp(en)/pdf/
|    +---development_basic
|
|---host-common
|---host-linux              Linux版 ユーティリティ
|---host-win32              Windows版 ユーティリティ
+---target
     |---updater            Flash Memoryアップデートファイル用のディレクトリ
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
SDK210の環境設定
----------------------------------------------------------------------
パッケージを展開したら、はじめにコミュニケーションプロセッサ用
ソフトウェア更新を行う必要があります。
具体的な更新の手順については「Reference Toolソフトウェアセットアップガイド」
に記載されておりますので、以下の点に注意し、更新してください。

- SDK100以前の環境をご使用の場合は、一旦SDK100に含まれるebootromで
  Flash Memoryの書き込みを行ってください。

- SDK100以前の環境から直接SDK210へアップデートした場合の動作についての
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

