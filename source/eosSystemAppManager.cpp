#include "eosSystemAppManager.h"
#include "eosSystemProxy.h"

namespace eos {
namespace sys {
	AppManager::AppManager(std::shared_ptr<ax::Event::Manager> evt_manager)
		: ax::Event::Object(evt_manager)
	{
		/// @todo Eventually parse app folder and find apps info dynamically.
		_appLoaders["calc"] = AppLoader("./app/calculator.so");
		_appLoaders["browser"] = AppLoader("./app/browser.so");
		_appLoaders["TextEditor"] = AppLoader("./app/text_editor.so");
		_appLoaders["Terminal"] = AppLoader("./app/terminal.so");
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
		if (frame == nullptr) {
			ax::Error("AppManager::LaunchApplication :", app_name, "could not get frame handle.");
			return;
		}

		// Bring application frame to front view.
		frame->GetWindow()->Show();
		eos::sys::proxy::BringToFront(frame->GetWindow());
	}
	
	void AppManager::CloseFullScreenApp(const std::string& app_name)
	{
		std::map<std::string, eos::AppLoader>::iterator it = _appLoaders.find(app_name);
		
		if(it == _appLoaders.end()) {
			return;
		}
		
		ax::Window::Backbone* handle = it->second.GetHandle();
		
		if(handle == nullptr) {
			return;
		}
		
		eos::Frame* frame = static_cast<eos::Frame*>(handle);
		frame->UnSetFullScreen(ax::Rect(100, 100, 500, 500));
		
		PushEvent(UN_FULLSCREEN_FRAME, new eos::Frame::Msg(frame));
	}

	void AppManager::OnWindowMinimize(const eos::Frame::Msg& msg)
	{
		eos::Frame* frame = msg.GetSender();

		if (frame != nullptr) {
			frame->GetWindow()->Hide();
		}
	}

	void AppManager::OnWindowClose(const eos::Frame::Msg& msg)
	{
		ax::Print("Window close.");
		eos::Frame* frame = msg.GetSender();

		if (frame == nullptr) {
			ax::Error("Application already closed.");
			return;
		}

		// Find wich app loader created the frame.
		for (auto& n : _appLoaders) {
			ax::Window::Backbone* handle = n.second.GetHandle();

			if(handle == nullptr) {
				continue;
			}
			
			if(handle->GetWindow()->GetId() == frame->GetWindow()->GetId()) {
				ax::Print("App loader found");
				_appLoaders[n.first].RemoveHandle();
				eos::sys::proxy::RemoveFrame(frame->GetWindow()->backbone);
				
				return;
			}
		}
	}

	void AppManager::OnWindowFullScreen(const eos::Frame::Msg& msg)
	{
		eos::sys::proxy::FullScreenFrame(msg.GetSender()->GetWindow());
		PushEvent(FRAME_FULL_SCREEN, new eos::Frame::Msg(msg));
	}
	
	void AppManager::OnServerFrameCreation(const ax::Event::SimpleMsg<std::shared_ptr<ax::Window::Backbone>>& msg)
	{
		std::shared_ptr<ax::Window::Backbone> server_frame = msg.GetMsg();
		
		const ax::Size frame_size(eos::Frame::GetFrameSizeFromChildSize(server_frame->GetWindow()->dimension.GetSize()));
		
		std::shared_ptr<eos::Frame> frame(new eos::Frame(ax::Rect(50, 50, frame_size), "Server app"));
		
		server_frame->GetWindow()->dimension.SetPosition(frame->GetChildRect().position);
		
		frame->GetWindow()->node.Add(server_frame);
		
		eos::sys::proxy::AddFrame(frame);
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