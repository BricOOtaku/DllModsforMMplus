#include <Windows.h>
#include <iostream>

#include "detours.h"
#include "Helpers.h"
#include "toml.hpp"
#include "xui.h"

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

bool coloredTarget;
bool smallerTarget;

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

void WriteInMemory(std::vector<void*> Arrays, int Offset, std::initializer_list<uint8_t> values) {
	
	for (size_t i = 0; i < Arrays.size(); i++) {
		if (Arrays[i] == nullptr) continue;
		size_t j = 0;
		for (auto value : values) {
			WRITE_MEMORY((char*)Arrays[i] + Offset + j, uint8_t, value);
			j++;
		}
	}
}

bool isDllLoaded(const std::string& dllName) {
    std::vector<HMODULE> hMods(1024);
    DWORD cbNeeded;

    if (EnumProcessModules(GetCurrentProcess(), hMods.data(), static_cast<DWORD>(hMods.size() * sizeof(HMODULE)), &cbNeeded)) {
        for (size_t i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char filepath[MAX_PATH];
            if (GetModuleFileNameA(hMods[i], filepath, sizeof(filepath) / sizeof(char))) {
                std::string fileName = std::strrchr(filepath, '\\') ? std::strrchr(filepath, '\\') + 1 : filepath;

                if (fileName == dllName) {
                    return true;
                }
            }
        }
    }
    return false;
}

void FT_Compatibility() {

	if (isDllLoaded("ps4.dll")) {
		std::cerr << "[X UI for MM+] ps4.dll is loaded..." << std::endl;
	}
	else {
		std::cerr << "[X UI for MM+] ps4.dll is not loaded. Patching..." << std::endl;

		WriteInMemory(FTPatchXmmArrays, 0, { 'x', 'm', 'm' });
		WriteInMemory(FTPatchMmArrays, 0, { 'm', 'm' });
	}
}

void hit_effects_0x_x(unsigned char v1 = 'e', unsigned char v2 = 'f', unsigned char v3 = 'f') {

	if (noSlides) {
		u4 = u5 = u6 = 0x00;
	}
	else {
		u4 = 'l';
		u5 = 'r';
		u6 = '_';
	}

	WriteInMemory(HitEffLArrays, 0x04, { v1, v2, v3, '0', '0', u6, u4 });
	WriteInMemory(HitEffRArrays, 0x04, { v1, v2, v3, '0', '0', u6, u5 });
}

void cloud_hit_effects(unsigned char v1, unsigned char v2, unsigned char v3) {

	WriteInMemory(HitEffArrays, 0x04, { v1, v2, v3 });
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

	WriteInMemory(songEnergyAArrays, 0, { v1, v2, v3, v4 });
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

	WriteInMemory(comboFineArrays, 0, { v1, v2, v3, v4, v5 });
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

	WriteInMemory(valueFineArrays, 0, { v1, v2, v3, v4, v5 });
}

void setColoredTarget() {

	WriteInMemory(targetArrays, 0x04, { 'c' });
}

void setSmallerTarget() {

	WriteInMemory(buttonArrays, 0x05, { 's' });
	WriteInMemory(targetArrays, 0x05, { 's' });
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
	};

	std::vector<Clouds> auras = {
		{coolAura, coolAura.as_array()->size(), 'c', 'r', 'c'},
		{elegantAura, elegantAura.as_array()->size(), 's', 'b', 'c'},
		{quirkyAura, quirkyAura.as_array()->size(), 'u', 'c', 'c'},
		{cuteAura, cuteAura.as_array()->size(), 'c', 'c', 'c'},
		{classicAura, classicAura.as_array()->size(), 'n', 't', 'c'}
	};


	for (const auto& clouds : auras) {
		for (size_t i = 0; i < clouds.size; i++) {
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

void handleParseException(const std::exception& exception) {

	std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
	MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Vocaloids sing SEGA +", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {

	std::cerr << "[X UI for MM+] Initializing..." << std::endl;
	try {
		config = toml::parse_file("config.toml");
		try {
			noSlides = config["No_Sliders_Effect"].value_or(false);
			percentage = config["Percentage"].value_or(0);
			modifiersFix = config["Fix_Modifiers"].value_or(false);
			xBarRank = config["X_Ranking_Bar"].value_or(false);
			hitEffect = config["Hit_Effect"].value_or(0);
			enValue = config["English_Values"].value_or(false);
			judgementsValue = config["Judgements_Values"].value_or(false);
			altFailed = config["Alternative_Fail"].value_or(false);
			coloredTarget = config["Colored_Target"].value_or(false);
			smallerTarget = config["Smaller_Target"].value_or(false);
			
			hitEffects = toml::parse_file("hitEffects.toml");

			coolAura = hitEffects["Cool"];
			elegantAura = hitEffects["Elegant"];
			quirkyAura = hitEffects["Quirky"];
			cuteAura = hitEffects["Cute"];
			classicAura = hitEffects["Classic"];
		}
		catch (std::exception& exception) {
			handleParseException(exception);
		}
	}
	catch (std::exception& exception) {
		handleParseException(exception);
	}
	FT_Compatibility();

	if (hitEffect == 6) INSTALL_HOOK(_Pv);

	if (modifiersFix) {
		INSTALL_HOOK(_Modifiers);
		WriteInMemory(optionInfoArrays, 0, { 0x00 });
	}

	if (xBarRank) {
		INSTALL_HOOK(_StartSong);
		WriteInMemory(songEnergyBArrays, 0, { 'b' });

	}

	if (enValue) {
		combo_fine_judgements('c', 'o', 'm', 'e', 'n');
		value_fine_judgements('v', 'a', 'l', 'e', 'n');
		WriteInMemory(valueTextArrays, 0x03, { 'e', 'n' });
	}

	if (judgementsValue) {
		combo_fine_judgements('a', 'l', 't');
		value_fine_judgements('a', 'l', 't');
		WriteInMemory(coolArrays, 0, { 'a', 'l', 't' });
	}

	if (noSlides) hit_effects_0x_x();

	if (percentage != 0) {

		switch (percentage) {
		case 1:
			percentage = 'b';
			break;
		case 2:
			percentage = 'n';
			break;
		default:
			percentage = 'p';
			break;
		}
		WriteInMemory(energyNumArrays, 0, { percentage });
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

	if (coloredTarget) setColoredTarget();
	if (smallerTarget) setSmallerTarget();
}