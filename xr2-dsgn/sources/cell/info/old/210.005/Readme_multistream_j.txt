[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 210.005
                    Copyright(C) 2008 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
このパッケージは、SDK210.001 パッケージ (PS3_SDK-210_001.zip) の
MultiStream に不具合修正および機能追加を行ったものです。


SDK210.001の関連パッケージを展開した環境で、本パッケージを展開して
ご使用ください。

----------------------------------------------------------------------
パッケージ構成
----------------------------------------------------------------------
cell/
|---Readme_multistream_*.txt
+---target
     +---ppu
          |---include
          |    +---cell
          |         +---mstream.h
          +---lib
               |---libmstreamSPURSAT3.a
               |---libmstreamSPURSMP3.a
               |---libmstreamThreadAT3.a
               +---libmstreamThreadMP3.a

----------------------------------------------------------------------
変更点
----------------------------------------------------------------------
<新規機能>
- cellMSStreamGetMP3FrameInfo()を追加しました。
  cellMSStreamGetMP3FrameInfo()の関数リファレンス情報を以下に示します。

  #include <cell/mstream.h>
  int cellMSStreamGetMP3FrameInfo(
                                    int streamNum,
                                    int *numFrames,
                                    int *totalSizeOfFrames
                                      );

  呼び出し条件
  ============
  - 割り込みハンドラから呼び出し可能。マルチスレッドセーフ。

  引数
  ====
  streamNum        : 情報取り出し先の MP3 ストリームチャネル番号。
  numFrames        : 再生された MP3 フレーム数を返す。
  totalSizeOfFrames: 再生されたフレームの累計サイズをバイト単位で返す。

  返り値
  ======
  Value  :  説明
   0     :  成功
  -1     :  エラー

  * -1 が返った場合は、cellMSSystemGetLastError() を使って詳細な
    エラーメッセージを取得してください。 


  解説
  ====
  - 最後に cellMSStreamPlay() コマンドが呼び出されてからストリーム上で
    再生された MP3 フレーム数を取得します。
    この情報を使うと、MP3 ストリームの再生時間を正確に計算することが
    できます。


<不具合修正>
- (B#47418)
  DSPのリバーブエフェクトを使用すると極まれに(およそ1週間に1回の頻度で)
  スタックバッファがオーバーフローを引き起こす不具合を修正しました。

- (B#47530)
  タイムドメインモードのいずれか（CELL_MS_TD_ONLY_*）を使用する場合、
  サブバスからのストリームが間違ったチャンネルに出力してしまうという不具合を
  修正しました。

- (B#47535)
  タイムドメインのみのエンベローブの分解能が小さすぎるという問題を
  改善しました。

- (B#47536)
  Multistreamの処理が次のSPUサイクルまでポーリングされた場合、
  cellMSStreamGetMP3FramePos()で無効な値を返してしまう不具合を修正しました。

- (B#47538)
  MP3のデコード処理をSPU側ですぐに開始しするとMP3再生における異常処理を
  引き起こしてしまうという不具合を修正しました。

----------------------------------------------------------------------
libmstreamの既知の不具合・制限事項・注意事項
----------------------------------------------------------------------
(Release 210.001 追加)
- cell/samples/sdk/sound/multistream/dspeffects 以下に *.pic ファイルが
  存在していますが、ゲームアプリケーションでは、
  cell/target/spu/lib/pic/multistream 以下の PPU のオブジェクト
  ファイル(*.ppu.o)を使用してください。

(Release 210.001 追加)
- cellMSSystemConfigureSysUtil()は、今後廃止を予定していますので、
  ゲームアプリケーションでは、SDK200.002 で追加しました
  cellMSSystemConfigureSysUtilEx()への移行をお願いします。

- タイムドメインモードにおいて、24khzより低いサンプリング周波数のADPCM
  データはノイズが発生してしまう不具合があります。

----------------------------------------------------------------------
使用許諾・制限
----------------------------------------------------------------------
このソフトウェアの使用許諾、使用制限は貴社と当社(株式会社ソニー・
コンピュータエンタテインメント)との間に締結されている契約に準じます。

----------------------------------------------------------------------
商標に関する注意書き
----------------------------------------------------------------------
"PLAYSTATION"は、株式会社ソニー・コンピュータエンタテインメント
の登録商標です。

"DTS" is a registered trademark of DTS, Inc.

パッケージ内に記載されている会社名、製品名は一般に各社の商標または
登録商標です。
なお、本文中に(R)、(TM)マークは明記していない場合があります。

