#include <Windows.h>
#include <sstream>
#include <set>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

#include "SprNswgamCmnLayers.h"

int rnd;
int rnd2;
uint8_t rorn = 'r';
uint8_t txorpf[] = { 'p', 'f' };

int randomLoading = 1;

toml::v3::node_view<toml::v3::node> loadingStyle;

toml::table config;

std::set<int> loadingStyleIndices;

SIG_SCAN(sigLoadingBg, 0x140CC2158, "\x6C\x6F\x61\x64\x69\x6E\x67\x5F\x62\x67", "xxxxxxxxxx")
SIG_SCAN(sigNowLoading, 0x140CC2128, "\x6E\x6F\x77\x5F\x6C\x6F\x61\x64\x69\x6E\x67", "xxxxxxxxxxx")
SIG_SCAN(sigLoadingScreen, 0x140654280, "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xC0\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxx?xxxx?xxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxxx????xxxxx????")

void generateArray() {

	if (loadingStyle.is_array()) {
		//Add values based on Loading_Style from config.toml
		for (const auto& value : *loadingStyle.as_array()) {
			if (value.is_integer()) {
				int64_t style = value.as_integer()->get();

				switch (style) {
				case 0: // M39
				case 1: // X
				case 2: // FT
				case 3: // F2nd
				case 4: // F
				case 5: // DT2nd&DTEx(PS3)
				case 6: // 2nd&Ex(PSP)
				case 7: // DT
					loadingStyleIndices.insert(static_cast<int>(style));
					break;
				default:
					continue; // Just ignore if invalid
				}
			}
		}
	}
}

void load_bg(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {

	unsigned char zeroX = '0x0';
	std::stringstream sstream;
	sstream << std::hex << v1;
	sstream << std::hex << v2;
	sstream << std::hex << v3;
	sstream << std::hex << v4;
	std::string result = sstream.str();
	unsigned char V1 = zeroX + static_cast<unsigned char>(v1);
	unsigned char V2 = zeroX + static_cast<unsigned char>(v2);
	unsigned char V3 = zeroX + static_cast<unsigned char>(v3);
	unsigned char V4 = zeroX + static_cast<unsigned char>(v4);
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
	load_bg(static_cast<unsigned char>(first), static_cast<unsigned char>(second), static_cast<unsigned char>(third), static_cast<unsigned char>(fourth));
}

void set_load_style() {
	uint8_t loadStyle[3];

	if (loadingStyleIndices.empty()) {

		printf("[Complete Loading Screen Customizable for MM+] Warning : No loading style set. Using default values. Please check values in Loading_Style in config.toml.\n");

		// Use default values if Loading_Style from config.toml is empty
		for (int i = 0; i <= 7; i++) {
			loadingStyleIndices.insert(i);
		}
	}

	auto it = loadingStyleIndices.begin();
	std::advance(it, rand() % loadingStyleIndices.size());
	int randomStyle = *it;

	switch (randomStyle) {

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

	if (randomStyle != 0) {
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

HOOK(__int64, __fastcall, _LoadingScreen, sigLoadingScreen(), int a1) {

	original_LoadingScreen(a1);
	random_bg();
	set_load_style();
	return 0;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[Complete Loading Screen Customizable for MM+] Initializing...\n");
	
	try {

		config = toml::parse_file("config.toml");
		try {

			randomLoading = config["Random_Loading"].value_or(0);
			loadingStyle = config["Loading_Style"];
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
	else if (randomLoading > 1019) {

		randomLoading = 1019;
	}

	srand(time(NULL));
	generateArray();
	random_bg();
	set_load_style();

	INSTALL_HOOK(_LoadingScreen);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}