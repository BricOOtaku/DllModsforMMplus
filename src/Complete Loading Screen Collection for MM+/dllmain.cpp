#include <Windows.h>
#include <sstream>
#include <set>
#include <iostream>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

#include "SprNswgamCmnLayers.h"

uint8_t rorn = 'r';
uint8_t txorpf[] = { 'p', 'f' };

toml::v3::node_view<toml::v3::node> randomLoading;
toml::v3::node_view<toml::v3::node> loadingStyle;
toml::v3::node_view<toml::v3::node> blacklist;

toml::table config;

std::set<int> loadingScreenIndices;
std::set<int> excludedIndices;
std::set<int> loadingStyleIndices;

SIG_SCAN(sigLoadingBg, 0x140CC2158, "\x6C\x6F\x61\x64\x69\x6E\x67\x5F\x62\x67", "xxxxxxxxxx")
SIG_SCAN(sigNowLoading, 0x140CC2128, "\x6E\x6F\x77\x5F\x6C\x6F\x61\x64\x69\x6E\x67", "xxxxxxxxxxx")
SIG_SCAN(sigLoadingScreen, 0x140654280, "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xC0\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxx?xxxx?xxxxx????xxx????xxx????xxxxxx????xxxxxxxxxxxxx????xxxxx????")

void generateArray() {
	
	if (blacklist.is_array()) {
		for (const auto& value : *blacklist.as_array()) {
			if (value.is_integer()) {
				excludedIndices.insert(static_cast<int>(value.as_integer()->get()));
			}
		}
	}

	if (randomLoading.is_array()) {
		//Add values based on Random_Loading from config.toml
		for (const auto& value : *randomLoading.as_array()) {
			if (value.is_integer()) {
				int64_t random = value.as_integer()->get();

				int start, end;
				switch (random) {
				case 0: start = 0; end = 4; break; //M39
				case 1: start = 5; end = 238; break; //X
				case 2: start = 239; end = 241; break; //FT
				case 3: start = 242; end = 398; break; //F2nd
				case 4: start = 399; end = 567; break; //F
				case 5: start = 568; end = 601; break; //EX
				case 6: start = 602; end = 806; break; //2nd
				case 7: start = 807; end = 1002; break; //1st
				case 8: start = 1003; end = 1014; break; //Steam
				case 9: start = 1015; end = 1018; break; //Flick
				default: continue; // Just ignore if invalid
				}

				for (int i = start; i <= end; i++) {
					loadingScreenIndices.insert(static_cast<int>(i));
				}
			}
		}

		//Add values based on Blacklist from config.toml
		for (const auto& value : excludedIndices) {
			loadingScreenIndices.erase(value);
		}
	}

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

	if (loadingScreenIndices.empty()) {

		std::cerr << "[Complete Loading Screen Collection for MM+] Warning : No loading screen set. Using default values (0-1018). Please check values in both Random_Loading and Blacklist in config.toml." << std::endl;

		// Use an array from 0 to 1018 if Random_Loading from config.toml is empty
		for (int i = 0; i <= 1018; i++) {
			loadingScreenIndices.insert(i);
		}
	}

	auto it = loadingScreenIndices.begin();
	std::advance(it, rand() % loadingScreenIndices.size());
	int rnd = *it;

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
		
		std::cerr << "[Complete Loading Screen Collection for MM+] Warning : No loading style set. Using default values. Please check values in Loading_Style in config.toml." << std::endl;
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

void handleParseException(const std::exception& exception) {

	std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
	MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Complete Loading Screen Collection for MM+", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {

	std::cerr << "[Complete Loading Screen Collection for MM+] Initializing... " << std::endl;
	try {
		config = toml::parse_file("config.toml");
		try {
			randomLoading = config["Random_Loading"]; 
			loadingStyle = config["Loading_Style"]; 
			blacklist = config["Blacklist"];
		}
		catch (std::exception& exception) {
			handleParseException(exception);
		}
	}
	catch (std::exception& exception) {
		handleParseException(exception);
	}

	srand(static_cast<unsigned int>(time(NULL)));
	generateArray();
	random_bg();
	set_load_style();

	INSTALL_HOOK(_LoadingScreen);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	return TRUE;
}
