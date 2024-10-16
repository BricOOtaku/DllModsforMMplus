#include <Windows.h>
#include <iostream>
#include <sstream>
#pragma comment(lib, "winmm.lib")

#include "detours.h"
#include "Helpers.h"
#include "toml.hpp"
#include "AdvVoice.h"

int setAdvVoice;
bool isStopped=true;
std::wstring currentSoundFile;
std::wstring advVoice[] = { L"MM_Plus", L"FT" };

toml::table config;

std::wstring GetDLLPath() {

    wchar_t path[MAX_PATH];
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)GetDLLPath, &hModule);
    GetModuleFileName(hModule, path, MAX_PATH);
    std::wstring::size_type pos = std::wstring(path).find_last_of(L"\\/");
    return std::wstring(path).substr(0, pos);
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

void PlaySoundFile(const wchar_t* relativePath) {

    std::wstring fullPath = GetDLLPath() + L"\\" + relativePath;
    currentSoundFile = fullPath;
    PlaySound(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void StopSound() {
	
    isStopped = true;
    PlaySound(NULL, 0, 0);
}

HOOK(__int64, , _SeFtMainEnter, sigSeFtMainEnter()) {

    StopSound();
    return original_SeFtMainEnter();
}

//This will prevent the sound to continue after the SEGA logo if it excced more than 30 seconds
HOOK(__int64, __fastcall, _SegaLogo, sigSegaLogo(), __int64 a1) {
    
    if (!isStopped) {
        StopSound();
    }
    return original_SegaLogo(a1);
}

HOOK(char, __fastcall, _AdvBg, sigAdvBg(), DWORD* a1, __int64 a2, __int64 a3, __int64 a4) {
    
    isStopped = false;
    std::wstringstream ws;
    ws << L"rom/sound/adv/Vocal_" << advVoice[setAdvVoice] << L".wav";
    PlaySoundFile(ws.str().c_str());
    return original_AdvBg(a1, a2, a3, a4);
}

void handleParseException(const std::exception& exception) {

    std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
    MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Title Screen Voice Announcement", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {

    std::cerr << "[Title Screen Voice Announcement] Initializing..." << std::endl;
    try {
        config = toml::parse_file("config.toml");
        setAdvVoice = config["Set_Adv_Voice"].value_or(0);
        try {
        }
        catch (std::exception& exception) {
            handleParseException(exception);
        }
    }
    catch (std::exception& exception) {
        handleParseException(exception);
    }

    if (setAdvVoice > 1) setAdvVoice = 1;
    else if (setAdvVoice < -1) setAdvVoice = -1;

    if (setAdvVoice == -1 && isDllLoaded("ps4.dll")) setAdvVoice = 1;
    else if (setAdvVoice != 1) setAdvVoice = 0;

    INSTALL_HOOK(_AdvBg);
    INSTALL_HOOK(_SeFtMainEnter);
    INSTALL_HOOK(_SegaLogo);
}