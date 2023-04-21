#include <Windows.h>
#include <string>

#include "Helpers.h"
#include "SigScan.h"

SIG_SCAN(sigLoadingRandom, 0x1406542B1, "\x41\x8D\x50\x04\x33\xC9\xE8\x00\x00\x00\x00\x8B\xD8\x48\x8D\x15\x00\x00\x00\x00", "xxxxxxx????xxxxx????")

extern "C" __declspec(dllexport) void Init() {

	WRITE_MEMORY((char*)sigLoadingRandom() + 0x03, uint8_t, 0x05); //6
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {

	return TRUE;
}