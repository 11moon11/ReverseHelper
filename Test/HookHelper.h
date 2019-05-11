#pragma once
//#include <tlhelp32.h>
//#include <Psapi.h>

#include "hookedFunctions.h"
#include "shared.h"

class HookHelper {
private:
	void error(const char* text) {
		MessageBox(NULL, text, "Hook Mudule", MB_OK);
	}

public:
	HookHelper() {
		if (MH_Initialize() != MH_OK) {
			error("Failed to initialize MinHook library");
		}

		registerHooks();
		MessageBox(NULL, "Startig function hooking...", "", MB_OK);

		VariableManager::Instance()->hooks["MessageBoxW"]->enable();

		MessageBoxW(NULL, L"All functions are hooked", L"", MB_OK);
	}

	~HookHelper() {
		MH_Uninitialize();
	}
};