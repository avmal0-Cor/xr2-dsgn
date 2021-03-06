/*
Copyright 2001-2004 by NVIDIA Corporation.  All rights reserved.

All information contained herein is proprietary and confidential to NVIDIA 
Corporation. Any use, reproduction, or disclosure without the advance written
permission of NVIDIA Corporation is prohibited.  

Without limiting any of the foregoing, all information contained herein shall
be deemed NVIDIA "Confidential Information" as defined in the Confidentiality
and Non-Disclosure Agreement among Sony Computer Entertainment Inc. ("SCEI"),
Toshiba Corporation ("Toshiba"), NVIDIA Corporation, and NVIDIA K.K.
(collectively, "NVIDIA"), entered into as of March 5, 2004 ("CNDA").  NVIDIA 
Confidential Information is hereby disclosed and transmitted solely and 
exclusively to SCEI pursuant to the terms and conditions set forth in the CNDA
and pursuant to the Letter of Intent executed between NVIDIA Corporation and
SCEI on or about on December 7, 2004 ("LOI").  Provided, however,
notwithstanding any language to the contrary in the CNDA,  SCEI acknowledges
and agrees that NVIDIA Confidential Information contained herein shall not be
disclosed to Toshiba without first obtaining NVIDIA's advance written approval.
*/




/*
 *
 * Copyright (c) 2002-2007, NVIDIA Corporation.
 * 
 *  
 * 
 * NVIDIA Corporation("NVIDIA") supplies this software to you in consideration 
 * of your agreement to the following terms, and your use, installation, 
 * modification or redistribution of this NVIDIA software constitutes 
 * acceptance of these terms.  If you do not agree with these terms, please do 
 * not use, install, modify or redistribute this NVIDIA software.
 * 
 *  
 * 
 * In consideration of your agreement to abide by the following terms, and 
 * subject to these terms, NVIDIA grants you a personal, non-exclusive license,
 * under NVIDIA's copyrights in this original NVIDIA software (the "NVIDIA 
 * Software"), to use, reproduce, modify and redistribute the NVIDIA 
 * Software, with or without modifications, in source and/or binary forms; 
 * provided that if you redistribute the NVIDIA Software, you must retain the 
 * copyright notice of NVIDIA, this notice and the following text and 
 * disclaimers in all such redistributions of the NVIDIA Software. Neither the 
 * name, trademarks, service marks nor logos of NVIDIA Corporation may be used 
 * to endorse or promote products derived from the NVIDIA Software without 
 * specific prior written permission from NVIDIA.  Except as expressly stated 
 * in this notice, no other rights or licenses express or implied, are granted 
 * by NVIDIA herein, including but not limited to any patent rights that may be 
 * infringed by your derivative works or by other works in which the NVIDIA 
 * Software may be incorporated. No hardware is licensed hereunder. 
 * 
 *  
 * 
 * THE NVIDIA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT 
 * WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING 
 * WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, 
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR ITS USE AND OPERATION 
 * EITHER ALONE OR IN COMBINATION WITH OTHER PRODUCTS.
 * 
 *  
 * 
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, 
 * EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, LOST 
 * PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY OUT OF THE USE, 
 * REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE NVIDIA SOFTWARE, 
 * HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING 
 * NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF NVIDIA HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */ 


/* 
 * The following macro invocations define the supported CG basic data types.
 *
 * The macros have the form :
 *
 *   CG_ENUM_MACRO(enum_name, enum_val)
 *
 *     enum_name     : The C enumerant name.
 *     enum_val      : The enumerant value.
 *   
 */



CG_ENUM_MACRO(CG_UNKNOWN, 4096)
CG_ENUM_MACRO(CG_IN, 4097)
CG_ENUM_MACRO(CG_OUT, 4098)
CG_ENUM_MACRO(CG_INOUT, 4099)
CG_ENUM_MACRO(CG_MIXED, 4100)
CG_ENUM_MACRO(CG_VARYING, 4101)
CG_ENUM_MACRO(CG_UNIFORM, 4102)
CG_ENUM_MACRO(CG_CONSTANT, 4103)
CG_ENUM_MACRO(CG_PROGRAM_SOURCE, 4104)
CG_ENUM_MACRO(CG_PROGRAM_ENTRY, 4105)
CG_ENUM_MACRO(CG_COMPILED_PROGRAM, 4106)
CG_ENUM_MACRO(CG_PROGRAM_PROFILE, 4107)
CG_ENUM_MACRO(CG_GLOBAL, 4108)
CG_ENUM_MACRO(CG_PROGRAM, 4109)
CG_ENUM_MACRO(CG_DEFAULT, 4110)
CG_ENUM_MACRO(CG_ERROR, 4111)
CG_ENUM_MACRO(CG_SOURCE, 4112)
CG_ENUM_MACRO(CG_OBJECT, 4113)
CG_ENUM_MACRO(CG_COMPILE_MANUAL, 4114)
CG_ENUM_MACRO(CG_COMPILE_IMMEDIATE, 4115)
CG_ENUM_MACRO(CG_COMPILE_LAZY, 4116)
CG_ENUM_MACRO(CG_CURRENT, 4117)
CG_ENUM_MACRO(CG_LITERAL, 4118)
CG_ENUM_MACRO(CG_VERSION, 4119)
CG_ENUM_MACRO(CG_ROW_MAJOR, 4120)
CG_ENUM_MACRO(CG_COLUMN_MAJOR, 4121)
CG_ENUM_MACRO(CG_FRAGMENT, 4122)
CG_ENUM_MACRO(CG_VERTEX, 4123)
CG_ENUM_MACRO(CG_POINT, 4124)
CG_ENUM_MACRO(CG_LINE, 4125)
CG_ENUM_MACRO(CG_LINE_ADJ, 4126)
CG_ENUM_MACRO(CG_TRIANGLE, 4127)
CG_ENUM_MACRO(CG_TRIANGLE_ADJ, 4128)
CG_ENUM_MACRO(CG_POINT_OUT, 4129)
CG_ENUM_MACRO(CG_LINE_OUT, 4130)
CG_ENUM_MACRO(CG_TRIANGLE_OUT, 4131)
CG_ENUM_MACRO(CG_IMMEDIATE_PARAMETER_SETTING, 4132)
CG_ENUM_MACRO(CG_DEFERRED_PARAMETER_SETTING, 4133)
CG_ENUM_MACRO(CG_NO_LOCKS_POLICY, 4134)
CG_ENUM_MACRO(CG_THREAD_SAFE_POLICY, 4135)
CG_ENUM_MACRO(CG_FORCE_UPPER_CASE_POLICY, 4136)
CG_ENUM_MACRO(CG_UNCHANGED_CASE_POLICY, 4137)
#undef CG_ENUM_MACRO


