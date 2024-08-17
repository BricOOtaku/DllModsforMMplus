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

toml::v3::node_view<toml::v3::node> vocaloid;
toml::v3::node_view<toml::v3::node> vocaloidAlt;
toml::v3::node_view<toml::v3::node> fanloid;
toml::v3::node_view<toml::v3::node> miraiDX;
toml::v3::node_view<toml::v3::node> others;

int currentVocals = 0;
bool randomizeVocals = false;

toml::table config;

std::vector<int> vocalsIndices;

int index = 0;

struct VocalParams {
    char v1;
    char v2;
    char v3;
    char v4;
};

std::unordered_map<int, VocalParams> vocalsMap;
std::unordered_map<std::string, VocalParams> othersMap;

std::vector<std::vector<int>> defaultVocalsSings;


void addVocalToMap(const std::string& name, char suffix, std::vector<int>& currentGroup) {
   
    vocalsMap[index] = { name[0], name[1], name[2], suffix };
    currentGroup.push_back(index++);
}


void addVocalsToMapAndGroup(const std::vector<std::string>& names, char suffix, std::vector<int>& currentGroup) {
    
    for (const auto& name : names) {
        addVocalToMap(name, suffix, currentGroup);
    }
    defaultVocalsSings.push_back(currentGroup);
    currentGroup.clear();
}


void addVocalsToMapManually(const std::string& name, char suffix, std::vector<int>& currentGroup) {
    
    addVocalToMap(name, suffix, currentGroup);
}

void initVocalsMap() {
    
    std::vector<std::string> suffixes = { "1", "2", "p", "r", "m", "y", "l", "f", "k" };
    std::vector<std::string> vocaloids = { "Mik", "Rin", "Len", "Luk", "Kai", "Mei" };
    std::vector<std::string> fanloids = { "Ner", "Hak", "Sak" };

    std::vector<int> currentGroup;

    addVocalsToMapAndGroup(vocaloids, suffixes[0][0], currentGroup); // Main
    addVocalsToMapAndGroup(vocaloids, suffixes[1][0], currentGroup); // Main alt
    addVocalsToMapAndGroup(fanloids, suffixes[2][0], currentGroup);  // Fanloid
    addVocalsToMapAndGroup(vocaloids, suffixes[3][0], currentGroup); // Mirai DX

    addVocalsToMapManually(vocaloids[0], suffixes[4][0], currentGroup); // Miku Mirai 1
    addVocalsToMapManually(vocaloids[0], suffixes[5][0], currentGroup); // Miku Youtube
    addVocalsToMapManually(vocaloids[1], suffixes[5][0], currentGroup); // Rin&Len Youtube
    addVocalsToMapManually(vocaloids[3], suffixes[5][0], currentGroup); // Luka Youtube
    addVocalsToMapManually(vocaloids[0], suffixes[6][0], currentGroup); // Miku Live stage producer
    addVocalsToMapManually(vocaloids[0], suffixes[7][0], currentGroup); // Miku Flick 1
    addVocalsToMapManually(vocaloids[0], suffixes[8][0], currentGroup); // Miku Flick 2

    defaultVocalsSings.push_back(currentGroup);
}

void buildConfigArray(toml::v3::node_view<toml::v3::node> configArray, int groupIndex, std::set<int>& uniqueIndices, const std::vector<std::pair<int, int>>& ranges) {
    
    if (configArray.is_array()) {
        const auto& array = *configArray.as_array();
        for (const auto& value : array) {
            if (value.is_integer()) {
                int index = static_cast<int>(value.as_integer()->get());
                if (index >= 0 && index < defaultVocalsSings[groupIndex].size()) {
                    uniqueIndices.insert(defaultVocalsSings[groupIndex][index]);
                }
                else {
                    std::cerr << "[Vocaloids sing SEGA +] Warning: Invalid index " << index << ". Valid indices are 0-" << defaultVocalsSings[groupIndex].size() - 1 << "." << std::endl;
                }
            }
        }
    }
}


void generateSingsArray() {
    
    std::vector<std::pair<int, int>> ranges;
    size_t startIndex = 0;
    for (const auto& group : defaultVocalsSings) {
        ranges.emplace_back(static_cast<int>(startIndex), static_cast<int>(startIndex + group.size() - 1));
        startIndex += group.size();
    }

    std::set<int> uniqueIndices;

    buildConfigArray(vocaloid, 0, uniqueIndices, ranges);
    buildConfigArray(vocaloidAlt, 1, uniqueIndices, ranges);
    buildConfigArray(fanloid, 2, uniqueIndices, ranges);
    buildConfigArray(miraiDX, 3, uniqueIndices, ranges);
    buildConfigArray(others, 4, uniqueIndices, ranges);

    if (uniqueIndices.empty()) {
        std::cerr << "[Vocaloids sing SEGA +] Warning: No valid vocals were set. Using default values (0-27). Please check the array values set on the config.toml" << std::endl;
        for (const auto& group : defaultVocalsSings) {
            uniqueIndices.insert(group.begin(), group.end());
        }
    }

    vocalsIndices.assign(uniqueIndices.begin(), uniqueIndices.end());

    if (randomizeVocals) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vocalsIndices.begin(), vocalsIndices.end(), g);
    }
}

void setVocal() {

    int vocals = vocalsIndices[currentVocals % vocalsIndices.size()];
    auto params = vocalsMap[vocals];

    WRITE_MEMORY((char*)sigSegaLogoOgg() + 0x24, uint8_t, static_cast<uint8_t>(params.v1), static_cast<uint8_t>(params.v2), static_cast<uint8_t>(params.v3), '_', static_cast<uint8_t>(params.v4));

    currentVocals++;
}


HOOK(char, __fastcall, _ADVBG, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {
    
    original_ADVBG(a1, a2, a3, a4);
    setVocal();
    return 1;
}

void handleParseException(const std::exception& exception) {

    std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
    MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Vocaloids sing SEGA +", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {
    
    std::cerr << "[Vocaloids sing SEGA +] Initializing... " << std::endl;
    try {
        config = toml::parse_file("config.toml");
        try {
            randomizeVocals = config["Randomize_Vocals"].value_or(false);
            vocaloid = config["Vocaloid"];
            vocaloidAlt = config["Vocaloid_Alt"];
            fanloid = config["Fanloid"];
            miraiDX = config["Mirai_DX"];
            others = config["Others"];
        }
        catch (std::exception& exception) {
            handleParseException(exception);
        }
    }
    catch (std::exception& exception) {
        handleParseException(exception);
    }

    INSTALL_HOOK(_ADVBG);
    initVocalsMap();
    generateSingsArray();
    setVocal();
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}