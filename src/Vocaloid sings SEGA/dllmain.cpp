#include <Windows.h>
#include <random>

#include "Helpers.h"
#include "SigScan.h"
#include "detours.h"
#include "toml.hpp"

std::vector<int> order{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

int vocalSet = 0;

int selectVocal = -1;
bool useVariant;

toml::table config;

SIG_SCAN(sigAdvBg, 0x1401A9A80, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\xC0\xFA\xFF\xFF\x48\x81\xEC\x40", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSegaLogoOgg, 0x140C7F0BC, "rom/sound/bgm/SEGAlogo_Attack_White_Ver.4.ogg", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")


void shuffleOrder() {

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(order.begin(), order.end(), g);
}

void changeVocal(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {
	
	WRITE_MEMORY((char*)sigSegaLogoOgg() + 0x24, uint8_t, v1, v2, v3, '_', v4);
}

void setVocal(int vocal = -1, bool variant = false) {

	if (vocal != -1) {
		
		vocalSet = vocal;
		if (variant) vocalSet += 6;
	}
	else vocalSet %= 12;

	switch (order[vocalSet]) {
	
		case 1:
			changeVocal('R', 'i', 'n', '1');
			break;

		case 2:
			changeVocal('L', 'e', 'n', '1');
			break;

		case 3:
			changeVocal('L', 'u', 'k', '1');
			break;

		case 4:
			changeVocal('K', 'a', 'i', '1');
			break;

		case 5:
			changeVocal('M', 'e', 'i', '1');
			break;

		case 6:
			changeVocal('M', 'i', 'k', '2');
			break;

		case 7:
			changeVocal('R', 'i', 'n', '2');
			break;

		case 8:
			changeVocal('L', 'e', 'n', '2');
			break;

		case 9:
			changeVocal('L', 'u', 'k', '2');
			break;

		case 10:
			changeVocal('K', 'a', 'i', '2');
			break;

		case 11:
			changeVocal('M', 'e', 'i', '2');
			break;

		default:
			changeVocal('M', 'i', 'k', '1');
			break;
	}
	vocalSet++;
}

HOOK(char, __fastcall, _ADVBG, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {

	original_ADVBG(a1, a2, a3,a4);
	setVocal();
	return 1;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[Vocaloid sings SEGA] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			selectVocal = config["Select_Vocal"].value_or(0);
			useVariant = config["Use_Variant"].value_or(false);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "Vocaloid sings SEGA", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "Vocaloid sings SEGA", MB_OK | MB_ICONERROR);
	}

	if (0 <= selectVocal && selectVocal < 6) {
		
		setVocal(selectVocal, useVariant);
	}
	else {

		if(selectVocal == -2) shuffleOrder();
		INSTALL_HOOK(_ADVBG);
		setVocal();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}
