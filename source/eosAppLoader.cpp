#include "eosAppLoader.h"
#include <dlfcn.h>


eos::AppLoader::Module::Module(const std::string& path)
{
    _binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LAZY);//RTLD_LOCAL);

    
    
	if(_binHandle != nullptr)
	{
		_fctHandle = reinterpret_cast<ax::Window*(*)(ax::Window*, ax::Rect)>
			(dlsym(_binHandle, "StartApplication"));
	}
	else
	{
		ax::Error("Failed to load library :", path);
        ax::Error(dlerror());
	}
}

eos::AppLoader::Module::Module(const std::string& path,
                               const std::string& func_name)
{
    _binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LAZY);//RTLD_LOCAL);
    
    if(_binHandle != nullptr)
    {
        _fctHandle = reinterpret_cast<ax::Window*(*)(ax::Window*, ax::Rect)>
        (dlsym(_binHandle, func_name.c_str()));
    }
    else
    {
        ax::Error("Failed to load library :", path);
        ax::Error(dlerror());
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
_func_name(""),
_module(nullptr),
_handle(nullptr)
{

}

eos::AppLoader::AppLoader(const std::string& path):
_path(path),
_func_name(""),
_module(nullptr),
_handle(nullptr)
{

}

eos::AppLoader::AppLoader(const std::string& path,
                          const std::string& func_name):
_path(path),
_func_name(func_name),
_module(nullptr),
_handle(nullptr)
{
    
}

ax::Window* eos::AppLoader::Create(ax::Window* parent, const ax::Rect& rect)
{
	if(_module == nullptr)
	{
        if(_func_name.empty())
        {
            _module = new Module(_path);
        }
        else
        {
             _module = new Module(_path, _func_name);
        }
		
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

ax::Window* eos::AppLoader::GetHandle()
{
	return _handle;
}
