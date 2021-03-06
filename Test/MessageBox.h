#pragma once
#include <Windows.h>
#include "shared.h"

#include <nana/gui.hpp>
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>

#include <iostream>

// MessageBoxW function difinition
typedef int(WINAPI *_MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);
// Pointer for calling original MessageBoxW.
_MessageBoxW fpMessageBoxW;

/* Will be called upon user clicking on an element in the list view which corresponds to MessageBoxW
 * A pointer to the `hookInfo` structure will be passed containing data populated during the call to the hooked function
 * Expected to display aditional information about call to the hooked function (ex: paramets, addresses, etc.)
*/
void detailMessageBoxW(const hookInfo *hi) {
	using namespace nana;
	wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;

	// Extract variables
	string hWnd = "HWND (as int): " + to_string((int)hi->invisible_data[0]);
	string text = "Text: \"" + conv.to_bytes(wstring((LPCWSTR)hi->invisible_data[1])) + "\"";
	string caption = "Caption: \"" + conv.to_bytes(wstring((LPCWSTR)hi->invisible_data[2])) + "\"";
	string uType = "Type (as int): " + to_string((int)hi->invisible_data[3]);

	// Create a new form
	auto& fm = form_loader<form>()();
	fm.caption(hi->procedure + " details");
	// Create labels
	label lWindow{ fm, hWnd };
	label lText{ fm, text };
	label lCaption{ fm, caption };
	label lType{ fm, uType };
	lWindow.bgcolor(nana::colors::white);
	lText.bgcolor(nana::colors::white);
	lCaption.bgcolor(nana::colors::white);
	lType.bgcolor(nana::colors::white);
	// Place labels on the window
	fm.div("<vertical abc>");
	fm["abc"] << lWindow << lText << lCaption << lType;
	fm.collocate();

	// Display window
	fm.show();
	API::modal_window(fm);
}

int HookMessageBoxW(HWND hWnd, LPCWSTR text, LPCWSTR caption, UINT uType) {
	VariableManager::Instance()->mtx.lock();

	// Preprocess parameters
	wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
	string sText = conv.to_bytes(wstring(text));
	string sCaption = conv.to_bytes(wstring(caption));

	hookInfo hf;
	// Add information to be displayed in list view
	hf.procedure = "MessageBoxW";
	hf.parameters = "text: \"" + sText + "\", caption: \"" + sCaption + "\"";
	// Save all parameters passed to the function
	hf.invisible_data.push_back((void *)hWnd);
	hf.invisible_data.push_back((void *)text);
	hf.invisible_data.push_back((void *)caption);
	hf.invisible_data.push_back((void *)uType);
	// Append data to the list view
	VariableManager::Instance()->cat.append(hf, true);

	// Call the original function, modify parameters if needed
	int ret = fpMessageBoxW(hWnd, text, caption, uType);
	VariableManager::Instance()->mtx.unlock();

	return ret;
}

//TODO: implement MessageBoxA