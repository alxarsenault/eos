#include "eosAppLoader.h"
#include <dlfcn.h>


eos::AppLoader::Module::Module(const std::string& path)
{
	_binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);

	if(_binHandle != nullptr)
	{
		_fctHandle = reinterpret_cast<axWindow*(*)(axWindow*, ax::Rect)>
			(dlsym(_binHandle, "StartApplication"));
	}
	else
	{
		ax::Error("Failed to load library :", path);
	}
}

eos::AppLoader::Module::~Module()
{
	dlclose(_binHandle);    
}

eos::AppLoader::Module::EntryFunction eos::AppLoader::Module::GetFunctionHandle()
{
	return _fctHandle;
}



eos::AppLoader::AppLoader():
_path(""),
_module(nullptr),
_handle(nullptr)
{

}

eos::AppLoader::AppLoader(const std::string& path):
_path(path),
_module(nullptr),
_handle(nullptr)
{

}

axWindow* eos::AppLoader::Create(axWindow* parent, const ax::Rect& rect)
{
	if(_module == nullptr)
	{
		_module = new Module(_path);
	}
		
	Module::EntryFunction fct = _module->GetFunctionHandle();
	
	if(fct)
	{
		return _handle = fct(parent, rect);
	}
			
	return nullptr;
}

void eos::AppLoader::RemoveHandle()
{
	_handle = nullptr;
}

axWindow* eos::AppLoader::GetHandle()
{
	return _handle;
}
