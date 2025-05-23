#include <Windows.h>
#include <iostream>
#include <random>
#include <set>
#include <filesystem>
#include "detours.h"
#include "Helpers.h"
#include "toml.hpp"
#include "xui.h"


typedef int i32;
typedef i32(__cdecl* IsFutureToneProc)();
auto IsFutureToneAddress = reinterpret_cast<IsFutureToneProc>(sigIsFutureTone());

typedef void* (__cdecl* SongInfoFunc_t)();
auto getSongInfo = reinterpret_cast<SongInfoFunc_t>(sigSongInfo());

uintptr_t sigRomBaseAddr = reinterpret_cast<uintptr_t>((char*)sigGetRomDirAddr() - 0x07);
uintptr_t sigRomOffset = *reinterpret_cast<uint32_t*>((sigRomBaseAddr)+3);
uintptr_t romDirsAddr = sigRomBaseAddr + sigRomOffset + 7;
std::vector<std::string>* romDirs = (std::vector<std::string>*)romDirsAddr; //0x1414AB8A0

uint32_t frameValueBtm;
uint32_t chanceValueBtm;
uintptr_t FrameChanceOffset = 0x06;
uintptr_t FramePriorityOffset = 0x0C;

toml::v3::node_view<toml::v3::node> skinsList;
std::vector<int> skinsIndices;

const int sknMax = 58;
const int idMax = 65535;
bool isParsed = false;

std::vector<std::string> hitEffectsVec = { "Default", "Classic", "Cute", "Cool", "Elegant", "Quirky" };
std::set<std::string> hitEffectsKeys(hitEffectsVec.begin(), hitEffectsVec.end());
std::vector<toml::table> parsedSkinsTables;
std::map<int, std::set<int>> parsedSkins;

std::vector<toml::table> parsedHitEffectsTables;
std::map<std::string, std::set<int>> parsedHitEffects;

__int8 modifiers;
__int8 difficulty;
__int8 hitEffect;
__int8 skins;
__int8 skinMode;
uint8_t percentage;
unsigned short int pv;

bool autoHit;
bool autoSkn;

bool ArcMixModeOpt[3];
bool ArcOpt[4];

bool noSlides;
bool xBarRank;
bool enValue;
bool judgementsValue;
bool altFailed;
bool coloredTarget;
bool smallerTarget;

int currentSkin = 0;

toml::table config;

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

void hit_effects_0x_x() {

    const int offset = 0x09;
    WriteInMemory(HitEffLArrays, offset, { 0x00, 0x00 });
    WriteInMemory(HitEffRArrays, offset, { 0x00, 0x00 });
}

void cloud_hit_effects(const std::string& hitEffect) {

    const int offset = 0x04;
    std::map<std::string, std::tuple<unsigned char, unsigned char, unsigned char>> hitEffectMap = {
        {hitEffectsVec[0], {'e', 'f', 'f'}},
        {hitEffectsVec[1], {'n', 't', 'c'}},
        {hitEffectsVec[2], {'c', 'c', 'c'}},
        {hitEffectsVec[3], {'c', 'r', 'c'}},
        {hitEffectsVec[4], {'s', 'b', 'c'}},
        {hitEffectsVec[5], {'u', 'c', 'c'}}
    };

    auto [v1, v2, v3] = hitEffectMap.at(hitEffect);
    WriteInMemory(HitEffArrays, offset, { v1, v2, v3 });
    WriteInMemory(HitEffLArrays, offset, { v1, v2, v3 });
    WriteInMemory(HitEffRArrays, offset, { v1, v2, v3 });
}

void bar_difficulty(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4) {

    WriteInMemory(songEnergyArrays, 0, { v1, v2, v3, v4 });
}

void judgmentsVals(unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4 = ' ', unsigned char v5 = ' ', bool isCombo = true) {

    if (v4 == v5 && v4 == ' ') {
        if (enValue) {
            v4 = 'e';
            v5 = 'n';
        }
        else if (isCombo && judgementsValue) {
            v4 = 'b';
            v5 = 'o';
        }
        else {
            v4 = 'u';
            v5 = 'e';
        }
    }

    if (isCombo) {
        WriteInMemory(comboFineArrays, 0, { v1, v2, v3, v4, v5 });
    }
    else {
        WriteInMemory(valueFineArrays, 0, { v1, v2, v3, v4, v5 });
    }
}

void setColoredTarget() {

    WriteInMemory(targetArrays, 0x04, { 'c' });
}

void setSmallerTarget() {

    const int offset = 0x05;
    WriteInMemory(buttonArrays, offset, { 's' });
    WriteInMemory(targetArrays, offset, { 's' });
}

void fixFrameSkinSlide(__int8 skn = -1) {

    chanceValueBtm = 1126728832;  // X Chance Bottom
    frameValueBtm = 1126568448;   // X Default Bottom
    switch (skn) {
    case 5: case 8: case 9: case 10: case 14: case 15: case 16: case 23:
    case 24: case 32: case 33: case 34: case 53: case 54: case 55:
        chanceValueBtm += 2000000;
        frameValueBtm += 2000000;
        break;
    case 7: case 44:
        chanceValueBtm += 3000000;
        frameValueBtm += 3000000;
        break;
    case 6: case 18: case 36: case 45: case 52:
        chanceValueBtm += 4000000;
        frameValueBtm += 4000000;
        break;
    case 19: case 22: case 26: case 31: case 50: case 51:
        chanceValueBtm += 5000000;
        frameValueBtm += 5000000;
        break;
    case 11: case 12: case 13: case 17: case 20: case 21: case 25:
    case 27: case 28: case 29: case 30: case 35: case 37: case 38:
    case 39: case 40: case 41: case 42: case 43: case 46: case 47:
    case 48: case 49: case 56: case 57: case 58:
        chanceValueBtm += 6000000;
        frameValueBtm += 6000000;
        break;
    }

    // Frame Top
    // 0x140274E23
    // 0x140274E5F
    // 0x140274E7C

    // Chance Bottom
    WRITE_MEMORY((char*)sigSlideFrame() - 0x28 + FrameChanceOffset, uint32_t, chanceValueBtm); //0x140274E41
    WRITE_MEMORY((char*)sigSlideFrame() - 0x14 + FrameChanceOffset, uint32_t, chanceValueBtm); //0x140274E55

    // Frame Bottom
    WRITE_MEMORY((char*)sigSlideFrame() - 0x3C + FrameChanceOffset, uint32_t, frameValueBtm);  //0x140274E2D
    WRITE_MEMORY((char*)sigSlideFrame() - 0x1E + FrameChanceOffset, uint32_t, frameValueBtm);  //0x140274E4B
    WRITE_MEMORY((char*)sigSlideFrame() + 0x27 + FrameChanceOffset, uint32_t, frameValueBtm);  //0x140274E90
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
                if (index >= 0 && index <= sknMax) {
                    uniqueIndices.insert(index);
                }
                else {
                    std::cerr << "[X UI for MM+] Warning: Invalid skins index " << index << ". Valid indices are 0-57.\n" << std::endl;
                }
            }
        }
    }

    if (uniqueIndices.empty()) {
        for (int i = 0; i <= sknMax; i++) {
            uniqueIndices.insert(i);
        }
    }

    skinsIndices.assign(uniqueIndices.begin(), uniqueIndices.end());

    if (skinMode == 1) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(skinsIndices.begin(), skinsIndices.end(), g);
    }
}

const toml::array processArray(const toml::array& inputArray) {
    std::set<int> uniqueValues;
    toml::array uniqueValuesArray;

    for (const auto& value : inputArray) {
        bool valid = false;
        int int_value = 0;

        if (value.is_integer()) {
            int_value = value.as_integer()->get();
            valid = (int_value >= 1 && int_value <= idMax);
        }

        if (valid) {
            if (uniqueValues.insert(int_value).second) {
                uniqueValuesArray.push_back(int_value);
            }
        }
    }

    return uniqueValuesArray;
}

template <typename U, typename I>
void parseTOMLContent(const std::vector<toml::table>& tables, const std::set<std::string>& validKeys, std::map<I, std::set<int>>& parsedItems, const std::string& prefix, int maxIndex = -1) {
    for (const auto& toml_content : tables) {
        for (const auto& [key, value] : toml_content) {
            std::string key_str = std::string(key.str());

            if (!validKeys.empty() && validKeys.find(key_str) == validKeys.end()) {
                std::cerr << "[X UI for MM+] Warning: Invalid key: " << key_str << "\n";
                continue;
            }

            if (maxIndex >= 0 && (key_str.size() < 4 || key_str.substr(0, 3) != prefix || std::stoi(key_str.substr(3)) > maxIndex)) {
                std::cerr << "[X UI for MM+] Warning: Identifier out of allowed range or format: " << key_str << "\n";
                continue;
            }

            try {
                const toml::array* values = value.as_array();
                if (values) {
                    toml::array processedValues = processArray(*values);
                    for (const auto& element : processedValues) {
                        int val = element.value_or<int>(0);
                        if constexpr (std::is_same_v<I, int>) {
                            parsedItems[std::stoi(key_str.substr(3))].insert(val);
                        }
                        else {
                            parsedItems[key_str].insert(val);
                        }
                    }
                }
                else {
                    std::cerr << "[X UI for MM+] Warning: Value is not an array for key: " << key_str << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "[X UI for MM+] Error: Exception while parsing key/value pair: " << key_str << ", " << e.what() << "\n";
            }
        }
    }
}

template <typename M>
void parseFile(const std::string& dir, const std::string& fileName, std::vector<M>& parsedTables) {
    std::string filePath = dir + "/" + fileName;

    if (std::filesystem::exists(filePath)) {
        try {
            auto toml_content = toml::parse_file(filePath);
            parsedTables.push_back(toml_content);
            std::cerr << "[X UI for MM+] Parsed " << filePath << std::endl;
        }
        catch (const toml::parse_error& err) {
            std::cerr << "[X UI for MM+] Failed to parse " << filePath << ": " << err << std::endl;
        }
    }
}

HOOK(char, __fastcall, _PvDbRead, sigPvDbRead(), uint64_t task) {
    auto result = original_PvDbRead(task);

    if (isParsed || !romDirs) {
        return result;
    }

    if (*(int32_t*)(task + 0x68) == 0 && !isParsed) {
        parsedSkinsTables.clear();
        parsedHitEffectsTables.clear();

        for (const auto& dir : *romDirs) {
            if (dir.empty() || dir.find("mods") != 0) continue;

            if (autoSkn) {
                parseFile(dir, "skins.toml", parsedSkinsTables);
            }

            if (autoHit) {
                parseFile(dir, "hitEffects.toml", parsedHitEffectsTables);
            }
        }

        if (autoSkn) parseTOMLContent<int, int>(parsedSkinsTables, {}, parsedSkins, "skn", sknMax);
        if (autoHit) parseTOMLContent<std::string, std::string>(parsedHitEffectsTables, hitEffectsKeys, parsedHitEffects, "");

        isParsed = true;
    }

    return result;
}

template <typename X>
void handleMode(int pvId, const std::map<X, std::set<int>>& parsedItems, const std::string& modeName) {
    std::vector<X> possibleItems;

    for (const auto& [index, values] : parsedItems) {
        if (values.count(pvId) > 0) {
            possibleItems.push_back(index);
        }
    }

    if (!possibleItems.empty()) {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, possibleItems.size() - 1);
        X selectedItem = possibleItems[dis(gen)];

        if constexpr (std::is_same_v<X, std::string>) cloud_hit_effects(selectedItem);
        else setSkin(static_cast<__int8>(selectedItem));
    }
    else {
        if (modeName == "Hit") cloud_hit_effects(hitEffectsVec[0]);
        else setSkin(0);
    }
}

HOOK(char, __fastcall, _PvId, sigPvId(), __int64* a1, __int64 a2, __int64 a3) {
    char result = original_PvId(a1, a2, a3);

    __int64* address = (__int64*)(*a1 + 185192);
    int pvId = *reinterpret_cast<int*>(*address);

    if (autoHit) handleMode<std::string>(pvId, parsedHitEffects, "Hit");
    if (autoSkn) handleMode<int>(pvId, parsedSkins, "Skn");

    return result;
}

HOOK(void, __fastcall, _StartSong, sigStartSong(), int* a1, __int64 a2, char a3) {

    uintptr_t songInfo = reinterpret_cast<uintptr_t>(getSongInfo());
    uintptr_t pvFlag = songInfo + 0x19;
    bool isPv = *reinterpret_cast<bool*>(pvFlag);

    original_StartSong(a1, a2, a3);

    if (!isPv) {

        if (xBarRank) {
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

void patchLayersPriority(char* priorityPtr) {

    uint8_t* currentPriorityPtr = reinterpret_cast<uint8_t*>(priorityPtr + 0x03);
    uint8_t currentPriority = *currentPriorityPtr;
    WRITE_MEMORY(currentPriorityPtr, uint8_t, currentPriority - 7);
}

void fixModifiersAnim(char* modifiersPtr) {

    uint8_t* currentPriorityPtr = reinterpret_cast<uint8_t*>(modifiersPtr + 0x03);
    WRITE_MEMORY(currentPriorityPtr, uint8_t, 0xE7);
}

void handleParseException(const std::exception& exception) {
    std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
    MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "X UI for MM+", MB_OK | MB_ICONERROR);
}

void checkOptionValues() {

    auto arcMixModeOptArray = config["Arcade_MixMode_Options"].as_array();
    auto arcOptArray = config["Arcade_Options"].as_array();

    if (arcMixModeOptArray && arcMixModeOptArray->size() >= 3) {
        enValue = (*arcMixModeOptArray)[0].value_or(false);
        altFailed = (*arcMixModeOptArray)[1].value_or(false);
        xBarRank = (*arcMixModeOptArray)[2].value_or(false);
    }
    else {
        enValue = false;
        altFailed = false;
        xBarRank = false;
    }

    if (arcOptArray && arcOptArray->size() >= 4) {
        judgementsValue = (*arcOptArray)[0].value_or(false);
        coloredTarget = (*arcOptArray)[1].value_or(false);
        smallerTarget = (*arcOptArray)[2].value_or(false);
        noSlides = (*arcOptArray)[3].value_or(false);
    }
    else {
        judgementsValue = false;
        coloredTarget = false;
        smallerTarget = false;
        noSlides = false;
    }
}

extern "C" __declspec(dllexport) void Init() {

    std::cerr << "[X UI for MM+] Initializing..." << std::endl;

    try {
        config = toml::parse_file("config.toml");
        try {
            checkOptionValues();

            percentage = config["Percentage"].value_or(0);
            hitEffect = config["Hit_Effect"].value_or(0);

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

    if (xBarRank) WriteInMemory(songEnergyBArrays, 0, { 'b' });

    if (noSlides) hit_effects_0x_x();

    if (enValue) {
        judgmentsVals('c', 'o', 'm', 'e', 'n', true);
        judgmentsVals('v', 'a', 'l', 'e', 'n', false);
        WriteInMemory(valueTextArrays, 0x03, { 'e', 'n' });
    }

    if (judgementsValue) {
        judgmentsVals('a', 'l', 't', ' ', ' ', true);
        judgmentsVals('a', 'l', 't', ' ', ' ', false);
        WriteInMemory(coolArrays, 0, { 'a', 'l', 't' });
    }

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

    switch (hitEffect) {
    case 1: case 2: case 3: case 4: case 5:
        cloud_hit_effects(hitEffectsVec[hitEffect]);
        break;
    case 6:
        autoHit = true;
        break;
    }

    switch (skinMode) {
    case 0: case 1:
        generateSkinsArray();
        break;
    case 2:
        autoSkn = true;
        break;
    default:
        fixFrameSkinSlide();
        break;
    }

    if (autoHit || autoSkn) {
        INSTALL_HOOK(_PvDbRead);
        INSTALL_HOOK(_PvId);
    }
    INSTALL_HOOK(_StartSong);
    //INSTALL_HOOK(_loadSprGamCmnLayers);

    patchLayersPriority((char*)sigFrame()); // frame_bottom 0x140278087
    patchLayersPriority((char*)sigFrame() + 0x151); // frame_top 0x1402781D8
    // chance_frame_bottom 0x151300C6E
    // chance_frame_top 0x151300CBF

    fixModifiersAnim((char*)sigModifiersAnimation()); // base 0x140279324
    fixModifiersAnim((char*)sigModifiersAnimation() + 0x11); // hi-speed 0x0x140279335
    fixModifiersAnim((char*)sigModifiersAnimation() + 0x22); // hidden 0x140279346
    fixModifiersAnim((char*)sigModifiersAnimation() + 0x33); // sudden 0x140279357

    if (altFailed) WRITE_MEMORY(sigFailedInfo(), uint8_t, 'a', 'l', 't', 'e', 'r', 'n');

    if (coloredTarget) setColoredTarget();
    if (smallerTarget) setSmallerTarget();
}

//Use of PostInit() to check that the FT UI is used beforehand
extern "C" __declspec(dllexport) void PostInit() {

    FT_Compatibility();
}