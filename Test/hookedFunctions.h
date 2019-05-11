#pragma once
#include <mutex>
#include <map>

#pragma comment(lib, "./Libs/libMinHook.x86.lib")
#include "./Includes/MinHook.h"

struct hookInfo;

using namespace std;

class FunctionHook {
private:
	LPVOID functionPointer;
	LPVOID hookFunction;
	LPVOID* savePointer;
	LPVOID detailForm;

public:
	FunctionHook(LPVOID functionPointer, LPVOID hookFunction, LPVOID savePointer, LPVOID detailForm);

	bool enable();
	bool disable();
	void showDetailForm(const hookInfo* hi);
};

void registerHooks();
