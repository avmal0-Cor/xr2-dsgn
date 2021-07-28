[SCE CONFIDENTIAL DOCUMENT]
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
======================================================================

sce-cgcdisasm

<ツールの解説>
    sce-cgcでコンパイルされたバイナリファイルをディスアセンブルして
    表示するツールです。バイナリファイル内に記述されているパラメータ
    情報をヘッダファイルとして出力するオプションがあります。
    パラメータ情報をあらかじめ取得しておくことで、ランタイム時に
    cellGcmCgGetNamedParameter()などの関数でパラメータ情報を取得する
    必要がなくなります。

<ツールの使用方法>
    -h/--help/-helpのいずれかで下記のメッセージが出力されます。

Usage: sce-cgcdisasm [-header] [-o outfile] <cgbinfile>
 -h --help -help     Display this output
 -v --version -V     Display this program's version number
 -q -quiet           Suppress printing to stderr
 -o <file>           Place console output into <file>
 -header             Output as header file format

    -q オプションを指定することで、stderrへの出力が非表示になります。

    -o オプションでファイル名を指定することで、コンソール画面に表示
    されるディスアセンブル情報とパラメータ情報をそのファイルに出力する
    ことができます。
    指定しない場合はコンソール画面上にのみ表示されます。

    -header オプションを指定することで、通常の画面出力に加えてパラメー
    タ情報をヘッダファイル形式で出力します。出力先のファイル名は、入力
    ファイルの拡張子を_params.hに置き換えたファイル名で、入力ファイル
    と同じディレクトリに出力されます。
    (例 vpshader.vpo の場合はvpshader_params.hとなる)
    現バージョンでは出力ファイル名を変更することはできません。

<使用例>
    sce-cgcdisasm fpshader.fpo
    （fpshader.fpoをディスアセンブルしてコンソール画面に表示します）

    sce-cgcdisasm -header fpshader.fpo
    （fpshader.fpoをディスアセンブルしてコンソール画面に表示し、さらに
      パラメータ情報をfpshader_params.hに出力します）

<出力の見方>
    SDKに含まれているドキュメント「sce-cgcツールチェーン ユーザガイド」を
    参照してください。

<効果的な利用方法>
    SDKに含まれているドキュメント「sce-cgcツールチェーン ユーザガイド」を
    参照してください。

<将来予定している追加機能>
　　現時点では特に無し
