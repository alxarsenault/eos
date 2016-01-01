#include "eosSystemCore.h"
#include "axLib/axLib.h"
#include "axLib/axGLShader.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"

#include "eosSystemAppManager.h"

//#include "WindowServer.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp> // glm::pi



eos::sys::Core* eos::sys::Core::_instance = nullptr;

namespace eos {
namespace sys {

	Core* Core::Create()
	{
		return (_instance = new Core());
	}

	Core::Core()
	{
		SetupMainApp();

		_appManager = std::shared_ptr<AppManager>(new AppManager(ax::App::GetInstance().GetEventManager()));
	}

	int Core::MainLoop()
	{
		ax::App& app(ax::App::GetInstance());
		_window_server->Run();
		app.MainLoop();

		return 0;
	}

	void Core::AddFrame(std::shared_ptr<ax::Window::Backbone> frame)
	{
		std::vector<ax::Window::Ptr>& children(_desktop->GetWindow()->node.GetChildren());

		unsigned int fdesk_app = _desktop->_desktop_apps[eos::Desktop::DesktopApps::DSKT_APP_NOTIFY]->GetId();
		std::size_t fdesk_app_index = 0;

		// Find first desktop app index.
		for (std::size_t i = _desktop->_last_icon_index; i < children.size(); i++) {
			if (children[i]->GetId() == fdesk_app) {
				fdesk_app_index = i;
				break;
			}
		}

		// Add frame on top of all frames.
		ax::Window::Ptr child = frame->GetWindow();
		child->node.SetParent(_desktop->GetWindow().get());
		children.insert(children.begin() + fdesk_app_index, child);
		child->backbone = frame;

		_desktop->GetWindow()->Update();
	}

	void Core::RemoveFrame(std::shared_ptr<ax::Window::Backbone> frame)
	{
		std::vector<ax::Window::Ptr>& children(_desktop->GetWindow()->node.GetChildren());

		unsigned int fdesk_app = _desktop->_desktop_apps[eos::Desktop::DesktopApps::DSKT_APP_NOTIFY]->GetId();
		std::size_t fdesk_app_index = 0;

		// Find first desktop app index.
		for (std::size_t i = _desktop->_last_icon_index; i < children.size(); i++) {
			if (children[i]->GetId() == fdesk_app) {
				fdesk_app_index = i;
				break;
			}
		}

		// Add frame on top of all frames.
		ax::Window::Ptr child = frame->GetWindow();
		std::size_t child_index = 0;

		// Look for frame in vector.
		for (auto& n : children) {
			if (child->GetId() == n->GetId()) {
				break;
			}
			child_index++;
		}

		// Remove frame.
		if (child_index != 0) {
			child->node.SetParent(nullptr);
			children.erase(children.begin() + child_index);
		}

		_desktop->GetWindow()->Update();
	}

	void Core::BringToFront(ax::Window::Ptr frame)
	{
		ax::Print("eos::sys::Core::BringToFront");

		if (frame != nullptr) {
			std::vector<ax::Window::Ptr>& children(_desktop->GetWindow()->node.GetChildren());

			unsigned int frame_id = frame->GetId();
			std::size_t frame_index = 0;

			// Find frame index.
			for (std::size_t i = _desktop->_last_icon_index; i < children.size(); i++) {
				if (children[i]->GetId() == frame_id) {
					frame_index = i;
					break;
				}
			}

			// Remove frame from child vector.
			children.erase(children.begin() + frame_index);

			unsigned int fdesk_app = _desktop->_desktop_apps[eos::Desktop::DSKT_APP_NOTIFY]->GetId();
			std::size_t fdesk_app_index = 0;

			// Find first desktop app index.
			for (std::size_t i = _desktop->_last_icon_index; i < children.size(); i++) {
				if (children[i]->GetId() == fdesk_app) {
					fdesk_app_index = i;
					break;
				}
			}

			// Add frame on top of all frames.
			children.insert(children.begin() + fdesk_app_index, frame);
		}

		_desktop->GetWindow()->Update();
	}

	void Core::FullScreenFrame(ax::Window::Ptr frame)
	{
		_instance->BringToFront(frame);
		std::shared_ptr<eos::Frame> fb(std::static_pointer_cast<eos::Frame>(frame->backbone));
		fb->SetFullScreen(
			ax::Rect(ax::Point(0, 0), _desktop->GetWindow()->dimension.GetSize() - ax::Size(0, 0)));
	}

	std::shared_ptr<AppManager> Core::GetAppManager()
	{
		return _appManager;
	}

	std::shared_ptr<eos::Desktop> Core::GetDesktop()
	{
		return _desktop;
	}

	void Core::SetupMainApp()
	{
		ax::App& app(ax::App::GetInstance());

		// System.
		std::shared_ptr<eos::Core::System> system(new eos::Core::System());
		
		const std::string& name("/Users/alexarse/Desktop/tpm");
		_window_server = std::unique_ptr<ax::wm::Manager>(new ax::wm::Manager(app.GetEventManager().get(), name));

		_window_server->AddConnection(ax::wm::msg::server::SERVER_FRAME_CREATION,
			_appManager->GetOnServerFrameCreation());


		// This will eventually not be there.
		system->LoginUser(1);

		app.AddMainEntry([&, system]() {
			ax::App& app(ax::App::GetInstance());

#ifdef __linux__
			ax::Size size(app.GetScreenSize());
#else
			ax::Size size(ax::Size(1000, 700));
			//	ax::Size size(ax::Size(1280, 800)); // Desktop.
			//	ax::Size size(ax::Size(750, 450)); // Tablet.
			app.SetFrameSize(size);
#endif

			_desktop = std::shared_ptr<eos::Desktop>(new eos::Desktop(ax::Rect(ax::Point(0, 0), size)));

			app.AddTopLevel(_desktop);
		});

		app.AddAfterGUILoadFunction([&app]() { app.SetFrameSize(ax::Size(1000, 700)); });
	}
}
}