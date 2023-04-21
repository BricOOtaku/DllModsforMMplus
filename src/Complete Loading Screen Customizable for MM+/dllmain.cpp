#include <Windows.h>
#include <sstream>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

int rnd;
int rnd2;
uint8_t rorn = 'r';

int randomLoading = 1;
int loadingStyle = -1;
toml::table config;

SIG_SCAN(sigLoadingBg, 0x140CC2158, "\x6C\x6F\x61\x64\x69\x6E\x67\x5F\x62\x67", "xxxxxxxxxx")
SIG_SCAN(sigNowLoading, 0x140CC2128, "\x6E\x6F\x77\x5F\x6C\x6F\x61\x64\x69\x6E\x67", "xxxxxxxxxxx")
SIG_SCAN(sigLoadingScreen, 0x140654280, "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xC0\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxx?xxxx?xxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxxx????xxxxx????")

SIG_SCAN(sigRightsList, 0x140CC21E8, "\x72\x69\x67\x68\x74\x73\x5F\x6C\x69\x73\x74", "xxxxxxxxxxx")
SIG_SCAN(sigRightsBg01, 0x140CC2178, "\x72\x69\x67\x68\x74\x73\x5F\x62\x67\x30\x31", "xxxxxxxxxx?x")
SIG_SCAN(sigRightsBg02, 0x140CC2168, "\x72\x69\x67\x68\x74\x73\x5F\x62\x67\x30\x32", "xxxxxxxxxx?x")

SIG_SCAN(sigRightsBaseMan, 0x140CC2208, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x6D\x61\x6E", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseArr, 0x140CC2218, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x61\x72\x72", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseLyr, 0x140CC2228, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x6C\x79\x72", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseMus, 0x140CC21A8, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x6D\x75\x73", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBasePv, 0x140CC2250, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x70\x76", "xxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseGui, 0x140CC21F8, "\x72\x69\x67\x68\x74\x73\x5F\x62\x61\x73\x65\x5F\x67\x75\x69", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigPRightsName02dLt, 0x140CC2238, "\x70\x5F\x72\x69\x67\x68\x74\x73\x5F\x6E\x61\x6D\x65\x25\x30\x32\x64\x5F\x6C\x74", "xxxxxxxxxxxxxx?xxxxx")
SIG_SCAN(sigPRightsSongLt, 0x140CC21D0, "\x70\x5F\x72\x69\x67\x68\x74\x73\x5F\x73\x6F\x6E\x67\x5F\x6C\x74", "xxxxxxxxxxxxxxxx")

void load_bg(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {

	unsigned char zeroX = '0x0';
	std::stringstream sstream;
	sstream << std::hex << v1;
	sstream << std::hex << v2;
	sstream << std::hex << v3;
	sstream << std::hex << v4;
	std::string result = sstream.str();
	unsigned char V1 = zeroX + v1;
	unsigned char V2 = zeroX + v2;
	unsigned char V3 = zeroX + v3;
	unsigned char V4 = zeroX + v4;
	WRITE_MEMORY((char*)sigLoadingBg() + 0x04, uint8_t, 'b', 'g', V1, V2, V3, V4);
}

void random_bg() {

	rnd = rand() % randomLoading;
	
	int first = rnd / 1000;
	rnd = rnd % 1000;
	int second = rnd / 100;
	rnd = rnd % 100;
	int third = rnd / 10;
	rnd = rnd % 10;
	int fourth = rnd;
	load_bg(first, second, third, fourth);
}

void set_load_style(int x1) {

	switch (x1) {

	case 1: //X
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'j', 'x');
		break;

	case 2: //FT
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'f', 't');
		break;

	case 3: //F2nd
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'd', 'f', '2');
		break;

	case 4: //F
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'j', 'f');
		break;

	case 5: //DTEX and DT2nd (PS3)
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 's', '3');
		break;

	case 6: //EX and 2nd (PSP)
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 's', 'p');
		break;

	case 7: //1st
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'p', 'j', 'd');
		break;

	default: //M39's
		rorn = 'r';
		WRITE_MEMORY(sigNowLoading(), uint8_t, 'n', 'o', 'w');
		break;
	}

	if (x1 != 0) {

		rorn = 'n';
	}

	WRITE_MEMORY(sigRightsList(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBg01(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBg02(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBaseMan(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBaseArr(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBaseLyr(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBaseMus(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBasePv(), uint8_t, rorn);
	WRITE_MEMORY(sigRightsBaseGui(), uint8_t, rorn);
	WRITE_MEMORY((char*)sigPRightsName02dLt() + 0x02, uint8_t, rorn);
	WRITE_MEMORY((char*)sigPRightsSongLt() + 0x02, uint8_t, rorn);
}

void random_load() {

	rnd2 = (rand() % 7);
	set_load_style(rnd2);
}

HOOK(__int64, __fastcall, _LoadingScreen, sigLoadingScreen(), int a1) {

	original_LoadingScreen(a1);
	random_bg();

	if (loadingStyle == -1) //Set to Random
	{
		random_load();
	}
	return 0;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[Complete Loading Screen Customizable for MM+] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			randomLoading = config["Random_Loading"].value_or(0);
			loadingStyle = config["Loading_Style"].value_or(0);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "Complete Loading Screen Customizable for MM+", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "Complete Loading Screen Customizable for MM+", MB_OK | MB_ICONERROR);
	}

	if (randomLoading < 1) {

		randomLoading = 1;
	}
	else if (randomLoading > 1003) {

		randomLoading = 1003;
	}

	srand(time(NULL));
	random_bg();

	if (loadingStyle != -1) {
		set_load_style(loadingStyle);
	}

	INSTALL_HOOK(_LoadingScreen);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}