#include <Windows.h>
#include <random>
#include <set>
#include <unordered_map>
#include <iostream>

#include "Helpers.h"
#include "SigScan.h"
#include "detours.h"
#include "toml.hpp"

#include "SigAdv.h"

toml::v3::node_view<toml::v3::node> openingList;

int currentOpening = 0;
bool useBonusAudio = false;
bool randomizeOpenings = false;

unsigned char v4;
unsigned char v5;

toml::table config;

std::vector<int> openingIndices;

struct OpeningParams {
    char v1;
    char v2;
    char v3;
    bool hasBonus;
    uint32_t sizeValue;
    uintptr_t mp4Sig;
    uintptr_t oggSig;
};

const uint32_t defaultsizeValue = 0x17;
const uintptr_t defaultMp4Sig = (uintptr_t)sigMega39sOpMp4();
const uintptr_t defaultOggSig = (uintptr_t)sigMega39sOpOgg();
const char defaultV1 = '3';
const char defaultV2 = '9';
const char defaultV3 = 's';

std::unordered_map<int, OpeningParams> openingMap;

void initOpeningMap() {

    openingMap = {
        {0, {'D', 'T', '1', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA
        {1, {' ', ' ', ' ', false, 0x1A, (uintptr_t)sigArcadeOpMp4(), (uintptr_t)sigArcadeOpOgg()}}, // Project DIVA Arcade
        {2, {'D', 'T', '2', useBonusAudio, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA 2nd
        {3, {'D', 'T', 'E', useBonusAudio, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA Extended
        {4, {'P', 'M', '1', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project Mirai
        {5, {' ', ' ', ' ', false, 0x1C, (uintptr_t)sigArcadeVersionBOpMp4(), (uintptr_t)sigArcadeVersionBOpOgg()}}, // Project DIVA Arcade Version B
        {6, {'P', 'J', 'F', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA F
        {7, {' ', ' ', ' ', false, 0x26, (uintptr_t)sigFutureTonePhotoStudioOpMp4(), (uintptr_t)sigFutureTonePhotoStudioOpOgg()}}, // Project DIVA Arcade Future Tone
        {8, {'P', 'M', '2', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project Mirai 2
        {9, {'D', 'F', '2', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA F2nd
        {10, {'P', 'M', 'R', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project Mirai DX
        {11, {' ', ' ', ' ', false, 0x1D, (uintptr_t)sigArcadeFutureToneMp4(), (uintptr_t)sigArcadeFutureToneOpOgg()}}, // Project DIVA Arcade Future Tone Version B
        {12, {'P', 'J', 'X', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}}, // Project DIVA X
        {13, {'3', '9', 's', false, defaultsizeValue, defaultMp4Sig, defaultOggSig}} // Project DIVA Megamix+
    };
}

void generateOpeningArray() {

    std::set<int> uniqueIndices;
    if (config["Opening_List"].is_array()) {
        for (const auto& value : *config["Opening_List"].as_array()) {
            if (value.is_integer()) {
                int index = static_cast<int>(value.as_integer()->get());
                if (index >= 0 && index <= 13) {
                    uniqueIndices.insert(index);
                }
                else {
                    std::cerr << "[Opening replacements+] Warning: Invalid opening index " << index << ". Valid indices are 0-13.\n" << std::endl;
                }
            }
        }
    }

    openingIndices.assign(uniqueIndices.begin(), uniqueIndices.end());

    if (openingIndices.empty()) {
        std::cerr << "[Opening replacements+] Warning : No opening was set. Using default values (0-13). Please check values in Opening_List in config.toml." << std::endl;
        for (int i = 0; i <= 13; i++) {
            openingIndices.push_back(i);
        }
    }

    if (randomizeOpenings) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(openingIndices.begin(), openingIndices.end(), g);
    }
}

void useAttractModeTheaterOpening(uint32_t sizeValue, uintptr_t mp4Sig, uintptr_t oggSig) {

    uintptr_t sizeMega39sOp = (uintptr_t)sigAdvBg() - 0x301; // mov r8d, sizeValue 
    uintptr_t addressMovie = sizeMega39sOp + 0x7; // lea rdx,[mp4Sig]
    uintptr_t addressOgg = sizeMega39sOp + 0x70; // lea rdx,[oggSig]

    WRITE_MEMORY((uint32_t*)(sizeMega39sOp + 0x2), uint32_t, 0x00000000 | sizeValue);
    WRITE_MEMORY((int32_t*)(addressMovie + 0x2), int32_t, (int32_t)(mp4Sig - addressMovie - 0x6));
    WRITE_MEMORY((int32_t*)(addressOgg + 0x3), int32_t, (int32_t)(oggSig - addressOgg - 0x7));
}

void changeOpening(unsigned char v1, unsigned char v2, unsigned char v3, bool hasBonus) {

    if (hasBonus) {
        v4 = '_';
        v5 = 'V';
    }
    else {
        v4 = 'O';
        v5 = 'P';
    }

    WRITE_MEMORY((char*)sigMega39sOpOgg() + 0x12, uint8_t, v1, v2, v3, v4, v5);
    WRITE_MEMORY((char*)sigMega39sOpMp4() + 0xe, uint8_t, v1, v2, v3);
}

void setOpening() {

    int opening = openingIndices[currentOpening % openingIndices.size()];
    auto params = openingMap[opening];

    if (params.v1 != ' ' && params.v2 != ' ' && params.v3 != ' ') {
        changeOpening(params.v1, params.v2, params.v3, params.hasBonus);
    }
    useAttractModeTheaterOpening(params.sizeValue, params.mp4Sig, params.oggSig);

    currentOpening++;
}

HOOK(char, __fastcall, _ADVBG, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {

    original_ADVBG(a1, a2, a3, a4);
    setOpening();
    return 1;
}

void handleParseException(const std::exception& exception) {

    std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
    MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Opening replacements+", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {

    std::cerr << "[Opening replacements+] Initializing..." << std::endl;
    try {

        config = toml::parse_file("config.toml");
        try {
            openingList = config["Opening_List"];
            useBonusAudio = config["Bonus_Audio"].value_or(false);
            randomizeOpenings = config["Randomize_Openings"].value_or(false);
        }
        catch (std::exception& exception) {
            handleParseException(exception);
        }
    }
    catch (std::exception& exception) {
        handleParseException(exception);
    }

    INSTALL_HOOK(_ADVBG);
    initOpeningMap();
    generateOpeningArray();
    setOpening();
}