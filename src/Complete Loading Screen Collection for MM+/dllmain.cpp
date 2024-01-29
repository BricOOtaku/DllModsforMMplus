#include <Windows.h>
#include <sstream>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

int rnd;
int rnd2;
uint8_t rorn = 'r';
uint8_t txorpf[] = { 'p', 'f' };

int randomLoading = -1;
int loadingStyle = -1;
toml::table config;

SIG_SCAN(sigLoadingBg, 0x140CC2158, "\x6C\x6F\x61\x64\x69\x6E\x67\x5F\x62\x67", "xxxxxxxxxx")
SIG_SCAN(sigNowLoading, 0x140CC2128, "\x6E\x6F\x77\x5F\x6C\x6F\x61\x64\x69\x6E\x67", "xxxxxxxxxxx")
SIG_SCAN(sigLoadingScreen, 0x140654280, "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xC0\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxx?xxxx?xxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxxx????xxxxx????")

SIG_SCAN(sigRightsList, 0x140CC21E8, "rights_list", "xxxxxxxxxxx")
SIG_SCAN(sigRightsBg01, 0x140CC2178, "rights_bg01", "xxxxxxxxxx?x")
SIG_SCAN(sigRightsBg02, 0x140CC2168, "rights_bg02", "xxxxxxxxxx?x")

SIG_SCAN(sigRightsBaseMan, 0x140CC2208, "rights_base_man", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseArr, 0x140CC2218, "rights_base_arr", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseLyr, 0x140CC2228, "rights_base_lyr", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseMus, 0x140CC21A8, "rights_base_mus", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBasePv, 0x140CC2250, "rights_base_pv", "xxxxxxxxxxxxxx")
SIG_SCAN(sigRightsBaseGui, 0x140CC21F8, "rights_base_gui", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigPRightsName02dLt, 0x140CC2238, "p_rights_name%02d_lt", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigPRightsSongLt, 0x140CC21D0, "p_rights_song_lt", "xxxxxxxxxxxxxxxx")

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

	switch (randomLoading) {

		case 0: //M39 (0-4)
			rnd = rand() % 5;
			break;

		case 1: //X (5-238)
			rnd = 5 + rand() % 234;
			break;

		case 2: //FT (239-241)
			rnd = 239 + rand() % 3;
			break;

		case 3: //Fnd (242-398)
			rnd = 242 + rand() % 157;
			break;

		case 4: //F (399-567)
			rnd = 399 + rand() % 169;
			break;

		case 5: //EX (568-601)
			rnd = 568 + rand() % 34;
			break;

		case 6: //2nd (602-806)
			rnd = 602 + rand() % 205;
			break;

		case 7: //1st (807-1002)
			rnd = 807 + rand() % 196;
			break;

		case 8: //SteamCards (1003-1014)
			rnd = 1003 + rand() % 12;
			break;

		case 9: //MikuFlick1&2 (1015-1018)
			rnd = 1015 + rand() % 4;
			break;

		default: //All
			rnd = rand() % 1019;
			break;
	}
	
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

	uint8_t loadStyle[3];
	switch (x1) {

	case 1: //X
		loadStyle[0] = 'p', loadStyle[1] = 'j', loadStyle[2] = 'x';
		break;

	case 2: //FT
		loadStyle[0] = 'p', loadStyle[1] = 'f', loadStyle[2] = 't';
		break;

	case 3: //F2nd
		loadStyle[0] = 'd', loadStyle[1] = 'f', loadStyle[2] = '2';
		break;

	case 4: //F
		loadStyle[0] = 'p', loadStyle[1] = 'j', loadStyle[2] = 'f';
		break;

	case 5: //DTEX and DT2nd (PS3)
		loadStyle[0] = 'p', loadStyle[1] = 's', loadStyle[2] = '3';
		break;

	case 6: //EX and 2nd (PSP)
		loadStyle[0] = 'p', loadStyle[1] = 's', loadStyle[2] = 'p';
		break;

	case 7: //1st
		loadStyle[0] = 'p', loadStyle[1] = 'j', loadStyle[2] = 'd';
		break;

	default: //M39's
		loadStyle[0] = 'n', loadStyle[1] = 'o', loadStyle[2] = 'w';
		rorn = 'r';
		txorpf[0] = 't', txorpf[1] = 'x';
		break;
	}

	WRITE_MEMORY(sigNowLoading(), uint8_t, loadStyle[0], loadStyle[1], loadStyle[2]);
	
	if (x1 != 0) {
		rorn = 'n';
		txorpf[0] = 'p', txorpf[1] = 'f';
	}

	WRITE_MEMORY(0x140CC2188, uint8_t, txorpf[0], txorpf[1]);
	WRITE_MEMORY(0x140CC2198, uint8_t, txorpf[0], txorpf[1]);

	void* RornArrays[] = {
		sigRightsList(),
		sigRightsBg01(),
		sigRightsBg02(),
		sigRightsBaseMan(),
		sigRightsBaseArr(),
		sigRightsBaseLyr(),
		sigRightsBaseMus(),
		sigRightsBasePv(),
		sigRightsBaseGui(),
		(char*)sigPRightsName02dLt() + 0x02,
		(char*)sigPRightsSongLt() + 0x02,
	};
	
	for (int i = 0; i < sizeof(RornArrays) / sizeof(RornArrays[0]); i++) {
		WRITE_MEMORY(RornArrays[i], uint8_t, rorn);
	}
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

	printf("[Complete Loading Screen Collection for MM+] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			randomLoading = config["Random_Loading"].value_or(0);
			loadingStyle = config["Loading_Style"].value_or(0);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "Complete Loading Screen Collection for MM+", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "Complete Loading Screen Collection for MM+", MB_OK | MB_ICONERROR);
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
