#include "hookedFunctions.h"
#include "User32.h"
#include "shared.h"

FunctionHook::FunctionHook(LPVOID functionPointer, LPVOID hookFunction, LPVOID savePointer, LPVOID detailForm) {
	this->functionPointer = functionPointer;
	this->hookFunction = hookFunction;
	this->savePointer = (LPVOID*)(savePointer);
	MH_CreateHook(functionPointer, hookFunction, (LPVOID*)(savePointer));

	this->detailForm = detailForm;
}
bool FunctionHook::enable() {
	return MH_EnableHook(functionPointer) == MH_OK;
}
bool FunctionHook::disable() {
	return MH_DisableHook(functionPointer) == MH_OK;
}
void FunctionHook::showDetailForm(const hookInfo* hi) {
	void (*draw)(const hookInfo*) = (void(*)(const hookInfo*))detailForm;

	if(draw != NULL)
		draw(hi);
}


VariableManager* VariableManager::instance = NULL;
VariableManager* VariableManager::Instance() {
	if (!instance)
		instance = new VariableManager();

	return instance;
}

nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const hookInfo& pr) {
	return ores << pr.procedure << pr.parameters;
}

void registerHooks() {
	registerUser32Hooks();
}
