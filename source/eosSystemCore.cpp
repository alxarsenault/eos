#include "eosSystemCore.h"
#include "axLib/axLib.h"
#include "axLib/axGLShader.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"

#include "WindowServer.hpp"

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
	}
	
	int Core::MainLoop()
	{
		ax::App& app(ax::App::GetInstance());
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
		
//		_desktop->AddFrame(frame);
	}
	
	void Core::BringToFront(ax::Window::Ptr frame)
	{
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
	}
	
	void Core::FullScreenFrame(ax::Window::Ptr frame)
	{
//		_instance->BringToFront(msg.GetSender()->GetWindow());
//		eos::sys::proxy::BringToFront(msg.GetSender()->GetWindow());
//		msg.GetSender()->SetFullScreen(ax::Rect(ax::Point(0, 25), win->dimension.GetSize() - ax::Size(0, 25)));
	}

	std::shared_ptr<eos::Desktop> Core::GetDesktop()
	{
		return _desktop;
	}

	void Core::SetupMainApp()
	{
		ax::App& app(ax::App::GetInstance());

		ax::GC gc;
		gc.shader_normal = ax::GL::Shader(
			"resources/vertex_shader.glsl", "resources/fragments_shader.glsl");

		gc.shader_fb = ax::GL::Shader("resources/fb_vertex_shader.glsl",
			"resources/fb_fragments_shader.glsl");

		gc.shader_font = ax::GL::Shader("resources/font_vertex_shader.glsl",
			"resources/font_fragments_shader.glsl");

		// Server obj.
		//	ax::Event::Object obj(app.GetEventManager());
		//
		//	obj.AddConnection(ws::Event::CREATE_FRAME,
		//		ax::Event::Function([&shared_buffer](ax::Event::Msg* msg)
		//	{
		//		ax::Rect
		// rect(static_cast<ax::Event::SimpleMsg<ax::Rect>*>(msg)->GetMsg());
		//		ax::Print("Rect :",rect.position.x, rect.position.y,
		//rect.size.x,
		// rect.size.y);
		//
		//		std::shared_ptr<eos::Desktop> desktop =
		// std::static_pointer_cast<eos::Desktop>(ax::App::GetInstance().GetTopLevel()->backbone);
		//
		//		std::shared_ptr<eos::Frame> frame (new eos::Frame(rect, "Remote
		// Window"));
		//		frame->GetWindow()->property.AddProperty("Resizable");
		//
		//		ax::Window::Ptr win = ax::Window::Create(frame->GetChildRect());
		//
		//		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win,
		//&shared_buffer](ax::Point pos) {
		//			win->Update();
		//		});
		//
		//		win->event.OnPaint = ax::WFunc<ax::GC>([win,
		//&shared_buffer](ax::GC
		// gc) {
		//			ax::Image img((void*)shared_buffer.data(), ax::Size(300,
		//300));
		//			gc.SetColor(ax::Color(1.0));
		//			gc.DrawRectangle(win->dimension.GetDrawingRect());
		//			gc.DrawImage(&img, ax::Point(0, 0));
		//		});
		//
		//		frame->GetWindow()->node.Add(win);
		//		frame->SetChildHandler(win);
		//		desktop->AddFrame(frame);
		//	}));
		//
		//	// Server thread.
		//	std::thread win_server([](ax::Event::Object& obj) {
		//		boost::asio::io_service io_service;
		//		std::string sock_path("/Users/alexarse/Desktop/socky02");
		//		std::remove(sock_path.c_str());
		//		server s(io_service, sock_path.c_str(), obj);
		//		io_service.run();
		//	}, std::ref(obj));

		// System.
		std::shared_ptr<eos::Core::System> system(new eos::Core::System());
		ax::GL::Shader shader(
			"img_vertex_shader.glsl", "img_fragments_shader.glsl");

		// This will eventually not be there.
		system->LoginUser(1);

		app.AddMainEntry([&, system]() {
			ax::App& app(ax::App::GetInstance());
			ax::GC gc;
			gc.shader_normal.CompileAndLink();
			gc.shader_fb.CompileAndLink();
			gc.shader_font.CompileAndLink();

			gc.shader_fb.Activate();

#ifdef __linux__
			ax::Size size(app.GetScreenSize());
#else
			ax::Size size(ax::Size(1000, 700));
			//				ax::Size size(ax::Size(1280, 800)); // Desktop.
			//		ax::Size size(ax::Size(750, 450)); // Tablet.
			app.SetFrameSize(size);
#endif

			_desktop = std::shared_ptr<eos::Desktop>(
				new eos::Desktop(ax::Rect(ax::Point(0, 0), size)));

			_desktop->GetWindow()
				->dimension.GetFrameBuffer()
				->AssignCustomFBDrawFunction([&](
					ax::GL::FrameBuffer& fb) {
					glEnable(GL_TEXTURE_2D);

					glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

					ax::FloatPoint pos(0.0, 0.0);
					const ax::Size& ss(
						_desktop->GetWindow()->dimension.GetShownRect().size);
					ax::FloatSize size(ss.x, ss.y);

					// Bind framebuffer texture.
					glBindTexture(
						GL_TEXTURE_2D, _desktop->GetWindow()
										   ->dimension.GetFrameBuffer()
										   ->GetFrameBufferTexture());
					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);

					float vertices[8] = { pos.x, pos.y, pos.x, pos.y + size.y,
						pos.x + size.x, pos.y + size.y, pos.x + size.x, pos.y };

					float tex_coords[8]
						= { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

					glVertexPointer(2, GL_FLOAT, 0, vertices);
					glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

					glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);

					glDisable(GL_TEXTURE_2D);
				});

			app.AddTopLevel(_desktop);
//			system->GetDesktopManager()->SetDesktop(desktop);
		});

		app.AddAfterGUILoadFunction(
			[&app]() { app.SetFrameSize(ax::Size(1000, 700)); });
	}
}
}

//
// void eos::Core::Manager::AddFrame(eos::Frame* frame)
//{
//	if(frame != nullptr)
//	{
//		axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
//
//		axWindowNode* main_node = tree->FindWinNode(_desktop);
//
//		std::vector<axWindowNode*>& node_childs = main_node->GetChild();
//
//		if(_childs.size() > 1)
//		{
//			// First added child.
//			ax::Window* first_desktop_child = _childs[0];
//			std::vector<axWindowNode*>::iterator node = node_childs.end();
//
//			// Find node.
//			for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
//			{
//				if((*it)->GetWindow() == first_desktop_child)
//				{
//					node = it;
//					break;
//				}
//			}
//
//			if(node != node_childs.end())
//			{
//				axWindowNode* frame_node = node_childs.back();
//				node_childs.insert(node, frame_node);
//				node_childs.pop_back();
//			}
//		}
//	}
//}
//
// void eos::Core::Manager::BringToFront(eos::Frame* frame)
//{
//	if(frame != nullptr)
//	{
//		axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
//		axWindowNode* main_node = tree->FindWinNode(_desktop);
//
//		std::vector<axWindowNode*>& node_childs = main_node->GetChild();
//		std::vector<axWindowNode*>::iterator node = node_childs.end();
//
//		if(_childs.size())
//		{
//			ax::Window* first_desktop_child = _childs[0];
//
//			// Find node.
//			for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
//			{
//				if((*it)->GetWindow() == first_desktop_child)
//				{
//					node = it;
//					break;
//				}
//			}
//		}
//		else
//		{
//			node = node_childs.end() - 1;
//		}
//
//		std::vector<axWindowNode*>::iterator frame_node =  node_childs.end();
//
//		// Find frame node.
//		for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
//		{
//			if((*it)->GetWindow() == frame)
//			{
//				frame_node = it;
//				break;
//			}
//		}
//
//		if(node != node_childs.end() && frame_node != node_childs.end())
//		{
//			node_childs.insert(node, *frame_node);
//			node_childs.erase(frame_node);
//		}
//	}
//}
//
// std::vector<eos::Frame*> eos::Core::Manager::GetFrameVector()
//{
//	std::vector<eos::Frame*> frames;
//	axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
//	axWindowNode* main_node = tree->FindWinNode(_desktop);
//
//	ax::Window* first_desktop_child = nullptr;
//
//	if(_childs.size())
//	{
//		first_desktop_child = _childs[0];
//	}
//
//	for(auto& n : main_node->GetChild())
//	{
//		if(n->window == first_desktop_child)
//		{
//			break;
//		}
//
//		frames.push_back(static_cast<eos::Frame*>(n->window));
//	}
//
//	return frames;
//}