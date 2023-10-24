#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include <random>

#include "Helpers.h"
#include "SigScan.h"
#include "detours.h"
#include "toml.hpp"

std::vector<int> order{ 0, 1, 2, 3, 4, 5, 6 };

int advSet = 0;

int selectAdv = -1;

toml::table config;

SIG_SCAN(sigAdvBg, 0x1401A9A80, "\x48\x89\x5C\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\xC0\xFA\xFF\xFF\x48\x81\xEC\x40", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sig39sLogo, 0x140C7F228, "39s_logo", "xxxxxxxx")

void shuffleOrder() {

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(order.begin(), order.end(), g);
}

void changeAdv(unsigned char v1, unsigned char v2, unsigned char v3) {
	
	WRITE_MEMORY((char*)sig39sLogo(), uint8_t, v1, v2, v3);
}

void setAdv(int adv = -1) {

	if (adv != -1) {

		advSet = adv;
	}
	else advSet %= 7;

	switch (order[advSet]) {

	case 1:
		changeAdv('i', 'c', 'h');
		break;

	case 2:
		changeAdv('k', 'a', 'n');
		break;

	case 3:
		changeAdv('k', 'o', 'h');
		break;

	case 4:
		changeAdv('m', 'i', 'n');
		break;

	case 5:
		changeAdv('m', 'i', 'k');
		break;

	case 6:
		changeAdv('t', 's', 'u');
		break;

	default:
		changeAdv('3', '9', 's');
		break;
	}
	advSet++;
}

HOOK(char, __fastcall, _ADVBG, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {

	original_ADVBG(a1, a2, a3, a4);
	setAdv();
	return 1;
}

extern "C" __declspec(dllexport) void Init() {

	printf("[Project Sekai 3rd anniversary title screen] Initializing...\n");
	try {

		config = toml::parse_file("config.toml");
		try {

			selectAdv = config["Select_Adv"].value_or(0);
		}
		catch (std::exception& exception) {

			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "Project Sekai 3rd anniversary title screen", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {

		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "Project Sekai 3rd anniversary title screen", MB_OK | MB_ICONERROR);
	}

	if (0 <= selectAdv && selectAdv < 7) {

		setAdv(selectAdv);
	}
	else {
		if (selectAdv == -1) shuffleOrder();
		
		INSTALL_HOOK(_ADVBG);
		setAdv();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}
