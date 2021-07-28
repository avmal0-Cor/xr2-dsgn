[SCE CONFIDENTIAL DOCUMENT]
                  Copyright (C) 2008 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.

sce-cgcstrip

<Description>
    This tool deletes unnecessary information from binary files compiled 
    with sce-cgc and reduces the file size.
    Unused parameters can be deleted, and it is also possible to delete
    strings such as parameter names and semantic names regardless of 
    their usage status.

<Usage>
   The following messages are displayed by specifying the -h/--help/-help 
   option;

Usage: sce-cgcstrip <option(s)> input_compiled_cg_binary
 Removes strings and unused parameters from a file
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

   -o option specifies the output file name. Please note that the input 
    file will be overwritten when -o option is not specified.

    For information regarding all other options, refer to the document
    "sce-cgc Toolchain User's Guide" in the SDK.

<Effective Usage of the Tool>
    With the -param option, information of parameters that are unused
    in the shader can be deleted to reduce the size of the binary file.
    
    It is effective to use sce-cgcstrip together with sce-cgcdisasm.
    sce-cgcdisasm can be used beforehand to obtain information of 
    parameters. Because this information can be used to access the 
    parameters, it is not necessary to call functions such as 
    cellGcmGetNamedParameter() at runtime. Also this information 
    makes it unnecessary to include parameter names in the binary file.
    By deleting the unnecessary parameter names, the size of the 
    binary file can be further reduced.

    For more information regarding using the tool, refer to the document
    "sce-cgc Toolchain User's Guide" in the SDK.

<Features to Be Added Later>
   None