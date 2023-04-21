#include <Windows.h>
#include <sstream>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

int rnd, rnd2, defaultLoading, randomLoading, loadingStyle;

toml::table config;

SIG_SCAN(sigLoadingBg, 0x140CC2158, "\x6C\x6F\x61\x64\x69\x6E\x67\x5F\x62\x67\x00\x00\x00\x00\x00\x00", "xxxxxxxxxx??????")
SIG_SCAN(sigNowLoading, 0x140CC2128, "\x6E\x6F\x77\x5F\x6C\x6F\x61\x64\x69\x6E\x67\x00\x00\x00\x00\x00", "xxxxxxxxxxx?????")
SIG_SCAN(sigLoadingScreen, 0x140654280, "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xC0\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxx?xxxx?xxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxxx????xxxxx????")

void load_bg(unsigned char v1, unsigned char v2, unsigned char v3) {
	
	unsigned char zeroX = '0x0';
	std::stringstream sstream;
	sstream << std::hex << v1;
	sstream << std::hex << v2;
	sstream << std::hex << v3;
	std::string result = sstream.str();
	unsigned char V1 = zeroX + v1;
	unsigned char V2 = zeroX + v2;
	unsigned char V3 = zeroX + v3;
	WRITE_MEMORY((char*)sigLoadingBg() + 0x07, uint8_t, V1, V2, V3);
}

void random_bg() {

	switch (randomLoading) {

		case 0: //X (0-199) 
			rnd = rand() % 200;
			break;

		case 1: //F2nd (200-355)
			rnd = 200 + rand() % 156;
			break;

		case 2: //F (356-524)
			rnd = 356 + rand() % 169;
			break;

		default: //All
			rnd = rand() % 525;
			break;
	}

	int first = rnd / 100;
	rnd = rnd % 100;
	int second = rnd / 10;
	rnd = rnd % 10;
	int third = rnd;
	load_bg(first, second, third);
}

void set_load_style(int x1) {

	switch (x1) {

		case 1: //F2nd
			WRITE_MEMORY(sigNowLoading(), uint8_t, 'd', 'f', '2');
			break;

		case 2: //F
			WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'j', 'f');
			break;

		default: //X
			WRITE_MEMORY(sigNowLoading(), uint8_t, 'n', 'o', 'w');
			break;
	}
}

void random_load() {

	rnd2 = (rand() % 3);
	set_load_style(rnd2);
}


HOOK(__int64, __fastcall, _LoadingScreen, sigLoadingScreen(), int a1) {

	original_LoadingScreen(a1);
	if (0<=randomLoading && randomLoading <=3) {

		random_bg();
	}
	if (loadingStyle == 3) {

		random_load();
	}
	return 0;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[F, F2nd and X UI Loading for MM+] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			defaultLoading = config["Default_Loading"].value_or(0);
			randomLoading = config["Random_Loading"].value_or(0);
			loadingStyle = config["Loading_Style"].value_or(0);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "F, F2nd and X UI Loading for MM+", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "F, F2nd and X UI Loading for MM+", MB_OK | MB_ICONERROR);
	}

	srand(time(NULL));

	if (defaultLoading == 1) {

		WRITE_MEMORY(sigLoadingBg(), uint8_t, 'l', 'o', 'a', 'd', 'i', 'n', 'g', '_', 'f', '2');
	}

	if (0<=randomLoading && randomLoading <=3) {

		random_bg();
		INSTALL_HOOK(_LoadingScreen);
	}

	if (loadingStyle != 3) {

		switch (loadingStyle) {

			case 1: //F2nd
				WRITE_MEMORY(sigNowLoading(), uint8_t, 'd', 'f', '2');
				break;

			case 2: //F
				WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'j', 'f');
				break;

			default: //X
				WRITE_MEMORY(sigNowLoading(), uint8_t, 'n', 'o', 'w');
				break;
		}
	}
	else {

		random_load();
		INSTALL_HOOK(_LoadingScreen);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}