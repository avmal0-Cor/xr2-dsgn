==================================================================

　TOOL 専用アップデートプログラム(PS3ToolUpdater.self)の説明書

==================================================================

■変更履歴
============
　2007.02/15 ksato/ximai :  初版作成。


■概要
========
　ターミナル（または ProDG）から TOOL をアップデートするプログラム。
　通常 GUI アップデートで使っている PS3UPDAT.PUP を使用する。GUI 操作がない、
　リモート起動できる、複数起動も可能、などより効率的に TOOL をアップデート
　できる。
　また、GUI アップデートと同じスキームを使っているので、100 以降既に作成されて
　いる PS3UPDAT.PUP も使用することができる。

　※複数起動方法などは「＊＊＊」をご参照下さい。


■アップデート手順
====================

　●前準備

　　０．TOOL をデバッグ起動するようにしておく。

　　　　$ dtcparam boot=dbg

　●アップデート

　　１．PS3ToolUpdater.self と TOOL 用の PS3UPDAT.PUP を /app_home にコピー
　　　　する。

　　２．プログラムを実行する。

　　　　$ bedbg -nodebug PS3ToolUpdater.self

　　　　/app_home にある PS3UPDAT.PUP を使ってアップデート開始する。

　●dtccons への表示

　　プログラム実行時には dtccons に以下のような出力が表示される。

　　lv2(2): 
　　lv2(2): PS3 TOOL Updater Version 1.0.0 (Feb 14 2007)
　　lv2(2): Copyright(C) 2007 Sony Computer Entertainment Inc. All Rights Reserved.
　　lv2(2): 
　　lv2(2): version 1.50 -> 1.60
　　lv2(2): 
　　lv2(2): Copy a update file ..........................................................
　　lv2(2): Reboot now !
　　lv2(2): 
　　lv2(2): Prepare to shutdown ......
　　lv2(2): Going to shutdown.lv2(0): total memory size: 502MB
　　lv2(0): kern memory size:   24MB (heap:9188KB  page pool:10240KB)
　　lv2(0): user memory size:  462MB
　　lv2(2): 
　　lv2(2): Cell OS Lv-2 32 bit version 1.5.0
　　lv2(2): Copyright 2006 Sony Computer Entertainment Inc.
　　lv2(2): All Rights Reserved.
　　lv2(2): 
　　lv2(2): revision: 1****
　　lv2(2): build date: 200*/**/** **:**:**
　　lv2(2): processor: Broadband Engine  Ver 0x0000  Rev 0x0201
　　lv2(2): PPU:0, Thread:0 is enabled.
　　lv2(2): PPU:0, Thread:1 is enabled.
　　lv2(2): 
　　lv2(2): mounting HOSTFS in default mount point "/app_home" : OK
　　lv2(2): mounting HOSTFS in default mount point "/host_root" : OK
　　lv2(2): Available physical SPUs: 6
　　lv2(2): mounting the flash file system : OK
　　lv2(2): 
　　lv2(2): ###
　　lv2(2): ### Software update mode
　　lv2(2): ###
　　lv2(2): 
　　lv2(2): mounting the builtin HDD1 : OK
　　lv2(2): creating the initial system process : OK
　　lv2(2): 
　　lv2(2): PS3 TOOL Updater Version 1.0.0 (Feb 14 2007)
　　lv2(2): Copyright(C) 2007 Sony Computer Entertainment Inc. All Rights Reserved.
　　lv2(2): 
　　lv2(2): Updating...
　　lv2(2):  Item [ 1] : .
　　lv2(2):  Item [ 2] : ....
　　lv2(2):  Item [ 3] : ....................................
　　lv2(2):  Item [ 4] : .........
　　lv2(2):  Item [ 5] : ............
　　lv2(2):  Item [ 6] : ...................
　　lv2(2):  Item [ 7] : .............................
　　lv2(2):  Item [ 8] : ................
　　lv2(2):  Item [ 9] : ..............
　　lv2(2):  Item [10] : .
　　lv2(2): BDemulator: disabled
　　lv2(2):  Item [11] : ................................
　　lv2(2):  Item [12] : ...........
　　lv2(2):  Item [13] : .
　　lv2(2): 
　　lv2(2): Update complete!
　　lv2(2): Reboot now!
　　lv2(2): 

