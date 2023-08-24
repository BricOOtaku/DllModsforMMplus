#include <Windows.h>
#include <random>

#include "Helpers.h"
#include "SigScan.h"
#include "detours.h"
#include "toml.hpp"

std::vector<int> order{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };

int openingSet = 0;

int selectOpening = -1;

toml::table config;

SIG_SCAN(sigAdvBg, 0x1401A9A80, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\xC0\xFA\xFF\xFF\x48\x81\xEC\x40", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigMega39sOpOgg, 0x140C7F100, "rom/sound/bgm/MEGA39sOP.ogg", "xxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigMega39sOpOpMp4, 0x140C7F120, "rom/movie/MEGA39sOP.mp4", "xxxxxxxxxxxxxxxxxxxxxxx")

void shuffleOrder() {

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(order.begin(), order.end(), g);
}

void changeOpening(unsigned char v1, unsigned char v2, unsigned char v3) {
	
	WRITE_MEMORY((char*)sigMega39sOpOgg() + 0x12, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigMega39sOpOpMp4() + 0xe, uint8_t, v1, v2, v3);
}

void setOpening(int opening = -1) {

	if (opening != -1) {
		
		openingSet = opening;
	}
	else openingSet %= 9;

	switch (order[openingSet]) {

		case 1:
			changeOpening('D', 'T', '2');
			break;

		case 2:
			changeOpening('D', 'T', 'E');
			break;

		case 3:
			changeOpening('P', 'J', 'F');
			break;

		case 4:
			changeOpening('D', 'F', '2');
			break;

		case 5:
			changeOpening('P', 'J', 'X');
			break;

		case 6:
			changeOpening('P', 'M', '1');
			break;

		case 7:
			changeOpening('P', 'M', '2');
			break;

		case 8:
			changeOpening('P', 'M', 'R');
			break;

		default:
			changeOpening('D', 'T', '1');
			break;
	}
	openingSet++;
}

HOOK(char, __fastcall, _ADVBG, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {

	original_ADVBG(a1, a2, a3,a4);
	setOpening();
	return 1;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[Opening replacements] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			selectOpening = config["Select_Opening"].value_or(0);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "Opening replacements", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "Opening replacements", MB_OK | MB_ICONERROR);
	}

	if (0 <= selectOpening && selectOpening < 9) {
		
		setOpening(selectOpening);
	}
	else {

		if(selectOpening == -1) shuffleOrder();
		INSTALL_HOOK(_ADVBG);
		setOpening();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}
