[SCE CONFIDENTIAL DOCUMENT]
PLAYSTATION(R)3 Programmer Tool Runtime Library 180.007
                    Copyright(C) 2007 Sony Computer Entertainment Inc.
                                                   All Rights Reserved.
======================================================================
This package includes new features and bug fixes of cgc 
in addition to SDK180.002 package (PS3_SDK-180_002.zip).

Please use this package by expanding it in an environment in which 
the packages related to SDK180.002 are expanded. 

----------------------------------------------------------------------
Contents of This Package
----------------------------------------------------------------------
cell/
|---Readme_cgc*.txt
|---host-linux
|    +---Cg
|         |---bin
|         |    +---sce-cgc
|         +---lib
|              +---libcgc.a
|---host-win32
|    +---Cg
|         |---bin
|         |    |---libcgc.dll
|         |    +---sce-cgc.exe
|         +---lib
|              |---libcgc.a
|              |---libcgc.lib
|              +---libcgc_static.lib
+---target
     +---ppu
          +---lib
               +---libcgc.a

----------------------------------------------------------------------
Chages
----------------------------------------------------------------------
<New Feature>
- For the shader output, an option -nofloatbindings that supports 
  a type other than float type has been added. 

- The texture format specified to the texture unit can be set to 
  the default setting. Set "default" to <texture unit number> that 
  is specified after the -texformat option.
    ex) sce-cgc -texformat default <format string>

<Bugfixs>
- When sce-cgc was performing optimization, the texture fetch instructions
  could be generated more than required. As a result, the performance 
  could be degraded. This problem has been fixed.

- When building a shader program with the -contalloc option specified, 
  a sampler* type array could not be assigned to TEXUNIT* repeatedly.
  This problem has been fixed.

- When h1texcompare2D() was used within the fragment shader, 
  an correct result could not be obtained. This problem has been fixed.

- The argument of the input parameter to the shader program could be set
  to "unreferenced" incorrectly. Because the parameter that is set to 
  "unreferenced" is deleted by sce-cgcstrip, the shader program that is 
  handled via sce-cgcstrip may not work correctly. 
  This problem has been fixed. 

- When creating a cgb file, a constant with an initial value that 
  had never been used in the vertex program could remain. 
  This problem has been fixed.

- Because OutColorPrec that is a profile option of sce-cgc was disabled,
  it was ignored even when the option was specified. This problem 
  has been fixed.

- Because a bug was found in the optimization logic for when compiling 
  a shader program, the sign of the float value could be inversed.
  Accordingly, an expected rendering processing could not be achieved.
  This problem has been fixed.

- When a compound assignment operator("*=", "+=" or the like) was used 
  within the shader program, a correct calculation result could not 
  be obtained.

    ex)DiffuseColor.xyz *= DiffuseColor.www;

  This problem has been fixed.

----------------------------------------------------------------------
Restrictions
----------------------------------------------------------------------
None

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

