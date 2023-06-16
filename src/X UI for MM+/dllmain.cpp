#include <Windows.h>
#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

#include "SprGamCmnLayers.h"

__int8 modifiers;
__int8 difficulty;
__int8 hitEffect;
uint8_t percentage;
unsigned short int pv;

bool noSlides;
bool modifiersFix;
bool xBarRank;
bool enValue;
bool judgementsValue;
bool altFailed;

bool noHitEffchange;

unsigned char u1;
unsigned char u2;
unsigned char u3;

unsigned char u4;
unsigned char u5;
unsigned char u6;

toml::table config;

toml::table hitEffects;

toml::v3::node_view<toml::v3::node> coolAura;
toml::v3::node_view<toml::v3::node> elegantAura;
toml::v3::node_view<toml::v3::node> quirkyAura;
toml::v3::node_view<toml::v3::node> cuteAura;
toml::v3::node_view<toml::v3::node> classicAura;

SIG_SCAN(sigStartSong, 0x1406F2820, "\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x30\x4C\x8B\xFA\x45\x0F", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigModifiers, 0x14027BEE0, "\x48\x8B\x01\x89\x50\x20\xC3\xCC", "xxxxxxxx")
SIG_SCAN(sigPv, 0x14043B310, "\x8B\xD1\xE9\xA9\xE8\xFF\xFF\xCC", "xxxxxxxx")

void hit_effects_0x_x(unsigned char v1 = 'e', unsigned char v2 = 'f', unsigned char v3 = 'f') {

	if (noSlides) {
		u4 = 0x00;
		u5 = u4;
		u6 = u4;
	}
	else {
		u4 = 'l';
		u5 = 'r';
		u6 = '_';
	}

	void* HitEffArrays[] = {
		sigHitEff00L(),
		sigHitEff00R(),
		sigHitEff01L(),
		sigHitEff01R()
	};

	for (int i = 0; i < sizeof(HitEffArrays) / sizeof(HitEffArrays[0]); i++) {
		WRITE_MEMORY((char*)HitEffArrays[i] + 0x04, uint8_t, v1, v2, v3, '0', '0', u6, (i % 2 == 0) ? u4 : u5);
	}
}

void cloud_hit_effects(unsigned char v1, unsigned char v2, unsigned char v3) {

	void* HitEffArrays[] = {
		sigHitEff00(),
		sigHitEff01(),
		sigHitEff02(),
		sigHitEff03(),
		sigHitEff04(),
		sigHitEffSlide01L(),
		sigHitEffSlide01R()
	};

	for (int i = 0; i < sizeof(HitEffArrays) / sizeof(HitEffArrays[0]); i++) {
		WRITE_MEMORY((char*)HitEffArrays[i] + 0x04, uint8_t, v1, v2, v3);
	}

	hit_effects_0x_x(v1, v2, v3);
}

void modifiers_fix(unsigned char v1 = 't', unsigned char v2 = 'x', unsigned char v3 = 't') {

	if (v1 == 't' && v2 == 'x' && v3 == 't') {
		u1 = 'p';
		u2 = 'r';
		u3 = 'c';
	}
	else {
		u1 = v1;
		u2 = v2;
		u3 = v3;
	}

	WRITE_MEMORY((char*)sigSongEnergyClearTxt() + 0x12, uint8_t, v1, v2, v3);
	WRITE_MEMORY(sigPrcGaugeBaseArcade(), uint8_t, u1, u2, u3);
}

void bar_difficulty(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {

	WRITE_MEMORY(sigSongEnergyReach(), uint8_t, v1, v2, v3, v4);
	WRITE_MEMORY(sigSongEnergyBorder(), uint8_t, v1, v2, v3, v4);
}

void combo_fine_judgements(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4 = ' ', unsigned char v5 = ' ') {

	if (v4 == v5 && v4 == ' ') {
		if (enValue) {
			v4 = 'e';
			v5 = 'n';
		}
		else if (judgementsValue) {
			v4 = 'b';
			v5 = 'o';
		}
	}

	void* comboArrays[] = {
		sigComboTempoFine001(),
		sigComboTempoFine010(),
		sigComboTempoFine100(),
		sigComboTempoFine100B(),
		sigComboTempoFine1000(),
		sigComboFine001(),
		sigComboFine010(),
		sigComboFine100(),
		sigComboFine100B(),
		sigComboFine1000()
	};

	for (int i = 0; i < sizeof(comboArrays) / sizeof(comboArrays[0]); i++) {
		WRITE_MEMORY(comboArrays[i], uint8_t, v1, v2, v3, v4, v5);
	}
}

void value_fine_judgements(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4 = ' ', unsigned char v5 = ' ') {

	if (v4 == v5 && v4 == ' ') {
		if (enValue) {
			v4 = 'e';
			v5 = 'n';
		}
		else {
			v4 = 'u';
			v5 = 'e';
		}
	}

	WRITE_MEMORY(sigValueTextFine01(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigValueTextFine03(), uint8_t, v1, v2, v3, v4, v5);
}

HOOK(__int64, __fastcall, _Modifiers, sigModifiers(), __int64* a1, int a2) {

	original_Modifiers(a1, a2);
	modifiers = a2;

	switch (modifiers) {
		case 1:
			modifiers_fix('h', 'i', 's');
			break;
		case 2:
			modifiers_fix('h', 'i', 'd');
			break;
		case 3:
			modifiers_fix('s', 'u', 'd');
			break;
		default:
			modifiers_fix();
			break;
	}

	return 0;
}

void no_hit_effects_change() {

	noHitEffchange = false;
}

HOOK(void, __fastcall, _Pv, sigPv(), __int64 a1) {

	original_Pv(a1);
	pv = static_cast<int>(a1);
	noHitEffchange = true;

	struct Clouds {
		toml::v3::node_view<toml::v3::node> aura;
		size_t size;
		char a;
		char b;
		char c;
	} auras[] = {
		{coolAura, coolAura.as_array()->size(), 'c', 'r', 'c'},
		{elegantAura, elegantAura.as_array()->size(), 's', 'b', 'c'},
		{quirkyAura, quirkyAura.as_array()->size(), 'u', 'c', 'c'},
		{cuteAura, cuteAura.as_array()->size(), 'c', 'c', 'c'},
		{classicAura, classicAura.as_array()->size(), 'n', 't', 'c'}
	};


	for (const auto& clouds : auras) {
		for (int i = 0; i < clouds.size; i++) {
			if (clouds.aura[i] == pv) {
				cloud_hit_effects(clouds.a, clouds.b, clouds.c);
				no_hit_effects_change();
				break;
			}
		}
		if (!noHitEffchange) break;
	}

	if (noHitEffchange) cloud_hit_effects('e', 'f', 'f');
}

HOOK(void, __fastcall, _StartSong, sigStartSong(), int* a1, __int64 a2, char a3) {

	original_StartSong(a1, a2, a3);
	difficulty = *a1;

	switch (difficulty) {
		case 1:
			bar_difficulty('n', 'o', 'r', 'm');
			break;
		case 2:
			bar_difficulty('h', 'a', 'r', 'd');
			break;
		case 3:
			bar_difficulty('e', 'x', 't', 'r');
			break;
		default:
			bar_difficulty('e', 'a', 's', 'y');
			break;
	}
}

extern "C" __declspec(dllexport) void Init() {

	printf("[X UI for MM+] Initializing...\n");
	try {
		config = toml::parse_file("config.toml");

		noSlides = config["No_Sliders_Effect"].value_or(false);
		percentage = config["Percentage"].value_or(0);
		modifiersFix = config["Fix_Modifiers"].value_or(false);
		xBarRank = config["X_Ranking_Bar"].value_or(false);
		hitEffect = config["Hit_Effect"].value_or(0);
		enValue = config["English_Values"].value_or(false);
		judgementsValue = config["Judgements_Values"].value_or(false);
		altFailed = config["Alternative_Fail"].value_or(false);

		hitEffects = toml::parse_file("hitEffects.toml");

		coolAura = hitEffects["Cool"];
		elegantAura = hitEffects["Elegant"];
		quirkyAura = hitEffects["Quirky"];
		cuteAura = hitEffects["Cute"];
		classicAura = hitEffects["Classic"];
	}
	catch (std::exception& exception) {
		char text[1024];
		sprintf_s(text, "Failed to parse config.toml or hitEffects.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "X UI for MM+", MB_OK | MB_ICONERROR);
	}

	if (hitEffect == 6) INSTALL_HOOK(_Pv);

	if (modifiersFix) {
		INSTALL_HOOK(_Modifiers);

		void* optionInfoArrays[] = {
			sigOptionInfoHiddenA(),
			sigOptionInfoHispeedA(),
			sigOptionInfoSuddenA(),
			sigOptionInfoBase()
		};

		for (int i = 0; i < sizeof(optionInfoArrays) / sizeof(optionInfoArrays[0]); i++) {
			WRITE_MEMORY(optionInfoArrays[i], uint8_t, 0x00);
		}
	}

	if (xBarRank) {
		INSTALL_HOOK(_StartSong);

		void* songEnergyArrays[] = {
			sigSongEnergyBorderExcellent(),
			sigSongEnergyBorderGreat(),
			sigSongEnergyEffReach()
		};

		for (int i = 0; i < sizeof(songEnergyArrays) / sizeof(songEnergyArrays[0]); i++) {
			WRITE_MEMORY(songEnergyArrays[i], uint8_t, 0x00);
		}
	}

	if (enValue) {
		combo_fine_judgements('c', 'o', 'm', 'e', 'n');
		value_fine_judgements('v', 'a', 'l', 'e', 'n');

		void* valueTextArrays[] = {
			sigValueTextSad(),
			sigValueTextWrong01(),
			sigValueTextWrong02(),
			sigValueTextWrong03(),
			sigValueTextWrong04()
		};

		for (int i = 0; i < sizeof(valueTextArrays) / sizeof(valueTextArrays[0]); i++) {
			WRITE_MEMORY((char*)valueTextArrays[i] + 0x03, uint8_t, 'e', 'n');
		}
	}

	if (judgementsValue) {
		combo_fine_judgements('a', 'l', 't');
		value_fine_judgements('a', 'l', 't');

		void* coolArrays[] = {
			sigValueTextCool01(),
			sigValueTextCool03(),
			sigComboTempoCool001(),
			sigComboTempoCool010(),
			sigComboTempoCool100(),
			sigComboTempoCool100B(),
			sigComboTempoCool1000(),
			sigComboCool001(),
			sigComboCool010(),
			sigComboCool100(),
			sigComboCool100B(),
			sigComboCool1000()
		};

		for (int i = 0; i < sizeof(coolArrays) / sizeof(coolArrays[0]); i++) {
			WRITE_MEMORY(coolArrays[i], uint8_t, 'a', 'l', 't');
		}
	}

	if (noSlides) hit_effects_0x_x();

	if (percentage != 0) {

		switch (percentage) {
			case 1:
				percentage = 'b';
				break;
			case 2:
				percentage = ' ';
				break;
			default:
				percentage = 'p';
				break;
		}

		void* (*energyNumArrays[])() = {
			sigPEnergyNum01C,
			sigPEnergyNum02C,
			sigPEnergyNum03C,
			sigPEnergyNum04C,
			sigPEnergyNum05C,
			sigPEnergyNum06C,
			sigPEnergyNum07C
		};

		for (int i = 0; i < sizeof(energyNumArrays) / sizeof(energyNumArrays[0]); i++) {
			WRITE_MEMORY(energyNumArrays[i](), uint8_t, percentage);
		}
	}

	if (hitEffect > 0 && hitEffect < 6) {
		switch (hitEffect) {
			case 1:
				cloud_hit_effects('c', 'c', 'c');
				break;
			case 2:
				cloud_hit_effects('c', 'r', 'c');
				break;
			case 3:
				cloud_hit_effects('s', 'b', 'c');
				break;
			case 4:
				cloud_hit_effects('n', 't', 'c');
				break;
			case 5:
				cloud_hit_effects('u', 'c', 'c');
				break;
			default:
				break;
		}
	}

	if (altFailed) WRITE_MEMORY(sigFailedInfo(), uint8_t, 'a', 'l', 't', 'e', 'r', 'n');
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	return TRUE;
}