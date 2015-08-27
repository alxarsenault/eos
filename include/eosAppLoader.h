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
			
			~Module();

			EntryFunction GetFunctionHandle();

		private:
			void* _binHandle;
			EntryFunction _fctHandle;
		};

		AppLoader();

		AppLoader(const std::string& path);

		axWindow* Create(axWindow* parent, const ax::Rect& rect);

		void RemoveHandle();

		axWindow* GetHandle();

	private:
		std::string _path;
		Module* _module;
		axWindow* _handle;
	};
}

#endif // __EOS_APP_LOADER_H__
