/* Copyright 2001-2006  Sony Corporation */
/*
 * SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 * Copyright (C) 2009 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */
#ifndef _L10N_H
#define _L10N_H

#include	<inttypes.h>
#include	<sys/types.h>

#define L10N_MAJOR_VERSION	6
#define L10N_MINOR_VERSION	0
#define L10N_PATCH_VERSION	1
#define L10N_VERSION_MODIFIER	""

#ifndef SS2
#define SS2	0x8e
#endif
#ifndef SS3
#define SS3	0x8f
#endif

/*  UCS-4 range (hex)    UTF-8 octet sequence(binary)			*/
/*  0000 0000-0000 007f  0xxxxxxx					*/
/*  0000 0080-0000 07ff  110xxxxx 10xxxxxx				*/
/*  0000 0800-0000 ffff  1110xxxx 10xxxxxx 10xxxxxx			*/
/*  0001 0000-001f ffff  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx		*/
/*  0020 0000-03ff ffff  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx	*/
/*  0400 0000-7fff ffff  1111110x 10xxxxxx ... 10xxxxxx			*/
#define UTF8_MASK0	0xc0
#define UTF8_MASK1	0x80
#define UTF8_MASK2	0xe0
#define UTF8_MASK3	0xf0
#define UTF8_MASK4	0xf8
#define UTF8_MASK5	0xfc
#define UTF8_MASK6	0xfe

#define UTF8_OCTET0	0x80
#define UTF8_OCTET1	0x00
#define UTF8_OCTET2	0xc0
#define UTF8_OCTET3	0xe0
#define UTF8_OCTET4	0xf0
#define UTF8_OCTET5	0xf8
#define UTF8_OCTET6	0xfc

/*  UCS-4 range               UTF-16 (bin)			*/
/*  00000000xxxxxxxxxxxxxxxx  xxxxxxxxxxxxxxxx			*/
/*     (0000 0000-0000 ffff)					*/
/*  000uuuuuyyyyyyxxxxxxxxxx  110110wwwwyyyyyy 110111xxxxxxxxxx */
/*			      wwww = uuuuu - 1			*/
/*     (0001 0000-0010 ffff)       (d800-dbff)      (dc00-dfff)	*/
#define UTF16_SURROGATES_MASK1	0xf800
#define UTF16_SURROGATES_MASK2	0xfc00

#define UTF16_SURROGATES	0xd800
#define UTF16_HIGH_SURROGATES	0xd800
#define UTF16_LOW_SURROGATES	0xdc00

#define L10N_STR_UNKNOWN	(1 << 0)
#define L10N_STR_ASCII		(1 << 1)
#define L10N_STR_JIS		(1 << 2)
#define L10N_STR_EUCJP		(1 << 3)
#define L10N_STR_SJIS		(1 << 4)
#define L10N_STR_UTF8		(1 << 5)
#define L10N_STR_ILLEGAL	(1 << 16)
#define L10N_STR_ERROR		(1 << 17)

typedef enum {
    ConversionOK,
    SRCIllegal,
    DSTExhausted,
    ConverterUnknown
} L10nResult;

typedef enum {
    L10N_UTF8 = 0,
    L10N_UTF16,
    L10N_UTF32,
    L10N_UCS2,
    L10N_UCS4,
    L10N_ISO_8859_1,
    L10N_ISO_8859_2,
    L10N_ISO_8859_3,
    L10N_ISO_8859_4,
    L10N_ISO_8859_5,
    L10N_ISO_8859_6,
    L10N_ISO_8859_7,
    L10N_ISO_8859_8,
    L10N_ISO_8859_9,
    L10N_ISO_8859_10,
    L10N_ISO_8859_11,
    L10N_ISO_8859_13,
    L10N_ISO_8859_14,
    L10N_ISO_8859_15,
    L10N_ISO_8859_16,
    L10N_CODEPAGE_437,
    L10N_CODEPAGE_850,
    L10N_CODEPAGE_863,
    L10N_CODEPAGE_866,
    L10N_CODEPAGE_932,
    L10N_CODEPAGE_936,
    L10N_CODEPAGE_949,
    L10N_CODEPAGE_950,
    L10N_CODEPAGE_1251,
    L10N_CODEPAGE_1252,
    L10N_EUC_CN,
    L10N_EUC_JP,
    L10N_EUC_KR,
    L10N_ISO_2022_JP,
    L10N_ARIB,
    L10N_HZ,
    L10N_GB18030,
    L10N_RIS_506,
    L10N_CODEPAGE_852,   //3.10-
    L10N_CODEPAGE_1250,  //3.10-
    L10N_CODEPAGE_737,   //3.10-
    L10N_CODEPAGE_1253,  //3.10-
    L10N_CODEPAGE_857,   //3.10-
    L10N_CODEPAGE_1254,  //3.10-
    L10N_CODEPAGE_775,   //3.10-
    L10N_CODEPAGE_1257,  //3.10-
    L10N_CODEPAGE_855,   //3.10-
    L10N_CODEPAGE_858,   //3.10-
    L10N_CODEPAGE_860,   //3.10-
    L10N_CODEPAGE_861,   //3.10-
    L10N_CODEPAGE_865,   //3.10-
    L10N_CODEPAGE_869,   //3.10-
    _L10N_CODE_
} L10nCode;

#define L10N_SHIFT_JIS		L10N_CODEPAGE_932
#define L10N_UHC		L10N_CODEPAGE_949
#define L10N_GBK		L10N_CODEPAGE_936
#define L10N_BIG5		L10N_CODEPAGE_950
#define L10N_JIS		L10N_ISO_2022_JP
#define L10N_MUSIC_SHIFT_JIS	L10N_RIS_506

typedef int32_t	l10n_conv_t;

/*			jis2eucjp	jis2sjis	jis2kuten	*/
/*	eucjp2jis			eucjp2sjis	eucjp2kuten	*/
/*	sjis2jis	sjis2eucjp			sjis2kuten	*/
/*	kuten2jis	kuten2eucjp	kuten2sjis			*/
#define jis2eucjp(c)	((c) | 0x8080)
#define jis2kuten(c)	((c) - 0x2020)

#define eucjp2jis(c)	((c) & 0x7f7f)
#define eucjp2sjis(c)	(jis2sjis((c) & 0x7f7f))
#define eucjp2kuten(c)	((c) - 0xa0a0)

#define sjis2eucjp(c)	(jis2eucjp(sjis2jis(c)))
#define sjis2kuten(c)	(jis2kuten(sjis2jis(c)))

#define kuten2jis(c)	((c) + 0x2020)
#define kuten2eucjp(c)	((c) + 0xa0a0)
#define kuten2sjis(c)	(jis2sjis(kuten2jis(c)))

#define iskana2(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xdf)
#define iskpun(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xa5)
#define iskmoji(c)	((uint8_t)(c) >= 0xa6  &&  (uint8_t)(c) <= 0xdf)

#define isjjis(c)	((uint8_t)(c) >= 0x21  &&  (uint8_t)(c) <= 0x7e)

#define issjis1(c)	(((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0x9f)  || \
			 ((uint8_t)(c) >= 0xe0  &&  (uint8_t)(c) <= 0xfc))
#define issjis2(c)	((uint8_t)(c) >= 0x40  &&  (uint8_t)(c) <= 0xfc  && \
			 (uint8_t)(c) != 0x7f)
#define issjis1byte(c)	((uint8_t)(c) <= 0x80  || \
			 ((uint8_t)(c) >= 0xa0  &&  (uint8_t)(c) <= 0xdf)  || \
			 (uint8_t)(c) >= 0xfd)
#define issjis2byte(c)	(((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0x9f)  || \
			 ((uint8_t)(c) >= 0xe0  &&  (uint8_t)(c) <= 0xfc))

#define iseucjp(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe)
#define iseucjpkana(c)	((uint8_t)(c) == SS2)
#define iseucjp1byte(c)	((uint8_t)(c) <= 0x8d  || \
			 ((uint8_t)(c) >= 0x90  &&  (uint8_t)(c) <= 0xa0)  || \
			  (uint8_t)(c) == 0xff)
#define iseucjp2byte(c)	(((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe)  || \
			 (uint8_t)(c) == SS2)
#define iseucjp3byte(c)	((uint8_t)(c) == SS3)

#define iseuckr(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe)
#define iseuckr1byte(c)	((uint8_t)(c) <= 0x8d  || \
			 ((uint8_t)(c) >= 0x90  &&  (uint8_t)(c) <= 0xa0)  || \
			  (uint8_t)(c) == 0xff)

#define isuhc1(c)	((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe)
#define isuhc2(c)	(((uint8_t)(c) >= 0x41  &&  (uint8_t)(c) <= 0x5a)  || \
			 ((uint8_t)(c) >= 0x61  &&  (uint8_t)(c) <= 0x7a)  || \
			 ((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe))
#define isuhc1byte(c)	((uint8_t)(c) <= 0x80  ||  (uint8_t)(c) == 0xff)

#define iseuccn(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe)
#define iseuccn1byte(c)	((uint8_t)(c) <= 0x8d  || \
			 ((uint8_t)(c) >= 0x90  &&  (uint8_t)(c) <= 0xa0)  || \
			  (uint8_t)(c) == 0xff)

#define isgbk1(c)	((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe)
#define isgbk2(c)	((uint8_t)(c) >= 0x40  &&  (uint8_t)(c) <= 0xfe  && \
			 (uint8_t)(c) != 0x7f)
#define isgbk1byte(c)	((uint8_t)(c) <= 0x80  ||  (uint8_t)(c) == 0xff)

#define isgb18030_1byte(c) \
			((uint8_t)(c) <= 0x7f  ||  (uint8_t)(c) == 0xff)
#define isgb18030_2byte1(c) \
			((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe)
#define isgb18030_2byte2(c) \
			((uint8_t)(c) >= 0x40  &&  (uint8_t)(c) <= 0xfe  && \
			 (uint8_t)(c) != 0x7f)
#define isgb18030_4byte1(c) \
			((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe)
#define isgb18030_4byte2(c) \
			((uint8_t)(c) >= 0x30  &&  (uint8_t)(c) <= 0x39)
#define isgb18030_4byte3(c) \
			((uint8_t)(c) >= 0x81  &&  (uint8_t)(c) <= 0xfe)
#define isgb18030_4byte4(c) \
			((uint8_t)(c) >= 0x30  &&  (uint8_t)(c) <= 0x39)

#define isbig5_1(c)	((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe)
#define isbig5_2(c)	(((uint8_t)(c) >= 0x40  &&  (uint8_t)(c) <= 0x7e)  || \
			 ((uint8_t)(c) >= 0xa1  &&  (uint8_t)(c) <= 0xfe))
#define isbig5_1byte(c)	((uint8_t)(c) <= 0x80  ||  (uint8_t)(c) == 0xff)

#define isEucJpZen(c)	((((uint16_t)(c) >> 8) >= 0xa1  && \
			  ((uint16_t)(c) >> 8) <= 0xfe)  && \
			 (((uint16_t)(c) & 0xff) >= 0xa1  && \
			  ((uint16_t)(c) & 0xff) <= 0xfe))
#define isEucJpL0(c)	((uint16_t)(c) >= 0xa1a1  && \
			 (uint16_t)(c) < 0xb0a1  && \
			 (((uint16_t)(c) & 0xff) >= 0xa1  && \
			  ((uint16_t)(c) & 0xff) <= 0xfe))
#define isEucJpL1(c)	((uint16_t)(c) >= 0xb0a1  && \
			 (uint16_t)(c) <= 0xcfd3  && \
			 (((uint16_t)(c) & 0xff) >= 0xa1  && \
			  ((uint16_t)(c) & 0xff) <= 0xfe))
#define isEucJpL2(c)	((uint16_t)(c) >= 0xd0a1  && \
			 (uint16_t)(c) <= 0xf4a6  && \
			 (((uint16_t)(c) & 0xff) >= 0xa1  && \
			  ((uint16_t)(c) & 0xff) <= 0xfe))
#define isEucJpAlpha(c)	(isEucJpUpper(c)  ||  isEucJpLower(c))
#define isEucJpUpper(c)	((uint16_t)(c) >= 0xa3c1  && \
			 (uint16_t)(c) <= 0xa3da)
#define isEucJpLower(c)	((uint16_t)(c) >= 0xa3e1  && \
			 (uint16_t)(c) <= 0xa3fa)
#define isEucJpDigit(c)	((uint16_t)(c) >= 0xa3b0  && \
			 (uint16_t)(c) <= 0xa3b9)
#define isEucJpKata(c)	((uint16_t)(c) >= 0xa5a1  && \
			 (uint16_t)(c) <= 0xa5f6)
#define isEucJpHira(c)	((uint16_t)(c) >= 0xa4a1  && \
			 (uint16_t)(c) <= 0xa4f3)
#define isEucJpSpace(c)	((uint16_t)(c) == 0xa1a1)

#define isSjisZen(c)	(((((uint16_t)(c) >> 8) >= 0x81  && \
			   ((uint16_t)(c) >> 8) <= 0x9f)  || \
			  (((uint16_t)(c) >> 8) >= 0xe0  && \
			   ((uint16_t)(c) >> 8) <= 0xfc))  && \
			 (((uint16_t)(c) & 0xff) >= 0x40  && \
			  ((uint16_t)(c) & 0xff) <= 0xfc  && \
			  ((uint16_t)(c) & 0xff) != 0x7f))
#define isSjisL0(c)	((uint16_t)(c) >= 0x8140  && \
			 (uint16_t)(c) < 0x889f  && \
			 (((uint16_t)(c) & 0xff) >= 0x40  && \
			  ((uint16_t)(c) & 0xff) <= 0xfc  && \
			  ((uint16_t)(c) & 0xff) != 0x7f))
#define isSjisL1(c)	((uint16_t)(c) >= 0x889f  && \
			 (uint16_t)(c) <= 0x9872  && \
			 (((uint16_t)(c) & 0xff) >= 0x40  && \
			  ((uint16_t)(c) & 0xff) <= 0xfc  && \
			  ((uint16_t)(c) & 0xff) != 0x7f))
#define isSjisL2(c)	((uint16_t)(c) >= 0x989f  && \
			 (uint16_t)(c) <= 0xeaa4  && \
			 (((uint16_t)(c) & 0xff) >= 0x40  && \
			  ((uint16_t)(c) & 0xff) <= 0xfc  && \
			  ((uint16_t)(c) & 0xff) != 0x7f))
#define isSjisAlpha(c)	(isSjisUpper(c)  ||  isSjisLower(c))
#define isSjisUpper(c)	((uint16_t)(c) >= 0x8260  && \
			 (uint16_t)(c) <= 0x8279)
#define isSjisLower(c)	((uint16_t)(c) >= 0x8281  && \
			 (uint16_t)(c) <= 0x829a)
#define isSjisDigit(c)	((uint16_t)(c) >= 0x824f  && \
			 (uint16_t)(c) <= 0x8258)
#define isSjisKata(c)	((uint16_t)(c) >= 0x8340  && \
			 (uint16_t)(c) <= 0x8396  && \
			 (uint16_t)(c) != 0x837f)
#define isSjisHira(c)	((uint16_t)(c) >= 0x829f  && \
			 (uint16_t)(c) <= 0x82f1)
#define isSjisSpace(c)	((uint16_t)(c) == 0x8140)

#define utf8is0byte(c)	(((uint8_t)(c) & UTF8_MASK0) == UTF8_OCTET0)
#define utf8is1byte(c)	(((uint8_t)(c) & UTF8_MASK1) == UTF8_OCTET1)
#define utf8is2byte(c)	(((uint8_t)(c) & UTF8_MASK2) == UTF8_OCTET2  && \
			 (uint8_t)(c) >= 0xc2)
#define utf8is3byte(c)	(((uint8_t)(c) & UTF8_MASK3) == UTF8_OCTET3)
#define utf8is4byte(c)	(((uint8_t)(c) & UTF8_MASK4) == UTF8_OCTET4)
#define utf8is5byte(c)	(((uint8_t)(c) & UTF8_MASK5) == UTF8_OCTET5)
#define utf8is6byte(c)	(((uint8_t)(c) & UTF8_MASK6) == UTF8_OCTET6)

#define issurrogates(c)		\
    (((uint16_t)(c) & UTF16_SURROGATES_MASK1) == UTF16_SURROGATES)
#define ishighsurrogates(c)	\
    (((uint16_t)(c) & UTF16_SURROGATES_MASK2) == UTF16_HIGH_SURROGATES)
#define islowsurrogates(c)	\
    (((uint16_t)(c) & UTF16_SURROGATES_MASK2) == UTF16_LOW_SURROGATES)

#define toEucJpLower(c)	(((uint16_t)(c) >= 0xa3c1  && \
			  (uint16_t)(c) <= 0xa3da) \
			 ? ((uint16_t)(c) + 0x0020) : (c))
#define toEucJpUpper(c)	(((uint16_t)(c) >= 0xa3e1  && \
			  (uint16_t)(c) <= 0xa3fa) \
			 ? ((uint16_t)(c) - 0x0020) : (c))
#define toEucJpHira(c)	(((uint16_t)(c) >= 0xa5a1  && \
			  (uint16_t)(c) <= 0xa5f3) \
			 ? ((uint16_t)(c) - 0x0100) : (c))
#define toEucJpKata(c)	(((uint16_t)(c) >= 0xa4a1  && \
			  (uint16_t)(c) <= 0xa4f3) \
			 ? ((uint16_t)(c) + 0x0100) : (c))
#define toSjisLower(c)	(((uint16_t)(c) >= 0x8260  && \
			  (uint16_t)(c) <= 0x8279) \
			 ? ((uint16_t)(c) + 0x0021) : (c))
#define toSjisUpper(c)	(((uint16_t)(c) >= 0x8281  && \
			  (uint16_t)(c) <= 0x829a) \
			 ? ((uint16_t)(c) - 0x0021) : (c))
#define toSjisHira(c)	(((uint16_t)(c) >= 0x8340  && \
			  (uint16_t)(c) <= 0x837e) \
			 ? ((uint16_t)(c) - 0x00a1) : \
			 (((uint16_t)(c) >= 0x8380  && \
			   (uint16_t)(c) <= 0x8393) \
			 ? ((uint16_t)(c) - 0x00a2) : (c)))
#define toSjisKata(c)	(((uint16_t)(c) >= 0x829f  && \
			  (uint16_t)(c) <= 0x82dd) \
			 ? ((uint16_t)(c) + 0x00a1) : \
			 (((uint16_t)(c) >= 0x82de  && \
			   (uint16_t)(c) <= 0x82f1) \
			 ? ((uint16_t)(c) + 0x00a2) : (c)))

#ifndef __P
#if defined(__STDC__)  ||  defined(__cplusplus)  ||  defined(c_plusplus)
#define __P(protos)	protos
#else
#define __P(protos)	()
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int	UTF8toUTF16 __P(( const uint8_t [], uint16_t [], size_t * ));
int	UTF8toUTF32 __P(( const uint8_t [], uint32_t * ));
int	UTF8toUCS2 __P(( const uint8_t [], uint16_t * ));
int	UTF16toUTF8 __P(( const uint16_t [], uint8_t [], size_t * ));
int	UTF16toUTF32 __P(( const uint16_t [], uint32_t * ));
int	UTF16toUCS2 __P(( const uint16_t [], uint16_t * ));
int	UTF32toUTF8 __P(( const uint32_t, uint8_t [] ));
int	UTF32toUTF16 __P(( const uint32_t, uint16_t [] ));
int	UTF32toUCS2 __P(( const uint32_t, uint16_t * ));
int	UCS2toUTF8 __P(( const uint16_t, uint8_t [] ));
int	UCS2toUTF16 __P(( const uint16_t, uint16_t [] ));
int	UCS2toUTF32 __P(( const uint16_t, uint32_t * ));

int	SBCStoUTF8 __P(( const uint8_t, uint8_t [], L10nCode ));
int	SBCStoUCS2 __P(( const uint8_t, uint16_t *, L10nCode ));
int	UTF8toSBCS __P(( const uint8_t [], uint8_t *, L10nCode ));
int	UCS2toSBCS __P(( const uint16_t, uint8_t *, L10nCode ));

int	SJIStoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	SJIStoUCS2 __P(( const uint8_t [], uint16_t * ));
int	SJIStoEUCJP __P(( const uint8_t [], uint8_t [], size_t * ));
int	EUCJPtoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	EUCJPtoUCS2 __P(( const uint8_t [], uint16_t * ));
int	EUCJPtoSJIS __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toSJIS __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toEUCJP __P(( const uint8_t [], uint8_t [], size_t * ));
int	UCS2toSJIS __P(( const uint16_t, uint8_t [] ));
int	UCS2toEUCJP __P(( const uint16_t, uint8_t [] ));

int	MSJIStoUCS2 __P(( const uint16_t, uint16_t * ));
int	MSJIStoUTF8 __P(( const uint16_t, uint8_t [] ));
int	UCS2toMSJIS __P(( const uint16_t, uint16_t * ));
int	UTF8toMSJIS __P(( const uint8_t [], uint16_t * ));

int	EUCKRtoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	EUCKRtoUCS2 __P(( const uint8_t [], uint16_t * ));
int	EUCKRtoUHC __P(( const uint8_t [], uint8_t [], size_t * ));
int	UHCtoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	UHCtoUCS2 __P(( const uint8_t [], uint16_t * ));
int	UHCtoEUCKR __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toEUCKR __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toUHC __P(( const uint8_t [], uint8_t [], size_t * ));
int	UCS2toEUCKR __P(( const uint16_t, uint8_t [] ));
int	UCS2toUHC __P(( const uint16_t, uint8_t [] ));

int	EUCCNtoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	EUCCNtoUCS2 __P(( const uint8_t [], uint16_t * ));
int	GBKtoUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	GBKtoUCS2 __P(( const uint8_t [], uint16_t * ));
int	GB18030toUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	GB18030toUCS2 __P(( const uint8_t [], uint16_t * ));
int	UTF8toEUCCN __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toGBK __P(( const uint8_t [], uint8_t [], size_t * ));
int	UTF8toGB18030 __P(( const uint8_t [], uint8_t [], size_t * ));
int	UCS2toEUCCN __P(( const uint16_t, uint8_t [] ));
int	UCS2toGBK __P(( const uint16_t, uint8_t [] ));
int	UCS2toGB18030 __P(( const uint16_t, uint8_t [] ));

int	BIG5toUTF8 __P(( const uint8_t [], uint8_t [], size_t * ));
int	BIG5toUCS2 __P(( const uint8_t [], uint16_t * ));
int	UTF8toBIG5 __P(( const uint8_t [], uint8_t [], size_t * ));
int	UCS2toBIG5 __P(( const uint16_t, uint8_t [] ));

L10nResult	UTF8stoUTF16s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF8stoUTF32s __P(( const uint8_t [], size_t *, uint32_t [], size_t * ));
L10nResult	UTF8stoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF16stoUTF8s __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF16stoUTF32s __P(( const uint16_t [], size_t *, uint32_t [], size_t * ));
L10nResult	UTF16stoUCS2s __P(( const uint16_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF32stoUTF8s __P(( const uint32_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF32stoUTF16s __P(( const uint32_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF32stoUCS2s __P(( const uint32_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UCS2stoUTF8s __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoUTF16s __P(( const uint16_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UCS2stoUTF32s __P(( const uint16_t [], size_t *, uint32_t [], size_t * ));

L10nResult	SBCSstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t *, L10nCode ));
L10nResult	SBCSstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t *, L10nCode ));
L10nResult	UTF8stoSBCSs __P(( const uint8_t [], size_t *, uint8_t [], size_t *, L10nCode ));
L10nResult	UCS2stoSBCSs __P(( const uint16_t [], size_t *, uint8_t [], size_t *, L10nCode ));

L10nResult	SJISstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	SJISstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	SJISstoEUCJPs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	SJISstoJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EUCJPstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EUCJPstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	EUCJPstoSJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EUCJPstoJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	JISstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	JISstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	JISstoSJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	JISstoEUCJPs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoSJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoEUCJPs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoJISs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoSJISs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoEUCJPs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoJISs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));

L10nResult	ARIBstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ARIBstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF8stoARIBs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoARIBs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));

L10nResult	MSJISstoUTF8s __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	MSJISstoUCS2s __P(( const uint16_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF8stoMSJISs __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UCS2stoMSJISs __P(( const uint16_t [], size_t *, uint16_t [], size_t * ));

L10nResult	EUCKRstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EUCKRstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	EUCKRstoUHCs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UHCstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UHCstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UHCstoEUCKRs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoEUCKRs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoUHCs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoEUCKRs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoUHCs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));

L10nResult	EUCCNstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EUCCNstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	GBKstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	GBKstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	HZstoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	HZstoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	GB18030stoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	GB18030stoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF8stoEUCCNs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoGBKs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoHZs __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UTF8stoGB18030s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoEUCCNs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoGBKs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoHZs __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoGB18030s __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));

L10nResult	BIG5stoUTF8s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	BIG5stoUCS2s __P(( const uint8_t [], size_t *, uint16_t [], size_t * ));
L10nResult	UTF8stoBIG5s __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	UCS2stoBIG5s __P(( const uint16_t [], size_t *, uint8_t [], size_t * ));

l10n_conv_t	l10n_get_converter __P(( L10nCode, L10nCode ));
int		l10n_convert __P(( l10n_conv_t, const void *, void *, size_t * ));
L10nResult	l10n_convert_str __P(( l10n_conv_t, const void *, size_t *, void *, size_t * ));

int		L10nConvert __P(( L10nCode, const void *, L10nCode, void *, size_t * ));
L10nResult	L10nConvertStr __P(( L10nCode, const void *, size_t *, L10nCode, void *, size_t * ));

uint16_t	eucjp2sjis __P(( const uint16_t ));
uint16_t	eucjp2jis __P(( const uint16_t ));
uint16_t	eucjp2kuten __P(( const uint16_t ));
uint16_t	sjis2eucjp __P(( const uint16_t ));
uint16_t	sjis2jis __P(( const uint16_t ));
uint16_t	sjis2kuten __P(( const uint16_t ));
uint16_t	jis2eucjp __P(( const uint16_t ));
uint16_t	jis2sjis __P(( const uint16_t ));
uint16_t	jis2kuten __P(( const uint16_t ));
uint16_t	kuten2eucjp __P(( const uint16_t ));
uint16_t	kuten2sjis __P(( const uint16_t ));
uint16_t	kuten2jis __P(( const uint16_t ));

uint16_t	toEucJpLower __P(( const uint16_t ));
uint16_t	toEucJpUpper __P(( const uint16_t ));
uint16_t	toEucJpHira __P(( const uint16_t ));
uint16_t	toEucJpKata __P(( const uint16_t ));

uint16_t	toSjisLower __P(( const uint16_t ));
uint16_t	toSjisUpper __P(( const uint16_t ));
uint16_t	toSjisHira __P(( const uint16_t ));
uint16_t	toSjisKata __P(( const uint16_t ));

uint16_t	eucjpzen2han __P(( const uint16_t ));
uint16_t	eucjphan2zen __P(( const uint16_t ));

uint16_t	sjiszen2han __P(( const uint16_t ));
uint16_t	sjishan2zen __P(( const uint16_t ));

L10nResult	ToEucJpLower __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToEucJpUpper __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToEucJpHira __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToEucJpKata __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));

L10nResult	ToSjisLower __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToSjisUpper __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToSjisHira __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	ToSjisKata __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));

L10nResult	EucJpZen2Han __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	EucJpHan2Zen __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));

L10nResult	SjisZen2Han __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));
L10nResult	SjisHan2Zen __P(( const uint8_t [], size_t *, uint8_t [], size_t * ));

int	utf8is0byte __P(( const uint8_t ));
int	utf8is1byte __P(( const uint8_t ));
int	utf8is2byte __P(( const uint8_t ));
int	utf8is3byte __P(( const uint8_t ));
int	utf8is4byte __P(( const uint8_t ));
int	utf8is5byte __P(( const uint8_t ));
int	utf8is6byte __P(( const uint8_t ));

int	issurrogates __P(( const uint16_t ));
int	ishighsurrogates __P(( const uint16_t ));
int	islowsurrogates __P(( const uint16_t ));

int	iskana2 __P(( const uint8_t ));
int	iskpun __P(( const uint8_t ));
int	iskmoji __P(( const uint8_t ));

int	isjjis __P(( const uint8_t ));

int	issjis1 __P(( const uint8_t ));
int	issjis2 __P(( const uint8_t ));
int	issjis1byte __P(( const uint8_t ));
int	issjis2byte __P(( const uint8_t ));

int	iseucjp __P(( const uint8_t ));
int	iseucjpkana __P(( const uint8_t ));
int	iseucjp1byte __P(( const uint8_t ));
int	iseucjp2byte __P(( const uint8_t ));
int	iseucjp3byte __P(( const uint8_t ));

int	iseuckr __P(( const uint8_t ));
int	iseuckr1byte __P(( const uint8_t ));

int	isuhc1 __P(( const uint8_t ));
int	isuhc2 __P(( const uint8_t ));
int	isuhc1byte __P(( const uint8_t ));

int	iseuccn __P(( const uint8_t ));
int	iseuccn1byte __P(( const uint8_t ));

int	isgbk1 __P(( const uint8_t ));
int	isgbk2 __P(( const uint8_t ));
int	isgbk1byte __P(( const uint8_t ));

int	isgb18030_1byte __P(( const uint8_t ));
int	isgb18030_2byte1 __P(( const uint8_t ));
int	isgb18030_2byte2 __P(( const uint8_t ));
int	isgb18030_4byte1 __P(( const uint8_t ));
int	isgb18030_4byte2 __P(( const uint8_t ));
int	isgb18030_4byte3 __P(( const uint8_t ));
int	isgb18030_4byte4 __P(( const uint8_t ));

int	isbig5_1 __P(( const uint8_t ));
int	isbig5_2 __P(( const uint8_t ));
int	isbig5_1byte __P(( const uint8_t ));

int	isEucJpZen __P(( const uint16_t ));
int	isEucJpL0 __P(( const uint16_t ));
int	isEucJpL1 __P(( const uint16_t ));
int	isEucJpL2 __P(( const uint16_t ));
int	isEucJpAlpha __P(( const uint16_t ));
int	isEucJpUpper __P(( const uint16_t ));
int	isEucJpLower __P(( const uint16_t ));
int	isEucJpDigit __P(( const uint16_t ));
int	isEucJpKata __P(( const uint16_t ));
int	isEucJpHira __P(( const uint16_t ));
int	isEucJpKigou __P(( const uint16_t ));
int	isEucJpSpace __P(( const uint16_t ));

int	isSjisZen __P(( const uint16_t ));
int	isSjisL0 __P(( const uint16_t ));
int	isSjisL1 __P(( const uint16_t ));
int	isSjisL2 __P(( const uint16_t ));
int	isSjisAlpha __P(( const uint16_t ));
int	isSjisUpper __P(( const uint16_t ));
int	isSjisLower __P(( const uint16_t ));
int	isSjisDigit __P(( const uint16_t ));
int	isSjisKata __P(( const uint16_t ));
int	isSjisHira __P(( const uint16_t ));
int	isSjisKigou __P(( const uint16_t ));
int	isSjisSpace __P(( const uint16_t ));

int	jstrchk __P(( const uint8_t [] ));
int	jstrnchk __P(( const uint8_t [], size_t ));

#ifdef __cplusplus
}
#endif

#endif /* _L10N_H */
