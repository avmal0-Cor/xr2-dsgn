[SCE CONFIDENTIAL DOCUMENT]
PlayStation(R)3 Programmer Tool Runtime Library 310.001
                  Copyright (C) 2007 Sony Computer Entertainment Inc.
                                                 All Rights Reserved.


<Sample Overview>
    This sample uses the libgcm offline tool library.
    It generates commands described in the template class into the 
    binary file.
    It obtains the required command size in the Measure mode and 
    generates commands in the Unsafe mode.


<Files>
    The file structure of the offline tool library are as shown 
    below:

    $(CELL_SDK)/host-common/include/gcm_tool.h	Common header file
    $(CELL_SDK)/host-linux/lib/libgcm_tool.a	Library for Linux
    $(CELL_SDK)/host-win32/lib/libgcm_tool.lib	Library for Windows
    $(CELL_SDK)/host-common/samples/gcm_tool/	Sample directory

    The command generation function provided by the offline tool library 
    is equivalent to the library for SPU.
    How to use the command generation function is also same as the 
    Runtime library.
    Please refer to the libgcm document for more details.


<How to compile>

    Please compile as the following procedure:

    Linux:
     -Set the environment variable
      Set the path in which the CELL SDK is installed.
      % export CELL_SDK=/usr/local/cell

     -Execute the compilation
      % cd host-common/samples/gcm_tool
      % make

    Windows:
     -Install CELL SDK
      CELL SDK should be installed under c:\usr\local\cell.
      If the path of the installation destination differs, 
      edit the path of the Project file of VisualStudio so that 
      it matches with your environment.

    -Load the solution file
      Move to host-common/samples/gcm_tool.
      Load the solution file "simple.sln" to the folder from
      VisualStudio.
      Build the sample via "Build Solution" of VisualStudio.


<How to execute the program>
    Execute the executable file "simple" from the command line.
    When it is executed without an argument, the default output file 
    "out.gcm" is output to the current directory.
    An output file name can be specified to the first argument.

     % simple <File name>

     When "-h" is specified to the first argument, Help is displayed.

     % simple -h


<Customizing the sample program>
    In this sample program, commands to be generated are described 
    in "template.h" that defines the template class.
    To change the commands to be generated, customize the commands 
    described in this template class.
    To realize more complex processing, you can introduce your own 
    customization based on the sample program.
