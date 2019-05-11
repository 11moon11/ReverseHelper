#pragma once
#include <mutex>
#include <map>

#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>

struct hookInfo {
	string procedure;
	string parameters;

	std::vector<void*> invisible_data;
};
nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const hookInfo& pr);

class FunctionHook;

class VariableManager {
public:
	std::map<std::string, FunctionHook*> hooks;
	nana::drawerbase::listbox::cat_proxy cat;
	std::mutex mtx;

	static VariableManager* Instance();

private:
	static VariableManager* instance;
	VariableManager() {};
};