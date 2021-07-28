[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 160.008
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                  All Rights Reserved.
======================================================================
This package includes libraries, compiler, and documents for 
the general-purpose binary data format CXML conversion tool. 

A CXML file is created by converting a XML file. 
The compiler to be used for the conversion is provided in 
cell/samples/cxml/compiler/cxml.py as a Python script. 
For the method of using it, please refer to 
cell/samples/cxml/compile/readme.txt and the document "CXML Overview". 

In addition, cxmlaccess library and cxmlutil library are used for 
a title application to access a CXML file when it is executed. 
For details, please refer to the document "CXML Overview". 

Please use this package by expanding it in the environment of 
PLAYSTATION(R)3 Programmer Tool Runtime Library SDK 160.008. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_CXML_*.txt
|---SDK_doc/     Documents
|    +---jp/en
|          +---pdf/cxml
|                |---CXML-Overview_*.pdf
|                +---CXML-Reference_*.pdf
+---samples
     +---cxml
          |---compiler
          |    +---cxml.py  CXML compiler (Python script)
          |---compile       Sample for creating CXML compiler
          |---include
          |    |---cxmlaccess.h
          |    +---cxmlutil.h
          |---lib           Library source codes
          |    +---src
          |         |---cxmlaccess   cxmlaccess Library
          |         +---cxmlutil     cxmlutil Library
          |---makefile
          |---makefile_base
          |---read
          +---write

----------------------------------------------------------------------
Environment for Operation Check
----------------------------------------------------------------------
Linux Environment
   Red Hat Professional Workstation (Linux 2.4.21)
   Red Hat Enterprise Linux 4 WS (Linux 2.6.9)
   SUSE Linux Professional 9.2 (Linux 2.6.8)

Windows Environment
   Windows XP Professional Service Pack 2

PLAYSTATION(R)3 Programmer Tool Runtime Library 160.008

----------------------------------------------------------------------
Permission and Restrictions on Use
----------------------------------------------------------------------
The permission and restrictions on using this software conform to 
the contract concluded between your company and our company (Sony 
Computer Entertainment Inc). 

----------------------------------------------------------------------
Note on Trademarks
----------------------------------------------------------------------
"PLAYSTATION" is a registered trademark of Sony Computer Entertainment Inc.
All other product and company names mentioned herein, with or without 
the registered trademark symbol (R) or trademark symbol (TM), are 
generally registered trademarks and/or trademarks of their respective 
owners. 

