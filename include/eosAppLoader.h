#ifndef __EOS_APP_LOADER_H__
#define __EOS_APP_LOADER_H__

#include "axLib/axLib.h"

namespace eos
{
	class AppLoader
	{
	public:
		class Module
		{
		public:
			typedef std::function<axWindow*(axWindow*, ax::Rect)> EntryFunction;

			Module(const std::string& path);
            
            Module(const std::string& path, const std::string& func_name);
			
			~Module();

			EntryFunction GetFunctionHandle();

		private:
			void* _binHandle;
			EntryFunction _fctHandle;
		};

		AppLoader();

		AppLoader(const std::string& path);
        
        AppLoader(const std::string& path, const std::string& func_name);

		axWindow* Create(axWindow* parent, const ax::Rect& rect);

		void RemoveHandle();

		axWindow* GetHandle();

	private:
		std::string _path, _func_name;
		Module* _module;
		axWindow* _handle;
	};
}

#endif // __EOS_APP_LOADER_H__
