#pragma once
#include <Windows.h>
#include <iostream>

#include "shared.h"

#pragma comment(lib, "./Libs/nana_v142_Release_x86.lib")
#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>

using namespace nana;
using namespace std;

class GuiHelper {
private:
	const char *title;

public:
	GuiHelper(HINSTANCE hInstance, const char *title) {
		this->title = title;

		form fm(nana::API::make_center(500, 500));
		fm.caption(title);

		label lab{ fm, "Hello, <bold blue size=16>Nana C++ Library</>" };
		lab.format(true);

		button btn{ fm, "Quit" };
		btn.events().click([&fm] {
			fm.close();
		});

		listbox lsbox{ fm };
		lsbox.append_header("Procedure");
		lsbox.append_header("Parameters");

		lsbox.events().selected([](const arg_listbox & arg) {
			auto ptr = arg.item.value_ptr<hookInfo>();
			if (ptr != nullptr) {
				auto procedure = VariableManager::Instance()->hooks[ptr->procedure];
				if (procedure != NULL) {
					procedure->showDetailForm(ptr);
				}
			}
		});

		VariableManager::Instance()->cat = lsbox.at(0);

		//fm.div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
		//fm["text"] << lsbox;
		//fm["button"] << btn;
		//fm.collocate();
		
		place plc2(fm);
		plc2.div("<abc vert arrange=[90%,10%]>");
		plc2.field("abc") << lsbox << btn;
		plc2.collocate();

		fm.show();
		exec();
	}

	~GuiHelper() {
		delete[] title;
	}
};