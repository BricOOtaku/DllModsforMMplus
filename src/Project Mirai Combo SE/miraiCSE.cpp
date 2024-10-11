#include <Windows.h>
#include <iostream>
#include <sstream>
#pragma comment(lib, "winmm.lib")

#include "detours.h"
#include "Helpers.h"
#include "toml.hpp"
#include "miraiCSE.h"

int combo;
int cmb;
int performer;
int setPerformer;
int currentPerformer;
bool isPerformerSet= false;

std::wstring currentSoundFile;
std::wstring performers[] = { L"mik", L"rin", L"len", L"luk", L"kai", L"mei" };

toml::table config;

std::wstring GetDLLPath() {
    
    wchar_t path[MAX_PATH];
    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)GetDLLPath, &hModule);
    GetModuleFileName(hModule, path, MAX_PATH);
    std::wstring::size_type pos = std::wstring(path).find_last_of(L"\\/");
    return std::wstring(path).substr(0, pos);
}

void PlaySoundFile(const wchar_t* relativePath) {
    
    std::wstring fullPath = GetDLLPath() + L"\\" + relativePath;
    currentSoundFile = fullPath;
    PlaySound(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

HOOK(__int64, __fastcall, _RestartSong, sigRestartSong(), __int64 a1, __int64 a2, __int64 a3) {

    if (!isPerformerSet) {

        performer = *reinterpret_cast<int*>(0x1412EF720); //Haven't found a value to sig for it

        switch (performer) {
        case 1:
            currentPerformer = 1; //Rin
            break;
        case 2:
            currentPerformer = 2; //Len
            break;
        case 3:
            currentPerformer = 3; //Luka
            break;
        case 6:
            currentPerformer = 4; //Kaito
            break;
        case 7:
        case 8:
            currentPerformer = 5; //Meiko/Sakine
            break;
        default:
            currentPerformer = 0; //Miku/Haku/Neru/Teto
            break;
        }
    }

    PlaySound(NULL, 0, 0);
    return original_RestartSong(a1, a2, a3);
}

HOOK(__int64, __fastcall, _EndSong, sigEndSong()) {
    
    PlaySound(NULL, 0, 0);
    return original_EndSong();
}

HOOK(__int64, __fastcall, _Pause, sigPause(), __int64 a1) {
    
    __int64 result = original_Pause(a1);
    if (a1) {
        PlaySound(NULL, 0, 0);
    }
    return result;
}

HOOK(void, __fastcall, _Combo, sigCombo(), __int64 a1, int a2, float a3, float a4, int a5) {
    
    original_Combo(a1, a2, a3, a4, a5);
    combo = a5;
    if (combo != 0 && combo % 100 == 0) {
        std::wstringstream ws;
        //Saving combo to another variable to keep the track of actual combo
        if (combo > 900) cmb = 900;
        else cmb = combo;
        ws << L"rom/sound/cmb/" << performers[currentPerformer] << L"cmb" << cmb << L".wav";
        PlaySoundFile(ws.str().c_str());
    }
    //std::cerr << "Combo : " << combo << "\n";
}

void handleParseException(const std::exception& exception) {
    
    std::cerr << "Failed to parse config.toml: " << exception.what() << std::endl;
    MessageBoxA(nullptr, ("Failed to parse config.toml:\n" + std::string(exception.what())).c_str(), "Project Mirai Combo SE", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) void Init() {
    
    std::cerr << "[Project Mirai Combo SE] Initializing..." << std::endl;
    try {
        config = toml::parse_file("config.toml");
        setPerformer = config["Set_Performer"].value_or(0);
        try {
        }
        catch (std::exception& exception) {
            handleParseException(exception);
        }
    }
    catch (std::exception& exception) {
        handleParseException(exception);
    }

    if (setPerformer > -1 && setPerformer < 6) {
        currentPerformer = setPerformer;
        isPerformerSet = true;
    }

    INSTALL_HOOK(_RestartSong);
    INSTALL_HOOK(_EndSong);
    INSTALL_HOOK(_Pause);
    INSTALL_HOOK(_Combo);
}