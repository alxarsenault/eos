#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "axLib/axLib.h"
#include <vector>

namespace chaiscript {
class ChaiScript;
class Module;
typedef std::shared_ptr<Module> ModulePtr;
}

class Script {
public:
	Script();

	void ProcessFile(const std::string& file_path);
	void ProcessString(const std::string& content);
	void AddWindowHandle(ax::Window::Ptr win);

private:
	std::shared_ptr<chaiscript::ChaiScript> _chai;
	static std::vector<chaiscript::ModulePtr> _modules;

	void CreateModules();
};

#endif // __SCRIPT_H__
