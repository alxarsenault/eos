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
			typedef std::function<ax::Window*(ax::Window*, ax::Rect)> EntryFunction;

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

		ax::Window* Create(ax::Window* parent, const ax::Rect& rect);

		void RemoveHandle();

		ax::Window* GetHandle();

	private:
		std::string _path, _func_name;
		Module* _module;
		ax::Window* _handle;
	};
}

#endif // __EOS_APP_LOADER_H__
