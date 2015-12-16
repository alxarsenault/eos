#include "eosSystemProxy.h"

namespace eos {
namespace sys {
		std::shared_ptr<eos::Desktop> proxy::GetDesktop()
		{
			return Core::GetInstance()->GetDesktop();
		}
		
		void proxy::AddFrame(std::shared_ptr<ax::Window::Backbone> frame)
		{
			Core::GetInstance()->AddFrame(frame);
		}
	
		void proxy::BringToFront(ax::Window::Ptr frame)
		{
			Core::GetInstance()->BringToFront(frame);
		}
	
		void proxy::FullScreenFrame(ax::Window::Ptr frame)
		{
			
		}
}
}
