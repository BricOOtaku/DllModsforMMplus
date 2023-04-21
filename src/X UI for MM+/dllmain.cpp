#include <Windows.h>

#include "detours.h"
#include "Helpers.h"
#include "SigScan.h"
#include "toml.hpp"

int modifiers = 0;
int difficulty = 0;
int pv = 0;
int hitEffect;
uint8_t percentage = 0;

bool noSlides;
bool modifiersFix;
bool xBarRank;
bool enValue;
bool judgementsValue;
bool altFailed;

unsigned char u1;
unsigned char u2;
unsigned char u3;

unsigned char u4;
unsigned char u5;
unsigned char u6;

toml::table config;

SIG_SCAN(sigStartSong, 0x1406F2820, "\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x30\x4C\x8B\xFA\x45\x0F", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigModifiers, 0x14027BEE0, "\x48\x8B\x01\x89\x50\x20\xC3\xCC", "xxxxxxxx")
SIG_SCAN(sigPv, 0x14043B310, "\x8B\xD1\xE9\xA9\xE8\xFF\xFF\xCC", "xxxxxxxx")

SIG_SCAN(sigHitEff00, 0x140C88450, "\x68\x69\x74\x5F\x65\x66\x66\x30\x30", "xxxxxxxxx")
SIG_SCAN(sigHitEff01, 0x140C8A278, "\x68\x69\x74\x5F\x65\x66\x66\x30\x31", "xxxxxxxxx")
SIG_SCAN(sigHitEff02, 0x140C8A288, "\x68\x69\x74\x5F\x65\x66\x66\x30\x32", "xxxxxxxxx")
SIG_SCAN(sigHitEff03, 0x140C8A258, "\x68\x69\x74\x5F\x65\x66\x66\x30\x33", "xxxxxxxxx")
SIG_SCAN(sigHitEff04, 0x140C8A2A8, "\x68\x69\x74\x5F\x65\x66\x66\x30\x34", "xxxxxxxxx")

SIG_SCAN(sigHitEff00L, 0x140C8A268, "\x68\x69\x74\x5F\x65\x66\x66\x30\x30\x5F\x6C", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff00R, 0x140C8A370, "\x68\x69\x74\x5F\x65\x66\x66\x30\x30\x5F\x72", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff01L, 0x140C8A360, "\x68\x69\x74\x5F\x65\x66\x66\x30\x31\x5F\x6C", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff01R, 0x140C8A338, "\x68\x69\x74\x5F\x65\x66\x66\x30\x31\x5F\x72", "xxxxxxxxxxx")

SIG_SCAN(sigHitEffSlide01L, 0x140C8A348, "\x68\x69\x74\x5F\x65\x66\x66\x5F\x73\x6C\x69\x64\x65\x30\x31\x5F\x6C", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigHitEffSlide01R, 0x140C8A308, "\x68\x69\x74\x5F\x65\x66\x66\x5F\x73\x6C\x69\x64\x65\x30\x31\x5F\x72", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigSongEnergyClearTxt, 0x140C8ACB0, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x63\x6C\x65\x61\x72\x5F\x74\x78\x74", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigPrcGaugeBaseArcade, 0x140CC0F40, "\x70\x72\x63\x5F\x67\x61\x75\x67\x65\x5F\x62\x61\x73\x65\x5F\x61\x72\x63\x61\x64\x65", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigSongEnergyReach, 0x140C8AD10, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x72\x65\x61\x63\x68", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyBorder, 0x140C8ADA0, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x62\x6F\x72\x64\x65\x72\x00", "xxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboTempoFine001, 0x140C8AC50, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x66\x69\x6E\x65\x30\x30\x31\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine010, 0x140C8AC08, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x66\x69\x6E\x65\x30\x31\x30\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine100, 0x140C8AC20, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine100B, 0x140C8ABD8, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x5F\x62\x00", "xxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine1000, 0x140C8ABF0, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x30\x00", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboFine001, 0x140C8AAC8, "\x63\x6F\x6D\x62\x6F\x5F\x66\x69\x6E\x65\x30\x30\x31\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine010, 0x140C8AAD8, "\x63\x6F\x6D\x62\x6F\x5F\x66\x69\x6E\x65\x30\x31\x30\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine100, 0x140C8AAA8, "\x63\x6F\x6D\x62\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine100B, 0x140C8AAB8, "\x63\x6F\x6D\x62\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x5F\x62\x00", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine1000, 0x140C8ABB8, "\x63\x6F\x6D\x62\x6F\x5F\x66\x69\x6E\x65\x31\x30\x30\x30\x00", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextFine01, 0x140C8A978, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x66\x69\x6E\x65\x30\x31", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextFine03, 0x140C8A9F0, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x66\x69\x6E\x65\x30\x33", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextSad, 0x140C8AA98, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x73\x61\x64", "xxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextWrong01, 0x140C8A960, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x77\x72\x6F\x6E\x67\x30\x31", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextWrong02, 0x140C8A938, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x77\x72\x6F\x6E\x67\x30\x32", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextWrong03, 0x140C8AA80, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x77\x72\x6F\x6E\x67\x30\x33", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextWrong04, 0x140C8AA50, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x77\x72\x6F\x6E\x67\x30\x34", "xxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigOptionInfoHiddenA, 0x140C8B188, "\x6F\x70\x74\x69\x6F\x6E\x5F\x69\x6E\x66\x6F\x5F\x68\x69\x64\x64\x65\x6E\x5F\x61", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoHispeedA, 0x140C8B1B8, "\x6F\x70\x74\x69\x6F\x6E\x5F\x69\x6E\x66\x6F\x5F\x68\x69\x73\x70\x65\x65\x64\x5F\x61", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoSuddenA, 0x140C8B158, "\x6F\x70\x74\x69\x6F\x6E\x5F\x69\x6E\x66\x6F\x5F\x73\x75\x64\x64\x65\x6E\x5F\x61", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoBase, 0x140C8B1A0, "\x6F\x70\x74\x69\x6F\x6E\x5F\x69\x6E\x66\x6F\x5F\x62\x61\x73\x65", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigSongEnergyBorderExcellent, 0x140C8AE10, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x62\x6F\x72\x64\x65\x72\x5F\x65\x78\x63\x65\x6C\x6C\x65\x6E\x74", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyBorderGreat, 0x140C8ADF0, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x62\x6F\x72\x64\x65\x72\x5F\x67\x72\x65\x61\x74", "xxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyEffReach, 0x140C8AC98, "\x73\x6F\x6E\x67\x5F\x65\x6E\x65\x72\x67\x79\x5F\x65\x66\x66\x5F\x72\x65\x61\x63\x68", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextCool01, 0x140C8AA20, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x63\x6F\x6F\x6C\x30\x31", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextCool03, 0x140C8AA38, "\x76\x61\x6C\x75\x65\x5F\x74\x65\x78\x74\x5F\x63\x6F\x6F\x6C\x30\x33", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboTempoCool001, 0x140C8AB48, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x63\x6F\x6F\x6C\x30\x30\x31\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool010, 0x140C8AB60, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x63\x6F\x6F\x6C\x30\x31\x30\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool100, 0x140C8AC68, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x00", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool100B, 0x140C8AC80, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x5F\x62\x00", "xxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool1000, 0x140C8AC38, "\x63\x6F\x6D\x62\x6F\x5F\x74\x65\x6D\x70\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x30\x00", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboCool001, 0x140C8ABC8, "\x63\x6F\x6D\x62\x6F\x5F\x63\x6F\x6F\x6C\x30\x30\x31\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool010, 0x140C8AB98, "\x63\x6F\x6D\x62\x6F\x5F\x63\x6F\x6F\x6C\x30\x31\x30\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool100, 0x140C8ABA8, "\x63\x6F\x6D\x62\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x00", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool100B, 0x140C8AB78, "\x63\x6F\x6D\x62\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x5F\x62\x00", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool1000, 0x140C8AB88, "\x63\x6F\x6D\x62\x6F\x5F\x63\x6F\x6F\x6C\x31\x30\x30\x30\x00", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigPEnergyNum01C, 0x140C8B3D8, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x31\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum02C, 0x140C8B4D0, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x32\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum03C, 0x140C8B4E8, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x33\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum04C, 0x140C8B4A0, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x34\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum05C, 0x140C8B4B8, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x35\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum06C, 0x140C8B470, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x36\x5F\x63", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum07C, 0x140C8B488, "\x70\x5F\x65\x6E\x65\x72\x67\x79\x5F\x6E\x75\x6D\x30\x37\x5F\x63", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigFailedInfo, 0x140C8A828, "\x66\x61\x69\x6C\x65\x64\x5F\x69\x6E\x66\x6F", "xxxxxxxxxxx")

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

	WRITE_MEMORY((char*)sigHitEff00L() + 0x04, uint8_t, v1, v2, v3, '0', '0', u6, u4);
	WRITE_MEMORY((char*)sigHitEff00R() + 0x04, uint8_t, v1, v2, v3, '0', '0', u6, u5);
	WRITE_MEMORY((char*)sigHitEff01L() + 0x04, uint8_t, v1, v2, v3, '0', '1', u6, u4);
	WRITE_MEMORY((char*)sigHitEff01R() + 0x04, uint8_t, v1, v2, v3, '0', '1', u6, u5);
}

void cloud_hit_effects(unsigned char v1, unsigned char v2, unsigned char v3) {

	WRITE_MEMORY((char*)sigHitEff00() + 0x04, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigHitEff01() + 0x04, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigHitEff02() + 0x04, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigHitEff03() + 0x04, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigHitEff04() + 0x04, uint8_t, v1, v2, v3);
	
	hit_effects_0x_x(v1, v2, v3);

	WRITE_MEMORY((char*)sigHitEffSlide01L() + 0x04, uint8_t, v1, v2, v3);
	WRITE_MEMORY((char*)sigHitEffSlide01R() + 0x04, uint8_t, v1, v2, v3);
}

void modifiers_fix(unsigned char v1 = 't', unsigned char v2 = 'x', unsigned char v3 = 't') {

	if (v1 == 't' && v2 == 'x' && v3 == 't') { //Default
		u1 = 'p';
		u2 = 'r';
		u3 = 'c';
	}
	else {
		u1 = v1;
		u2 = v2;
		u3 = v3;
	}

	WRITE_MEMORY((char*)sigSongEnergyClearTxt()+0x12, uint8_t, v1, v2, v3);
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

	WRITE_MEMORY(sigComboTempoFine001(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboTempoFine010(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboTempoFine100(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboTempoFine100B(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboTempoFine1000(), uint8_t, v1, v2, v3, v4, v5);

	WRITE_MEMORY(sigComboFine001(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboFine010(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboFine100(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboFine100B(), uint8_t, v1, v2, v3, v4, v5);
	WRITE_MEMORY(sigComboFine1000(), uint8_t, v1, v2, v3, v4, v5);
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
	
	original_Modifiers(a1,a2);
	modifiers = a2;

	if (modifiers == 1) { //his

		modifiers_fix('h', 'i', 's');
	}
	else if (modifiers == 2) { //hid

		modifiers_fix('h', 'i', 'd');
	}
	else if (modifiers == 3) { //sud

		modifiers_fix('s', 'u', 'd');
	}
	else { //txt and prc

		modifiers_fix();
	}

	return 0;
}
HOOK(void, __fastcall, _Pv, sigPv(), __int64 a1) {
	
	original_Pv(a1);
	pv = static_cast<int>(a1);

	if (pv == 801 || pv == 805 || pv == 814 || pv == 819 || pv == 820 || pv == 827) { //Cool

		cloud_hit_effects('c', 'r', 'c');
	}
	else if (pv == 802 || pv == 815 || pv == 816 || pv == 821 || pv == 822 || pv == 828) { //Elegant

		cloud_hit_effects('s', 'b', 'c');
	}
	else if (pv == 803 || pv == 804 || pv == 806 || pv == 817 || pv == 818 || pv == 829) { //Quirky

		cloud_hit_effects('u', 'c', 'c');
	}
	else if (pv == 807 || pv == 808 || pv == 811 || pv == 812 || pv == 813 || pv == 825) { //Cute

		cloud_hit_effects('c', 'c', 'c');
	}
	else if (pv == 809 || pv == 810 || pv == 823 || pv == 824 || pv == 826 || pv == 830 || pv == 831 || pv == 800) { //Classic

		cloud_hit_effects('n', 't', 'c');
	}
	else {

		cloud_hit_effects('e', 'f', 'f');
	}
}

HOOK(void, __fastcall, _StartSong, sigStartSong(), int* a1, __int64 a2, char a3) {
	
	original_StartSong(a1, a2, a3);
	difficulty = *a1;
	
	if (difficulty == 1) { //normal

		bar_difficulty('n', 'o', 'r', 'm');
	}
	else if (difficulty == 2) { //hard

		bar_difficulty('h', 'a', 'r', 'd');
	}
	else if (difficulty == 3) { //extreme or ex-extreme

		bar_difficulty('e', 'x', 't', 'r');
	}
	else { //easy or tutorial

		bar_difficulty('e', 'a', 's', 'y');
	}
}

extern "C" __declspec(dllexport) void Init() {
	
	printf("[X UI for MM+] Initializing...\n");
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
		}
		catch (std::exception& exception) {
			
			char text[1024];
			sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
			MessageBoxA(nullptr, text, "X UI for MM+", MB_OK | MB_ICONERROR);
		}
	}
	catch (std::exception& exception) {
		
		char text[1024];
		sprintf_s(text, "Failed to parse config.toml:\n%s", exception.what());
		MessageBoxA(nullptr, text, "X UI for MM+", MB_OK | MB_ICONERROR);
	}

	if (hitEffect == 6) {

		INSTALL_HOOK(_Pv);
	}

	if (modifiersFix) {

		INSTALL_HOOK(_Modifiers);
		WRITE_MEMORY(sigOptionInfoHiddenA(), uint8_t, 0x00);
		WRITE_MEMORY(sigOptionInfoHispeedA(), uint8_t, 0x00);
		WRITE_MEMORY(sigOptionInfoSuddenA(), uint8_t, 0x00);
		WRITE_MEMORY(sigOptionInfoBase(), uint8_t, 0x00);
	}

	if (xBarRank) {

		INSTALL_HOOK(_StartSong);
		WRITE_MEMORY(sigSongEnergyBorderExcellent(), uint8_t, 0x00);
		WRITE_MEMORY(sigSongEnergyBorderGreat(), uint8_t, 0x00);
		WRITE_MEMORY(sigSongEnergyEffReach(), uint8_t, 0x00);
	}

	if (enValue) {

		combo_fine_judgements('c', 'o', 'm', 'e', 'n');
		value_fine_judgements('v', 'a', 'l', 'e', 'n');
		
		WRITE_MEMORY((char*)sigValueTextSad() + 0x03, uint8_t, 'e', 'n');
		WRITE_MEMORY((char*)sigValueTextWrong01() + 0x03, uint8_t, 'e', 'n');
		WRITE_MEMORY((char*)sigValueTextWrong02() + 0x03, uint8_t, 'e', 'n');
		WRITE_MEMORY((char*)sigValueTextWrong03() + 0x03, uint8_t, 'e', 'n');
		WRITE_MEMORY((char*)sigValueTextWrong04() + 0x03, uint8_t, 'e', 'n');
	}

	if (judgementsValue) {

		combo_fine_judgements('a', 'l', 't');
		value_fine_judgements('a', 'l', 't');

		WRITE_MEMORY(sigValueTextCool01(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigValueTextCool03(), uint8_t, 'a', 'l', 't');

		WRITE_MEMORY(sigComboTempoCool001(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboTempoCool010(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboTempoCool100(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboTempoCool100B(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboTempoCool1000(), uint8_t, 'a', 'l', 't');

		WRITE_MEMORY(sigComboCool001(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboCool010(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboCool100(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboCool100B(), uint8_t, 'a', 'l', 't');
		WRITE_MEMORY(sigComboCool1000(), uint8_t, 'a', 'l', 't');
	}

	if (noSlides) {

		hit_effects_0x_x();
	}

	if (percentage != 0) {
		
		switch (percentage) {

			case 1: //Bigger
				percentage = 'b';
				break;

			case 2: //None
				percentage = ' ';
				break;

			default: //Default
				percentage = 'p';
				break;
		}

		WRITE_MEMORY(sigPEnergyNum01C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum02C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum03C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum04C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum05C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum06C(), uint8_t, percentage);
		WRITE_MEMORY(sigPEnergyNum07C(), uint8_t, percentage);
	}

	if (hitEffect>0 && hitEffect<6) {

		switch (hitEffect) {
		
			case 1: //Cute
				cloud_hit_effects('c', 'c', 'c');
				break;

			case 2: //Cool
				cloud_hit_effects('c', 'r', 'c');
				break;

			case 3: //Elegant
				cloud_hit_effects('s', 'b', 'c');
				break;

			case 4: //Classic
				cloud_hit_effects('n', 't', 'c');
				break;

			case 5: //Quirky
				cloud_hit_effects('u', 'c', 'c');
				break;
			
			default:
				break;
		}
	}

	if (altFailed) {

		WRITE_MEMORY(sigFailedInfo(), uint8_t, 'a', 'l', 't', 'e', 'r', 'n');
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}