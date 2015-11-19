#ifndef __EOS_APP_LOADER_H__
#define __EOS_APP_LOADER_H__

#include "axLib/axLib.h"

namespace eos {
	/*
	 * eos::AppLoader.
	 */
	class AppLoader
	{
	public:
		class Module
		{
		public:
			typedef std::function<ax::Window::Backbone*(ax::Rect)> EntryFunction;

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

		ax::Window::Backbone* Create(const ax::Rect& rect);

		void RemoveHandle();

		ax::Window::Backbone* GetHandle();

	private:
		std::string _path, _func_name;
		Module* _module;
		ax::Window::Backbone* _handle;
	};
}

#endif // __EOS_APP_LOADER_H__
