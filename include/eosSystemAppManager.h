#ifndef __EOS_SYSTEM_APP_MANAGER_H__
#define __EOS_SYSTEM_APP_MANAGER_H__

#include <axLib/axLib.h>
#include <axLib/axEventManager.h>

#include "eosAppLoader.h"
#include "eosFrame.h"

namespace eos {
namespace sys {
	class AppManager : public ax::Event::Object {
	public:
	
		enum ApplicationEvents
		{
			FRAME_FULL_SCREEN,
			UN_FULLSCREEN_FRAME
		};
	
		AppManager(std::shared_ptr<ax::Event::Manager> evt_manager);
		
		void LaunchApplication(const std::string& app_name);
		
		void CloseFullScreenApp(const std::string& app_name);
		
		axEVENT_ACCESSOR(ax::Event::SimpleMsg<std::shared_ptr<ax::Window::Backbone>>, OnServerFrameCreation);
		
	private:
		std::map<std::string, eos::AppLoader> _appLoaders;
		
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowMinimize);
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowClose);
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowFullScreen);
		
		void OnWindowMinimize(const eos::Frame::Msg& msg);
		void OnWindowClose(const eos::Frame::Msg& msg);
		void OnWindowFullScreen(const eos::Frame::Msg& msg);
		
		void OnServerFrameCreation(const ax::Event::SimpleMsg<std::shared_ptr<ax::Window::Backbone>>& msg);
	};
}
}

#endif //__EOS_SYSTEM_APP_MANAGER_H__
