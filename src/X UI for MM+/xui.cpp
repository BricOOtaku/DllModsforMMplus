#include <Windows.h>
#include <iostream>
#include <random>
#include <set>
#include <unordered_map>
#include "detours.h"
#include "Helpers.h"
#include "toml.hpp"
#include "xui.h"

typedef int i32;
typedef i32(__cdecl* IsFutureToneProc)();
auto IsFutureToneAddress = reinterpret_cast<IsFutureToneProc>(sigIsFutureTone());

typedef void* (__cdecl* SongInfoFunc_t)();
auto getSongInfo = reinterpret_cast<SongInfoFunc_t>(sigSongInfo());

uint32_t frameValueBtm;
uint32_t chanceValueBtm;
uintptr_t FrameChanceoffset = 0x06;
uintptr_t FramePriorityoffset = 0x0C;

toml::v3::node_view<toml::v3::node> skinsList;
std::vector<int> skinsIndices;

__int8 modifiers;
__int8 difficulty;
__int8 hitEffect;
__int8 skins;
__int8 skinMode;
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

int currentSkin = 0;

unsigned char u1;
unsigned char u2;
unsigned char u3;

unsigned char u4;
unsigned char u5;
unsigned char u6;

toml::table config;
toml::table hitEffects;
toml::table skinsData;

toml::v3::node_view<toml::v3::node> defaultAura;
toml::v3::node_view<toml::v3::node> coolAura;
toml::v3::node_view<toml::v3::node> elegantAura;
toml::v3::node_view<toml::v3::node> quirkyAura;
toml::v3::node_view<toml::v3::node> cuteAura;
toml::v3::node_view<toml::v3::node> classicAura;
std::vector<toml::v3::node_view<toml::v3::node>> skn(58);

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

void FT_Compatibility() {

	if (!IsFutureToneAddress()) {
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

	WriteInMemory(songEnergyArrays, 0, { v1, v2, v3, v4 });
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

void fixFrameSkinSlide(__int8 skn = -1) {

	chanceValueBtm = 1126728832;  // X Chance Bottom
	frameValueBtm = 1126568448;   // X Default Bottom
	switch (skn) {
		case 14: case 16: case 23: case 24: case 5: case 8: case 9:
		case 10: case 34: case 32: case 33: case 53: case 54: case 55:
			chanceValueBtm += 2000000;
			frameValueBtm += 2000000;
			break;
		case 7: case 44:
			chanceValueBtm += 3000000;
			frameValueBtm += 3000000;
			break;
		case 36: case 45: case 6: case 52: case 18:
			chanceValueBtm += 4000000;
			frameValueBtm += 4000000;
			break;
		case 19: case 22: case 26: case 31: case 50: case 51:
			chanceValueBtm += 5000000;
			frameValueBtm += 5000000;
			break;
		case 27: case 35: case 21: case 28: case 47: case 57: case 29:
		case 56: case 30: case 25: case 38: case 48: case 49: case 17:
		case 42: case 43: case 11: case 39: case 40: case 41: case 12:
		case 20: case 46: case 37:
			chanceValueBtm += 6000000;
			frameValueBtm += 6000000;
			break;
	}

	// Frame Top
	// 0x140274E23
	// 0x140274E5F
	// 0x140274E7C

	// Chance Bottom
	WRITE_MEMORY((char*)sigSlideFrame() - 0x28 + FrameChanceoffset, uint32_t, chanceValueBtm); //0x140274E41
	WRITE_MEMORY((char*)sigSlideFrame() - 0x14 + FrameChanceoffset, uint32_t, chanceValueBtm); //0x140274E55

	// Frame Bottom
	WRITE_MEMORY((char*)sigSlideFrame() - 0x3C + FrameChanceoffset, uint32_t, frameValueBtm);  //0x140274E2D
	WRITE_MEMORY((char*)sigSlideFrame() - 0x1E + FrameChanceoffset, uint32_t, frameValueBtm);  //0x140274E4B
	WRITE_MEMORY((char*)sigSlideFrame() + 0x27 + FrameChanceoffset, uint32_t, frameValueBtm);  //0x140274E90
}

void setSkin(__int8 skin) {

	unsigned char skn[3];
	sprintf_s(reinterpret_cast<char*>(skn), sizeof(skn), "%02d", skin);
	if (skn[1] == '\0') skn[1] = '0';

	WriteInMemory(FrameUpBottomArrays, 0, { 's', 'k', 'n', skn[0], skn[1] });
	fixFrameSkinSlide(skin);
}

void generateSkinsArray() {

	std::set<int> uniqueIndices;

	if (config["Skins_List"].is_array()) {
		for (const auto& value : *config["Skins_List"].as_array()) {
			if (value.is_integer()) {
				int index = static_cast<int>(value.as_integer()->get());
				if (index >= 0 && index <= 57) {
					uniqueIndices.insert(index);
				}
				else {
					std::cerr << "[X UI for MM+] Warning: Invalid skins index " << index << ". Valid indices are 0-57.\n" << std::endl;
				}
			}
		}
	}

	if (uniqueIndices.empty()) {
		for (int i = 0; i <= 57; i++) {
			uniqueIndices.insert(i);
		}
	}

	skinsIndices.assign(uniqueIndices.begin(), uniqueIndices.end());

	if (skinMode == 1) {
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(skinsIndices.begin(), skinsIndices.end(), g);
	}

	/*
	std::cerr << "[X UI for MM+] Skins Indices: [";
	for (size_t i = 0; i < skinsIndices.size(); ++i) {
		std::cerr << skinsIndices[i];
		if (i != skinsIndices.size() - 1) {
			std::cerr << ", ";
		}
	}
	std::cerr << "]\n";*/
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

HOOK(char, __fastcall, _PvId, sigPvId(), __int64* a1, __int64 a2, __int64 a3) {

	char result = original_PvId(a1, a2, a3);

	__int64* address = (__int64*)(*a1 + 185192);
	int pvId = *reinterpret_cast<int*>(*address);

	if (hitEffect == 6) {
		noHitEffchange = true;

		struct Clouds {
			toml::v3::node_view<toml::v3::node> aura;
			size_t size;
			char a;
			char b;
			char c;
		};

		std::vector<Clouds> auras = {
			{defaultAura, defaultAura.as_array()->size(), 'e', 'f', 'f'},
			{coolAura, coolAura.as_array()->size(), 'c', 'r', 'c'},
			{elegantAura, elegantAura.as_array()->size(), 's', 'b', 'c'},
			{quirkyAura, quirkyAura.as_array()->size(), 'u', 'c', 'c'},
			{cuteAura, cuteAura.as_array()->size(), 'c', 'c', 'c'},
			{classicAura, classicAura.as_array()->size(), 'n', 't', 'c'}
		};

		//In case of same numbers for multiples array inside hitEffects.toml
		std::vector<std::tuple<char, char, char>> possibleEffects;

		for (const auto& clouds : auras) {
			for (size_t i = 0; i < clouds.size; i++) {
				if (clouds.aura[i] == pvId) {
					possibleEffects.emplace_back(clouds.a, clouds.b, clouds.c);
				}
			}
		}

		if (!possibleEffects.empty()) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, possibleEffects.size() - 1);
			auto selectedEffect = possibleEffects[dis(gen)];
			cloud_hit_effects(std::get<0>(selectedEffect), std::get<1>(selectedEffect), std::get<2>(selectedEffect));
			no_hit_effects_change();
		}
		else {
			cloud_hit_effects('e', 'f', 'f');
		}
	}

	if (skinMode == 2) {

		//In case of same numbers for multiples array inside skins.toml
		std::vector<int> possibleSkins;

		for (size_t i = 0; i < skn.size(); ++i) {
			auto sknArray = skn[i].as_array();
			if (sknArray) {
				for (const auto& value : *sknArray) {
					if (value.is_integer()) {
						int intValue = value.as_integer()->get();
						if (intValue == pvId) {
							possibleSkins.push_back(i);
						}
					}
				}
			}
		}

		if (!possibleSkins.empty()) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, possibleSkins.size() - 1);
			int selectedSkin = possibleSkins[dis(gen)];
			setSkin(selectedSkin);
		}
		else {
			setSkin(0);
		}
	}

	return result;
}

HOOK(void, __fastcall, _StartSong, sigStartSong(), int* a1, __int64 a2, char a3) {

	uintptr_t songInfo = reinterpret_cast<uintptr_t>(getSongInfo());
	uintptr_t pvFlag = songInfo + 0x19;
	bool isPv = *reinterpret_cast<bool*>(pvFlag);

	original_StartSong(a1, a2, a3);

	if (!isPv) {
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

		if (skinMode == 0 || skinMode == 1) {
			__int8 currentSkinIndex = skinsIndices[currentSkin % skinsIndices.size()];
			setSkin(currentSkinIndex);
			currentSkin++;
		}
	}
}

/*HOOK(__int64, __fastcall, _loadSprGamCmnLayers, 0x14027A3E0, __int64 a1, unsigned int a2, int a3, int a4, int a5, __int64 a6) {
	//a3 = layers priority | a5+0x100000000 = layers address
	// if a5 == 12 => chance_frame_top / chance_frame_bottom 
	// if a5 == 10 => frame_up / frame_bottom / frame_up_danger / frame_bottom_danger

	if (a5 >= 1073741824 && 1342177280 > a5) {
		if (a5 == 1086893832) a6 = 1;
		__int64 address = static_cast<__int64>(a5) + 0x100000000;
		const char* valueAtAddress = reinterpret_cast<const char*>(address);
		std::cerr << "Value at address " << std::hex << std::showbase << address << ": " << valueAtAddress << std::endl;
	}
	__int64 result = original_loadSprGamCmnLayers(a1, a2, a3, a4, a5, a6);
	return result;
}*/

/*void patchLayersPriority(__int64 priority) {
	
	uint8_t* currentPriorityPtr = reinterpret_cast<uint8_t*>(priority + 0x03);
	uint8_t currentPriority = *currentPriorityPtr;
	WRITE_MEMORY(currentPriorityPtr, uint8_t, currentPriority - 7);
}*/

void patchLayersPriority(char* priorityPtr) { 

	uint8_t* currentPriorityPtr = reinterpret_cast<uint8_t*>(priorityPtr + 0x03); 
	uint8_t currentPriority = *currentPriorityPtr; 
	WRITE_MEMORY(currentPriorityPtr, uint8_t, currentPriority - 7); 
}

toml::array processArray(const toml::array& inputArray) {
	
	std::set<int> uniqueValues;
	toml::array uniqueValuesArray;

	for (const auto& value : inputArray) {
		bool valid = false;
		int int_value = 0;

		if (value.is_integer()) {
			int_value = value.as_integer()->get();
			valid = (int_value >= 1 && int_value <= 65535); //Limit ID range
		}

		if (valid) {
			if (uniqueValues.insert(int_value).second) {
				uniqueValuesArray.push_back(int_value);
			}
		}
	}

	return uniqueValuesArray;
}

void parseHitEffectsFile() {
	
	try {
		hitEffects = toml::parse_file("hitEffects.toml");
		
		auto processAura = [&](const std::string& key) -> toml::array {
			return processArray(*hitEffects[key].as_array());
		};

		toml::array defaultAuraTemp = processAura("Default");
		toml::array coolAuraTemp = processAura("Cool");
		toml::array elegantAuraTemp = processAura("Elegant");
		toml::array quirkyAuraTemp = processAura("Quirky");
		toml::array cuteAuraTemp = processAura("Cute");
		toml::array classicAuraTemp = processAura("Classic");

		//std::cerr << "Default: " << defaultAuraTemp << std::endl;
		//std::cerr << "Cool: " << coolAuraTemp << std::endl;
		//std::cerr << "Elegant: " << elegantAuraTemp << std::endl;
		//std::cerr << "Quirky: " << quirkyAuraTemp << std::endl;
		//std::cerr << "Cute: " << cuteAuraTemp << std::endl;
		//std::cerr << "Classic: " << classicAuraTemp << std::endl;

		hitEffects.insert_or_assign("Default", defaultAuraTemp);
		hitEffects.insert_or_assign("Cool", coolAuraTemp);
		hitEffects.insert_or_assign("Elegant", elegantAuraTemp);
		hitEffects.insert_or_assign("Quirky", quirkyAuraTemp);
		hitEffects.insert_or_assign("Cute", cuteAuraTemp);
		hitEffects.insert_or_assign("Classic", classicAuraTemp);

		defaultAura = hitEffects["Default"];
		coolAura = hitEffects["Cool"];
		elegantAura = hitEffects["Elegant"];
		quirkyAura = hitEffects["Quirky"];
		cuteAura = hitEffects["Cute"];
		classicAura = hitEffects["Classic"];
	}
	catch (const std::exception& ex) {
		std::cerr << "Failed to read hitEffects.toml: " << ex.what() << std::endl;
	}
}

void parseSkinsFile() {
	
	try {
		skinsData = toml::parse_file("skins.toml");

		for (int i = 0; i < 58; ++i) {
			std::string key = "skn" + std::string(i < 10 ? "0" : "") + std::to_string(i);
			if (skinsData[key].is_array()) {
				toml::array processedArray = processArray(*skinsData[key].as_array());
				
				//std::cerr << key << ": " << processedArray << std::endl;
				
				skinsData.insert_or_assign(key, processedArray);
				skn[i] = skinsData[key];
			}
			else {
				skn[i] = toml::v3::node_view<toml::v3::node>();
			}
		}
	}
	catch (const std::exception& ex) {
		std::cerr << "Failed to read skins.toml: " << ex.what() << std::endl;
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
			// Aracde and Mix Mode OPTIONS
			enValue = config["English_Values"].value_or(false);
			altFailed = config["Alternative_Fail"].value_or(false);
			xBarRank = config["X_Ranking_Bar"].value_or(false);

			// Arcade OPTIONS
			judgementsValue = config["Judgements_Values"].value_or(false);
			percentage = config["Percentage"].value_or(0);
			hitEffect = config["Hit_Effect"].value_or(0);
			coloredTarget = config["Colored_Target"].value_or(false);
			smallerTarget = config["Smaller_Target"].value_or(false);
			noSlides = config["No_Sliders_Effect"].value_or(false);
			modifiersFix = config["Fix_Modifiers"].value_or(false);
			
			// Skins OPTIONS
			skinMode = config["Skin_Mode"].value_or(-1);
			skinsList = config["Skins_List"];
		}
		catch (std::exception& exception) {
			handleParseException(exception);
		}
	}
	catch (std::exception& exception) {
		handleParseException(exception);
	}

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
	else if (hitEffect == 6) parseHitEffectsFile();

	//INSTALL_HOOK(_loadSprGamCmnLayers);

	if (skinMode != -1) {
		if(skinMode != 2) generateSkinsArray();
		else parseSkinsFile();
	}
	else fixFrameSkinSlide();

	INSTALL_HOOK(_PvId);

	patchLayersPriority((char*)sigFrame()); // frame_bottom 0x140278087
	patchLayersPriority((char*)sigFrame() + 0x151); // frame_top 0x1402781D8
	patchLayersPriority((char*)sigChanceFrame()); // chance_frame_bottom 0x151300C6E
	patchLayersPriority((char*)sigChanceFrame() + 0x51); // chance_frame_top 0x151300CBF

	if (altFailed) WRITE_MEMORY(sigFailedInfo(), uint8_t, 'a', 'l', 't', 'e', 'r', 'n');

	if (coloredTarget) setColoredTarget();
	if (smallerTarget) setSmallerTarget();
}

//Use of PostInit() to check that the FT UI is used beforehand
extern "C" __declspec(dllexport) void PostInit() {
	
	FT_Compatibility();
}