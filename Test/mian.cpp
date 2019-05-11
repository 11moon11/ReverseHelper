#include <Windows.h>
#include <iostream>

#include "HookHelper.h"
#include "GuiHelper.h"

using namespace std;

HMODULE dllModule;
HANDLE mainThread;
HANDLE guiThread;
bool isRunning;

void WINAPI GUIThread() {
	GuiHelper gh(dllModule, "Reverse Helper");

	while (isRunning) {
		Sleep(5000);
	}
}

void WINAPI DllThread() {
	isRunning = true;

	// Create a console for debugging purposes
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);


	guiThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GUIThread, NULL, NULL, NULL);
	HookHelper *hk = new HookHelper();

	// Entering processing loop
	while (isRunning) {
		Sleep(5000);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	DisableThreadLibraryCalls(hModule);

	switch (reason) {
	case DLL_PROCESS_ATTACH:
		dllModule = hModule;
		mainThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DllThread, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		isRunning = false;
		WaitForSingleObject(mainThread, INFINITE);
		WaitForSingleObject(guiThread, INFINITE);
		break;
	}

	return true;
}