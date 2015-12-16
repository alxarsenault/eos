#ifndef __EOS_APP_MANAGER_H__
#define __EOS_APP_MANAGER_H__

#include <axLib/axLib.h>
//#include "eosAppLoader.h"
//#include "eosFrame.h"

namespace eos {

class Desktop;

namespace sys {

	class AppManager;

	class Core {
	public:
		static Core* Create();
		
		inline static Core* GetInstance()
		{
			return _instance;
		}
		
		int MainLoop();

		std::shared_ptr<eos::Desktop> GetDesktop();
		
		void AddFrame(std::shared_ptr<ax::Window::Backbone> frame);
		
		void RemoveFrame(std::shared_ptr<ax::Window::Backbone> frame);
		
		void BringToFront(ax::Window::Ptr frame);
		
		void FullScreenFrame(ax::Window::Ptr frame);
		
		std::shared_ptr<AppManager> GetAppManager();
		
	private:
		Core();
		
		static Core* _instance;
	
		std::shared_ptr<Desktop> _desktop;
		
		std::shared_ptr<AppManager> _appManager;
		
		void SetupMainApp(); /// Setup ax::App::AddMainEntry.
	};
}
}

#endif //__EOS_APP_MANAGER_H__
