// SKCAccuratePalette.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SKCModLoader.h"

struct Color
{
	char B;
	char G;
	char R;
	char X;
};

#pragma warning(suppress : 4838 4309)
const char MDColorTable[] = { 0, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };

DataPointer(char, b_water_full_screen_flag, 0x8FFF64E);
DataArray(short, water_palette, 0x8FFF080, 64);
DataPointer(int, TransparentColorIndex, 0x8549D8);
FunctionPointer(int, sub_404350, (), 0x404350);
DataPointer(int, dword_854DE8, 0x854DE8);
ThiscallFunctionPointer(void, CopyPalette2, (int _this, const void *Src), 0x4069D0);
ThiscallFunctionPointer(void, CopyPalette, (int _this, const void *Src), 0x40699E);
DataPointer(char, b_hyper_sonic_flash_timer, 0x8FFF666);
DataPointer(short, w_H_int_counter_command, 0x8FFF624);

void __cdecl ConvertPalette_r()
{
	Color rgbpal[64]; // [sp+Ch] [bp-110h]@1
	int i; // [sp+10Ch] [bp-10h]@4
	__int16 *mdpal; // [sp+110h] [bp-Ch]@2
	Color bgcol; // [sp+114h] [bp-8h]@7
	int palette_dest; // [sp+118h] [bp-4h]@7

	memset(rgbpal, 0, 0x100u);
	if (b_water_full_screen_flag)
		mdpal = water_palette;
	else
		mdpal = Normal_palette;
	for (i = 0; i < 64; ++i)
	{
		rgbpal[i].R = MDColorTable[(mdpal[i] & 0xF) >> 1];
		rgbpal[i].G = MDColorTable[((mdpal[i] >> 4) & 0xF) >> 1];
		rgbpal[i].B = MDColorTable[((mdpal[i] >> 8) & 0xF) >> 1];
	}
	bgcol = rgbpal[TransparentColorIndex];
	rgbpal[0] = bgcol;
	rgbpal[16] = bgcol;
	rgbpal[32] = bgcol;
	rgbpal[48] = bgcol;
	palette_dest = sub_404350();
	if (dword_854DE8 == 5)
	{
		CopyPalette2(palette_dest, rgbpal);
		for (i = 0; i < 64; ++i)
		{
			rgbpal[i].R >>= 1;
			rgbpal[i].G >>= 1;
			rgbpal[i].B >>= 1;
		}
		CopyPalette(palette_dest, rgbpal);
	}
	else
	{
		CopyPalette(palette_dest, rgbpal);
		CopyPalette2(palette_dest, rgbpal);
	}
}

void __cdecl ConvertPalette2_r()
{
	signed int flash_flag; // [sp+Ch] [bp-114h]@2
	Color rgbpal[64]; // [sp+10h] [bp-110h]@4
	int i; // [sp+110h] [bp-10h]@7
	__int16 *mdpal; // [sp+114h] [bp-Ch]@6
	Color bgcol; // [sp+118h] [bp-8h]@17
	int palette_dest; // [sp+11Ch] [bp-4h]@17

	if ((signed int)(unsigned __int8)b_hyper_sonic_flash_timer <= 0)
	{
		flash_flag = 0;
	}
	else
	{
		--b_hyper_sonic_flash_timer;
		flash_flag = 1;
	}
	memset(rgbpal, 0, 0x100u);
	if (b_water_full_screen_flag)
		*(char*)&w_H_int_counter_command = 0;
	mdpal = Normal_palette;
	if (flash_flag)
	{
		for (i = 0; i < 32; ++i)
		{
			rgbpal[i].R = 0xFFu;
			rgbpal[i].G = 0xFFu;
			rgbpal[i].B = 0xFFu;
		}
		for (i = 33; i < 64; ++i)
		{
			rgbpal[i].R = 0xFFu;
			rgbpal[i].G = 0xFFu;
			rgbpal[i].B = 0xFFu;
		}
	}
	else
	{
		for (i = 0; i < 64; ++i)
		{
			rgbpal[i].R = MDColorTable[(mdpal[i] & 0xF) >> 1];
			rgbpal[i].G = MDColorTable[((mdpal[i] >> 4) & 0xF) >> 1];
			rgbpal[i].B = MDColorTable[((mdpal[i] >> 8) & 0xF) >> 1];
		}
	}
	bgcol = rgbpal[TransparentColorIndex];
	rgbpal[0] = bgcol;
	rgbpal[16] = bgcol;
	rgbpal[32] = bgcol;
	rgbpal[48] = bgcol;
	palette_dest = sub_404350();
	CopyPalette(palette_dest, rgbpal);
	mdpal = water_palette;
	if (flash_flag)
	{
		for (i = 0; i < 32; ++i)
		{
			rgbpal[i].R = 0xFFu;
			rgbpal[i].G = 0xFFu;
			rgbpal[i].B = 0xFFu;
		}
		for (i = 33; i < 64; ++i)
		{
			rgbpal[i].R = 0xFFu;
			rgbpal[i].G = 0xFFu;
			rgbpal[i].B = 0xFFu;
		}
	}
	else
	{
		for (i = 0; i < 64; ++i)
		{
			rgbpal[i].R = MDColorTable[(mdpal[i] & 0xF) >> 1];
			rgbpal[i].G = MDColorTable[((mdpal[i] >> 4) & 0xF) >> 1];
			rgbpal[i].B = MDColorTable[((mdpal[i] >> 8) & 0xF) >> 1];
		}
	}
	bgcol = rgbpal[TransparentColorIndex];
	rgbpal[0] = bgcol;
	rgbpal[16] = bgcol;
	rgbpal[32] = bgcol;
	rgbpal[48] = bgcol;
	CopyPalette2(palette_dest, rgbpal);
}

extern "C"
{
	__declspec(dllexport) void Init(const wchar_t *path, const HelperFunctions &helperFunctions)
	{
		WriteJump((void*)0x405536, ConvertPalette_r);
		WriteJump((void*)0x4056F7, ConvertPalette2_r);
	}

	__declspec(dllexport) ModInfo SKCModInfo = { ModLoaderVer };
}