[SCE CONFIDENTIAL DOCUMENT]
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.

sce-cgcdisasm

<Description>
    sce-cgcdisasm displays disassembled instructions from a binary
    file compiled by sce-cgc. 
    The -header option is available to output the parameter information 
    (included in the binary file) as a header file.
    When the parameter information is thus obtained, there is no need to 
    obtain the information at runtime using a function such as 
    cellGcmCgGetNamedParameter().

<Usage>
    The following messages are displayed by specifying the -h/--help/-help 
    option;

Usage: sce-cgcdisasm [-header] [-o outfile] <cgbinfile>
 -h --help -help     Display this output
 -v --version -V     Display this program's version number
 -q -quiet           Suppress printing to stderr
 -o <file>           Place console output into <file>
 -header             Output as header file format

    The -q option can be used to suppress stderr output.

    The -o option can be used to output disassemble information and 
    parameters information to the specified file. If unspecified, 
    information will only be output to the console screen.

    The -header option can be used to output parameter information in 
    the header file format (in addition to the normal console output). 
    The name of the output file is the name of the input file with the 
    file extension changed to _params.h, and it is placed in the same 
    directory as the input file.
    (For example, the file vpshader_params.h is output for vpshader.vpo.)
    In the current version, the name of the output file cannot be changed.

<Example>
    sce-cgcdisasm fpshader.fpo
    (fpshader.fpo is disassembled and displayed on the console.)

    sce-cgcdisasm -header fpshader.fpo
    (fpshader.fpo is disassembled and displayed on the console. 
    Also, parameter information is output as fpshader_params.h.)

<Explanation of Output>
    Refer to the document "sce-cgc Toolchain User's Guide" in the SDK.

<Effective Usage of the Tool>
    Refer to the document "sce-cgc Toolchain User's Guide" in the SDK.

<Features to Be Added Later>
    None