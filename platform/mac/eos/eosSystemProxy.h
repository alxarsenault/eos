#ifndef __EOS_SYSTEM_PROXY_H_
#define __EOS_SYSTEM_PROXY_H_

#include "eosSystemCore.h"

namespace eos {
namespace sys {
	class proxy {
	public:
		static std::shared_ptr<eos::Desktop> GetDesktop();
		
		static void AddFrame(std::shared_ptr<ax::Window::Backbone> frame);
		
		static void BringToFront(ax::Window::Ptr frame);
		
		static void FullScreenFrame(ax::Window::Ptr frame);
		
		static void LaunchApplication(const std::string& app_name);
		
		static void RemoveFrame(std::shared_ptr<ax::Window::Backbone> frame);
	};
}
}

#endif // __EOS_SYSTEM_PROXY_H_
