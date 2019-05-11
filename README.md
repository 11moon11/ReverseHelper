# ReverseHelper
It is a DLL which hooks WinAPI calls upon being injected into a process. When WinAPI functions are called Reverse Helper will display them in real time and parameters passed to them.

Compiled using Visual Studio 2019.
At the moment only works with x86 processes.
At the moment very few WinAPI calls are supported.

# How to use ReverseHelper
1. Compile project using Visual Studio
2. Open a target process in a debugger (ex: x64dbg)
3. Inject DLL (located in 'Release' folder) into a target process
4. Run target process in a debugger

# Used libraries
1. For function hooking MinHook library is used: https://github.com/TsudaKageyu/minhook
2. For GUI NANA library is used: https://github.com/cnjinhao/nana/

# How to add more calls:
1. Create a header file at: 'Header Files/HookedFunctions/\<Module\>/\<Function\>.h' (in 'Project Explorer' view in Visual Studio)
    - Function must be replaced with the name of the WinAPI call (ex: MessageBox)
    - \<Module\> should be replaced with the module function comes from (ex: User32)
2. Add function difinitions (Can be found on MSDN)
  MessageBoxW example:
  ```CPP
  #include <Windows.h>
  #include "shared.h"

  #include <nana/gui.hpp>
  #include <nana/gui/wvl.hpp>
  #include <nana/gui/widgets/listbox.hpp>
  #include <nana/gui/widgets/button.hpp>
  #include <nana/gui/widgets/label.hpp>
  
  // MessageBoxW function difinition
  typedef int(WINAPI *_MessageBoxW)(HWND, LPCWSTR, LPCWSTR, UINT);
  // Pointer for calling original MessageBoxW.
  _MessageBoxW fpMessageBoxW;
  ```
3. Implement hook function
  ```CPP
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
  ```
4. If needed you can implement a view, which will be displayed when user selects a WinAPI call from list view
  ```CPP
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
  ```
5. Finally you can register hook function in 'Source Files/modules/<module>.cpp'
  ```CPP
    void registerUser32Hooks() {
      VariableManager::Instance()->hooks["MessageBoxW"] = new FunctionHook(&MessageBoxW, &HookMessageBoxW, &fpMessageBoxW, detailMessageBoxW);
    }
  ```
  `detailMessageBoxW` can be replaced with NULL if detail window is not needed
