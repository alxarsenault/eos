#include "eosSystemAppManager.h"
#include "eosSystemProxy.h"

namespace eos {
namespace sys {
	AppManager::AppManager()
	{
		/// @todo Eventually parse app folder and find apps info dynamically.
		_appLoaders["calc"] = AppLoader("./app/calculator.so");
		_appLoaders["browser"] = AppLoader("./app/browser.so");
		_appLoaders["txtedit"] = AppLoader("./app/text_editor.so");
		_appLoaders["term"] = AppLoader("./app/terminal.so");
		_appLoaders["viewer"] = AppLoader("./app/video.so");
		_appLoaders["mail"] = AppLoader("./app/mail.so");
		_appLoaders["calender"] = AppLoader("./app/slideshow.so");
		_appLoaders["book"] = AppLoader("./app/editor.so");
		_appLoaders["settings"] = AppLoader("./app/mail.so");
		_appLoaders["trash"] = AppLoader("./app/mail.so");
	}

	void AppManager::LaunchApplication(const std::string& app_name)
	{
		ax::Print("AppManager::LaunchApplication :", app_name);

		std::map<std::string, eos::AppLoader>::iterator it = _appLoaders.find(app_name);

		// Application name does't exist in map.
		if (it == _appLoaders.end()) {
			ax::Error("AppManager::LaunchApplication -> app", app_name, "does't exist.");
			return;
		}

		AppLoader& app_loader = it->second;

		// Opening application for the first time.
		if (app_loader.GetHandle() == nullptr) {
			ax::Rect default_frame_rect(500, 50, 162 + 2 * 9, 255 + 25 + 9);
			std::shared_ptr<ax::Window::Backbone> frame(app_loader.Create(default_frame_rect));

			// Frame handle does't exist, problem while loading dynamic library.
			if (frame == nullptr) {
				ax::Error("AppManager::LaunchApplication :", app_name, "could not launch.");
				return;
			}

			// Frame has been created.
			frame->GetWindow()->AddConnection(eos::Frame::Events::MINIMIZE, GetOnWindowMinimize());
			frame->GetWindow()->AddConnection(eos::Frame::Events::FULL_SCREEN, GetOnWindowFullScreen());
			frame->GetWindow()->AddConnection(eos::Frame::Events::CLOSE, GetOnWindowClose());

			eos::sys::proxy::AddFrame(frame);

			return;
		}

		// Application is already running.
		ax::Window::Backbone* frame = app_loader.GetHandle();

		// Error while getting frame handle.
		if(frame == nullptr) {
			ax::Error("AppManager::LaunchApplication :", app_name, "could not get frame handle.");
			return;
		}
		
		// Bring application frame to front view.
		eos::sys::proxy::BringToFront(frame->GetWindow());
	}

	void AppManager::OnWindowMinimize(const eos::Frame::Msg& msg)
	{
	}

	void AppManager::OnWindowClose(const eos::Frame::Msg& msg)
	{
	}

	void AppManager::OnWindowFullScreen(const eos::Frame::Msg& msg)
	{
	}
} // namespace sys
} // namespace eos

// void eos::Dock::OnAppSelect(const ax::Event::StringMsg& msg)
//{
//	ax::Print("App select :", msg.GetMsg());
//	AppLoader& loader = _appLoaders[msg.GetMsg()];
//
//	if (loader.GetHandle() == nullptr) {
//		ax::Rect rect(500, 50, 162 + 2 * 9, 255 + 25 + 9);
//
//		std::shared_ptr<ax::Window::Backbone> frame(loader.Create(rect));
//
//		if (frame != nullptr) {
//			frame->GetWindow()->AddConnection(eos::Frame::Events::MINIMIZE, GetOnWindowMinimize());
//
//			frame->GetWindow()->AddConnection(eos::Frame::Events::FULL_SCREEN, GetOnWindowFullScreen());
//
//			frame->GetWindow()->AddConnection(eos::Frame::Events::CLOSE, GetOnWindowClose());
//
//			eos::sys::proxy::AddFrame(frame);
//
//			//			std::shared_ptr<eos::Desktop> desktop
//			//				= std::static_pointer_cast<eos::Desktop>(
//			//					ax::App::GetInstance().GetTopLevel()->backbone);
//
//			//			desktop->AddFrame(frame);
//			//			desktop->GetWindow()->node.Add(frame);
//			//			eos::Frame* osframe = static_cast<eos::Frame*>(frame);
//			//			static_cast<eos::Desktop*>(GetParent())->AddFrame(osframe);
//		}
//		else {
//			// WARNING MESSAGE BOX.
//		}
//		return;
//	}
//
//	ax::Window::Backbone* frame = loader.GetHandle();
//
//	if (frame != nullptr) {
//		if (frame->GetWindow()->IsShown()) {
//			frame->GetWindow()->Hide();
//		}
//		else {
//			frame->GetWindow()->Show();
//		}
//	}
//}