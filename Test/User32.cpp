#include "User32.h"

#include "hookedFunctions.h"
#include "shared.h"

#include "MessageBox.h"

void registerUser32Hooks() {
	VariableManager::Instance()->hooks["MessageBoxW"] = new FunctionHook(&MessageBoxW, &HookMessageBoxW, &fpMessageBoxW, detailMessageBoxW);
}
