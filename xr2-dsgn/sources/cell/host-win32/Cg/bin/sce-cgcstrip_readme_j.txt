[SCE CONFIDENTIAL DOCUMENT]
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.
======================================================================

sce-cgcstrip

<ツールの解説>
    sce-cgcでコンパイルされたバイナリファイルから不要な情報を削除して
    ファイルサイズを削減するツールです。
    使用されていないパラメータを削除したり、使用・未使用に関らず
    パラメータ名やセマンティック名などの文字列を削除することが可能です。

<ツールの使用方法>
    -h/--help/-helpのいずれかで下記のメッセージが出力されます。

Usage: sce-cgcstrip <option(s)> input_compiled_cg_binary
 Removes strings and unused parameters from file
 The options are:
  -h --help -help        Display this output
  -v --version -V        Display this program's version number
  -o <file>              Place stripped output into <file>
  -name                  Remove all name strings
  -semantic              Remove all semantic strings
  -param                 Remove all unused parameters
  -varying               Remove all varying parameters
  -nodefault             Remove all parameters have no default value
  -sampler               Remove all sampler parameters
  -a --all -A            Remove all strings and parameters
  -k --keep -K <param>   Keep specified parameter

    -o オプションで出力ファイル名を指定することができます。
    指定しない場合は入力ファイルに上書きしますので注意してください。

    ほかのオプションの機能と利用方法については、SDKに含まれるドキュメント
    「sce-cgcツールチェーン ユーザガイド」を参照してください。

<効果的な利用方法>
    -paramオプションを使用すると、シェーダ内で使用されていないパラメータ
    の情報を削除して、バイナリファイルのサイズを小さくすることができます。

    本ツールをsce-cgcdisasmと併せて利用するとより効果的です。
    sce-cgcdisasmでパラメータの情報を取得しておくと、その情報を使って
    パラメータにアクセスできるため、実行時にcellGcmGetNamedParameter()
    などの関数を使用する必要がなくなるとともに、パラメータ名をバイナリ
    ファイルに含めておく必要もなくなります。不要になったパラメータ名など
    を削除すると、バイナリファイルのサイズをさらに小さくできます。

    利用方法の詳細は、SDKに含まれるドキュメント「sce-cgcツールチェーン 
    ユーザガイド」を参照してください。

<将来予定している追加機能>
    特に無し