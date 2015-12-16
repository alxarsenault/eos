#ifndef __EOS_SYSTEM_APP_MANAGER_H__
#define __EOS_SYSTEM_APP_MANAGER_H__

#include <axLib/axLib.h>
#include "eosAppLoader.h"
#include "eosFrame.h"

namespace eos {
namespace sys {
	class AppManager {
	public:
	
		AppManager();
		
		void LaunchApplication(const std::string& app_name);
		
	private:
		std::map<std::string, eos::AppLoader> _appLoaders;
		
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowMinimize);
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowClose);
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowFullScreen);
		
		void OnWindowMinimize(const eos::Frame::Msg& msg);
		void OnWindowClose(const eos::Frame::Msg& msg);
		void OnWindowFullScreen(const eos::Frame::Msg& msg);
	};
}
}

#endif //__EOS_SYSTEM_APP_MANAGER_H__
