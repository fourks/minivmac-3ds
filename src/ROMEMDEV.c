/*
	ROMEMDEV.c

	Copyright (C) 2007 Philip Cummins, Paul C. Pratt

	You can redistribute this file and/or modify it under the terms
	of version 2 of the GNU General Public License as published by
	the Free Software Foundation.  You should have received a copy
	of the license along with this file; see the file COPYING.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	license for more details.
*/

/*
	Read Only Memory EMulated DEVice

	Checks the header of the loaded ROM image, and then patches
	the ROM image.

	This code descended from "ROM.c" in vMac by Philip Cummins.

	Support for "Twiggy" Mac by Mathew Hybler.
*/

#ifndef AllFiles
#include "SYSDEPNS.h"
#include "MYOSGLUE.h"
#include "ENDIANAC.h"
#include "EMCONFIG.h"
#include "GLOBGLUE.h"
#endif

#include "ROMEMDEV.h"

#define UseSonyPatch \
	((CurEmMd <= kEmMd_Classic) || (CurEmMd == kEmMd_II) \
		|| (CurEmMd == kEmMd_IIx))

#define UseLargeScreenHack \
	(IncludeVidMem \
	&& (CurEmMd != kEmMd_PB100) \
	&& (CurEmMd != kEmMd_II) \
	&& (CurEmMd != kEmMd_IIx))

#if UseSonyPatch
LOCALVAR const ui3b sony_driver[] = {
/*
	Replacement for .Sony driver
	68k machine code, compiled from mydriver.a
*/
0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xEE, 0x00, 0x18, 0x00, 0x24, 0x00, 0x4A,
0x00, 0x8A, 0x05, 0x2E, 0x53, 0x6F, 0x6E, 0x79,
0x48, 0xE7, 0x00, 0xC0, 0x55, 0x4F, 0x3F, 0x3C,
0x00, 0x01, 0x60, 0x30, 0x48, 0xE7, 0x00, 0xC0,
0x55, 0x4F, 0x3F, 0x3C, 0x00, 0x02, 0x41, 0xFA,
0x01, 0x84, 0x2F, 0x18, 0x20, 0x50, 0x20, 0x8F,
0x5C, 0x4F, 0x30, 0x1F, 0x4C, 0xDF, 0x03, 0x00,
0x0C, 0x68, 0x00, 0x01, 0x00, 0x1A, 0x66, 0x1E,
0x4E, 0x75, 0x48, 0xE7, 0x00, 0xC0, 0x55, 0x4F,
0x3F, 0x3C, 0x00, 0x03, 0x41, 0xFA, 0x01, 0x5E,
0x2F, 0x18, 0x20, 0x50, 0x20, 0x8F, 0x5C, 0x4F,
0x30, 0x1F, 0x4C, 0xDF, 0x03, 0x00, 0x32, 0x28,
0x00, 0x06, 0x08, 0x01, 0x00, 0x09, 0x67, 0x0C,
0x4A, 0x40, 0x6F, 0x02, 0x42, 0x40, 0x31, 0x40,
0x00, 0x10, 0x4E, 0x75, 0x4A, 0x40, 0x6F, 0x04,
0x42, 0x40, 0x4E, 0x75, 0x2F, 0x38, 0x08, 0xFC,
0x4E, 0x75, 0x48, 0xE7, 0x00, 0xC0, 0x55, 0x4F,
0x3F, 0x3C, 0x00, 0x04, 0x41, 0xFA, 0x01, 0x1E,
0x2F, 0x18, 0x20, 0x50, 0x20, 0x8F, 0x5C, 0x4F,
0x30, 0x1F, 0x4C, 0xDF, 0x03, 0x00, 0x4E, 0x75,
0x48, 0xE7, 0xE0, 0xC0, 0x20, 0x2F, 0x00, 0x14,
0x59, 0x4F, 0x2F, 0x00, 0x55, 0x4F, 0x3F, 0x3C,
0x00, 0x08, 0x41, 0xFA, 0x00, 0xF8, 0x2F, 0x18,
0x20, 0x50, 0x20, 0x8F, 0x5C, 0x4F, 0x32, 0x1F,
0x58, 0x4F, 0x20, 0x1F, 0x4A, 0x41, 0x66, 0x06,
0x30, 0x7C, 0x00, 0x07, 0xA0, 0x2F, 0x4C, 0xDF,
0x03, 0x07, 0x58, 0x4F, 0x4E, 0x73, 0x21, 0x40,
0x00, 0x06, 0x43, 0xF8, 0x03, 0x08, 0x4E, 0xF9,
0x00, 0x40, 0x0B, 0x20, 0x4E, 0x75, 0x48, 0xE7,
0x1F, 0x10, 0x48, 0xE7, 0x00, 0xC0, 0x5D, 0x4F,
0x3F, 0x3C, 0x00, 0x05, 0x41, 0xFA, 0x00, 0xB6,
0x2F, 0x18, 0x20, 0x50, 0x20, 0x8F, 0x5C, 0x4F,
0x30, 0x1F, 0x2E, 0x1F, 0x0C, 0x40, 0xFF, 0xCF,
0x66, 0x06, 0x42, 0x40, 0x60, 0x00, 0x00, 0x8E,
0x4A, 0x40, 0x66, 0x00, 0x00, 0x88, 0x20, 0x07,
0xA7, 0x1E, 0x26, 0x48, 0x20, 0x0B, 0x67, 0x00,
0x00, 0x86, 0x9E, 0xFC, 0x00, 0x10, 0x2F, 0x0B,
0x2F, 0x07, 0x55, 0x4F, 0x3F, 0x3C, 0x00, 0x06,
0x41, 0xFA, 0x00, 0x7A, 0x2F, 0x18, 0x20, 0x50,
0x20, 0x8F, 0x5C, 0x4F, 0x30, 0x1F, 0x50, 0x4F,
0x2E, 0x1F, 0x76, 0x00, 0x36, 0x1F, 0x38, 0x1F,
0x3C, 0x1F, 0x3A, 0x1F, 0x26, 0x5F, 0x4A, 0x40,
0x66, 0x4A, 0x20, 0x0B, 0x67, 0x0E, 0x41, 0xFA,
0xFF, 0x8C, 0x27, 0x48, 0x00, 0x06, 0x20, 0x4B,
0xA0, 0x58, 0x60, 0x1A, 0x41, 0xFA, 0xFF, 0x70,
0x30, 0x3C, 0xA0, 0x4E, 0xA0, 0x47, 0x60, 0x0E,
0x20, 0x47, 0x30, 0x06, 0x48, 0x40, 0x30, 0x05,
0xA0, 0x4E, 0xDE, 0x83, 0x52, 0x46, 0x51, 0xCC,
0xFF, 0xF0, 0x48, 0x7A, 0xFF, 0x1C, 0x55, 0x4F,
0x3F, 0x3C, 0x00, 0x07, 0x41, 0xFA, 0x00, 0x1E,
0x2F, 0x18, 0x20, 0x50, 0x20, 0x8F, 0x5C, 0x4F,
0x30, 0x1F, 0x58, 0x4F, 0x4C, 0xDF, 0x03, 0x00,
0x4C, 0xDF, 0x08, 0xF8, 0x4E, 0x75, 0x30, 0x3C,
0xFF, 0xFF, 0x60, 0xF0
};
#endif

#if UseSonyPatch
LOCALVAR const ui3b my_disk_icon[] = {
	0x7F, 0xFF, 0xFF, 0xF0,
	0x81, 0x00, 0x01, 0x08,
	0x81, 0x00, 0x71, 0x04,
	0x81, 0x00, 0x89, 0x02,
	0x81, 0x00, 0x89, 0x01,
	0x81, 0x00, 0x89, 0x01,
	0x81, 0x00, 0x89, 0x01,
	0x81, 0x00, 0x89, 0x01,
	0x81, 0x00, 0x89, 0x01,
	0x81, 0x00, 0x71, 0x01,
	0x81, 0x00, 0x01, 0x01,
	0x80, 0xFF, 0xFE, 0x01,
	0x80, 0x00, 0x00, 0x01,
	0x80, 0x00, 0x00, 0x01,
	0x80, 0x00, 0x00, 0x01,
	0x80, 0x00, 0x00, 0x01,
	0x83, 0xFF, 0xFF, 0xC1,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x06, 0x30, 0x21,
	0x84, 0x06, 0x60, 0x21,
	0x84, 0x06, 0xC0, 0x21,
	0x84, 0x07, 0x80, 0x21,
	0x84, 0x07, 0x00, 0x21,
	0x84, 0x06, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x84, 0x00, 0x00, 0x21,
	0x7F, 0xFF, 0xFF, 0xFE,

	/* mask */

	0x3F, 0xFF, 0xFF, 0xF0,
	0x7F, 0xFF, 0xFF, 0xF0,
	0xFF, 0xFF, 0xFF, 0xFC,
	0xFF, 0xFF, 0xFF, 0xFC,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x7F, 0xFF, 0xFF, 0xFC,
	0x3F, 0xFF, 0xFF, 0xFC,

	/* empty pascal string */
	0x00, 0x00,
};
#endif

#if CurEmMd <= kEmMd_Twig43
#define Sony_DriverBase 0x1836
#elif CurEmMd <= kEmMd_Twiggy
#define Sony_DriverBase 0x16E4
#elif CurEmMd <= kEmMd_128K
#define Sony_DriverBase 0x1690
#elif CurEmMd <= kEmMd_Plus
#define Sony_DriverBase 0x17D30
#elif CurEmMd <= kEmMd_Classic
#define Sony_DriverBase 0x34680
#elif (CurEmMd == kEmMd_II) || (CurEmMd == kEmMd_IIx)
#define Sony_DriverBase 0x2D72C
#endif

#define kVidMem_Base 0x00540000

#if UseSonyPatch
LOCALPROC Sony_Install(void)
{
	ui3p pto = Sony_DriverBase + ROM;

	MyMoveBytes((anyp)sony_driver, (anyp)pto, sizeof(sony_driver));
#if CurEmMd <= kEmMd_Twiggy
	do_put_mem_long(pto + 0x14, 0x4469736B);
		/* 'Disk' instead of 'Sony' */
#if CurEmMd <= kEmMd_Twig43
	do_put_mem_word(pto + 0xEA, 0x0C8A);
#else
	do_put_mem_word(pto + 0xEA, 0x0B74);
#endif
#endif

	pto += sizeof(sony_driver);

	do_put_mem_word(pto, kcom_callcheck);
	pto += 2;
	do_put_mem_word(pto, kExtnSony);
	pto += 2;
	do_put_mem_long(pto, kExtn_Block_Base); /* pokeaddr */
	pto += 4;

	my_disk_icon_addr = (pto - ROM) + kROM_Base;
	MyMoveBytes((anyp)my_disk_icon, (anyp)pto, sizeof(my_disk_icon));
	pto += sizeof(my_disk_icon);

#if UseLargeScreenHack
	{
		ui3p patchp = pto;

#include "SCRNHACK.h"
	}
#endif

	(void) pto; /* avoid warning about unused */
}
#endif

#ifndef CheckRomCheckSum
#define CheckRomCheckSum 1
#endif

#ifndef DisableRomCheck
#define DisableRomCheck 1
#endif

#ifndef DisableRamTest
#define DisableRamTest 1
#endif

#if CheckRomCheckSum
LOCALFUNC ui5r Calc_Checksum(void)
{
	long int i;
	ui5b CheckSum = 0;
	ui3p p = 4 + ROM;

	for (i = (kCheckSumRom_Size - 4) >> 1; --i >= 0; ) {
		CheckSum += do_get_mem_word(p);
		p += 2;
	}

	return CheckSum;
}
#endif

#ifdef CurAltHappyMac
#include "HPMCHACK.h"
#endif

#ifdef ln2mtb
LOCALPROC ROMscrambleForMTB(void)
{
	si5r j;
	ui3p p = ROM;
	ui3p p2 = ROM + (1 << ln2mtb);

	for (j = kROM_Size / (1 << ln2mtb) / 2; --j >= 0; ) {
		si5r i;

		for (i = (1 << ln2mtb); --i >= 0; ) {
			ui3b t0 = *p;
			ui3b t1 = *p2;
			*p++ = t1;
			*p2++ = t0;
		}

		p += (1 << ln2mtb);
		p2 += (1 << ln2mtb);
	}
}
#endif

GLOBALFUNC blnr ROM_Init(void)
{
#if CheckRomCheckSum
	ui5r CheckSum = Calc_Checksum();

#if CurEmMd >= kEmMd_Twiggy
	if (CheckSum != do_get_mem_long(ROM)) {
		WarnMsgCorruptedROM();
	} else
#endif
#if CurEmMd <= kEmMd_Twig43
	if (CheckSum == 0x27F4E04B) {
	} else
#elif CurEmMd <= kEmMd_Twiggy
	if (CheckSum == 0x2884371D) {
	} else
#elif CurEmMd <= kEmMd_128K
	if (CheckSum == 0x28BA61CE) {
	} else
	if (CheckSum == 0x28BA4E50) {
	} else
#elif CurEmMd <= kEmMd_Plus
	if (CheckSum == 0x4D1EEEE1) {
		/* Mac Plus ROM v 1, 'Lonely Hearts' */
	} else
	if (CheckSum == 0x4D1EEAE1) {
		/* Mac Plus ROM v 2, 'Lonely Heifers' */
	} else
	if (CheckSum == 0x4D1F8172) {
		/* Mac Plus ROM v 3, 'Loud Harmonicas' */
	} else
#elif CurEmMd <= kEmMd_SE
	if (CheckSum == 0xB2E362A8) {
	} else
#elif CurEmMd <= kEmMd_SEFDHD
	if (CheckSum == 0xB306E171) {
	} else
#elif CurEmMd <= kEmMd_Classic
	if (CheckSum == 0xA49F9914) {
	} else
#elif CurEmMd <= kEmMd_PB100
	if (CheckSum == 0x96645F9C) {
	} else
#elif CurEmMd <= kEmMd_II
	if (CheckSum == 0x9779D2C4) {
	} else
	if (CheckSum == 0x97221136) {
		/* accept IIx ROM */
	} else
#elif CurEmMd <= kEmMd_IIx
	if (CheckSum == 0x97221136) {
	} else
#endif
	{
		WarnMsgUnsupportedROM();
	}
	/*
		Even if ROM is corrupt or unsupported, go ahead and
		try to run anyway. It shouldn't do any harm.
	*/

#endif /* CheckRomCheckSum */


#if DisableRomCheck

/* skip the rom checksum */
#if CurEmMd <= kEmMd_Twig43
	/* no checksum code */
#elif CurEmMd <= kEmMd_Twiggy
	do_put_mem_word(0x136 + ROM, 0x6004);
#elif CurEmMd <= kEmMd_128K
	do_put_mem_word(0xE2 + ROM, 0x6004);
#elif CurEmMd <= kEmMd_Plus
	do_put_mem_word(0xD7A + ROM, 0x6022);
#elif CurEmMd <= kEmMd_Classic
	do_put_mem_word(0x1C68 + ROM, 0x6008);
#elif (CurEmMd == kEmMd_II) || (CurEmMd == kEmMd_IIx)
	do_put_mem_word(0x2AB0 + ROM, 0x6008);
#endif

#endif /* DisableRomCheck */


#if DisableRamTest

#if CurEmMd <= kEmMd_128K
#elif CurEmMd <= kEmMd_Plus
	do_put_mem_word(3752 + ROM, 0x4E71);
		/* shorten the ram check read */
	do_put_mem_word(3728 + ROM, 0x4E71);
		/* shorten the ram check write */
#elif CurEmMd <= kEmMd_Classic
	do_put_mem_word(134 + ROM, 0x6002);
	do_put_mem_word(286 + ROM, 0x6002);
#elif (CurEmMd == kEmMd_II) || (CurEmMd == kEmMd_IIx)
	do_put_mem_word(0xEE + ROM, 0x6002);
	do_put_mem_word(0x1AA + ROM, 0x6002);
#endif

#endif /* DisableRamTest */

#ifdef CurAltHappyMac
	PatchHappyMac();
#endif

	/* do_put_mem_word(862 + ROM, 0x4E71); */ /* shorten set memory */

#if UseSonyPatch
	Sony_Install();
#endif

#ifdef ln2mtb
	ROMscrambleForMTB();
#endif

	return trueblnr;
}
