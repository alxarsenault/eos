#include "eosSystemProxy.h"
#include "eosSystemAppManager.h"

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
	
		void proxy::RemoveFrame(std::shared_ptr<ax::Window::Backbone> frame)
		{
			Core::GetInstance()->RemoveFrame(frame);
		}
	
		void proxy::BringToFront(ax::Window::Ptr frame)
		{
			Core::GetInstance()->BringToFront(frame);
		}
	
		void proxy::FullScreenFrame(ax::Window::Ptr frame)
		{
			Core::GetInstance()->FullScreenFrame(frame);
		}
	
		void proxy::LaunchApplication(const std::string& app_name)
		{
			ax::Print("proxy::LaunchApplication");
			std::shared_ptr<AppManager> app_manager = Core::GetInstance()->GetAppManager();
			app_manager->LaunchApplication(app_name);
		}
	
		void proxy::ConnectToAppManager(const ax::Event::Id& id, ax::Event::Function fct)
		{
			std::shared_ptr<AppManager> app_manager = Core::GetInstance()->GetAppManager();
			app_manager->AddConnection(id, fct);
		}
	
		void proxy::UnFullScreenFrame(const std::string& app_name)
		{
			std::shared_ptr<AppManager> app_manager = Core::GetInstance()->GetAppManager();
			app_manager->CloseFullScreenApp(app_name);
		}
}
}
