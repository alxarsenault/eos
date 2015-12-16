#include "eosSystemCore.h"

int main()
{
	eos::sys::Core* core = eos::sys::Core::Create();
	return core->MainLoop();
}

//#include "axLib/axLib.h"
//#include "axLib/axGLShader.h"
//
//#include "eosCoreSystem.h"
//#include "eosDesktop.h"
//
//#include "WindowServer.hpp"
//
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <unistd.h>
//#include <fcntl.h>
//
//template <typename T> class SharedMemory {
//public:
//	SharedMemory(const key_t& key, const std::size_t& size)
//		: _key(key)
//		, _size(size)
//	{
//		_id = shmget(key, size * sizeof(T), IPC_CREAT | 0666);
//
//		if (_id < 0) {
//			std::cerr << "shget" << std::endl;
//			return;
//		}
//
//		_data = (T*)shmat(_id, NULL, 0);
//
//		if ((char*)_data == (char*)-1) {
//			std::cerr << "shmat" << std::endl;
//			return;
//		}
//	}
//
//	T* data()
//	{
//		return _data;
//	}
//
//	T& operator[](const unsigned int& i)
//	{
//		return _data[i];
//	}
//
//	std::size_t size() const
//	{
//		return _size;
//	}
//
//private:
//	key_t _key;
//	std::size_t _size;
//	int _id;
//	T* _data;
//};
//
//int main()
//{
//	//	SharedMemory<char> shared_buffer(key_t(8322),
//	//		std::size_t(sizeof(char) * 300 * 300 * 4));
//
//	ax::App& app(ax::App::GetInstance());
//
//	ax::GC gc;
//	gc.shader_normal = ax::GL::Shader(
//		"resources/vertex_shader.glsl", "resources/fragments_shader.glsl");
//
//	gc.shader_fb = ax::GL::Shader("resources/fb_vertex_shader.glsl",
//		"resources/fb_fragments_shader.glsl");
//
//	gc.shader_font = ax::GL::Shader("resources/font_vertex_shader.glsl",
//		"resources/font_fragments_shader.glsl");
//
//	// Server obj.
//	//	ax::Event::Object obj(app.GetEventManager());
//	//
//	//	obj.AddConnection(ws::Event::CREATE_FRAME,
//	//		ax::Event::Function([&shared_buffer](ax::Event::Msg* msg)
//	//	{
//	//		ax::Rect
//	//rect(static_cast<ax::Event::SimpleMsg<ax::Rect>*>(msg)->GetMsg());
//	//		ax::Print("Rect :",rect.position.x, rect.position.y, rect.size.x,
//	//rect.size.y);
//	//
//	//		std::shared_ptr<eos::Desktop> desktop =
//	//std::static_pointer_cast<eos::Desktop>(ax::App::GetInstance().GetTopLevel()->backbone);
//	//
//	//		std::shared_ptr<eos::Frame> frame (new eos::Frame(rect, "Remote
//	//Window"));
//	//		frame->GetWindow()->property.AddProperty("Resizable");
//	//
//	//		ax::Window::Ptr win = ax::Window::Create(frame->GetChildRect());
//	//
//	//		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win,
//	//&shared_buffer](ax::Point pos) {
//	//			win->Update();
//	//		});
//	//
//	//		win->event.OnPaint = ax::WFunc<ax::GC>([win, &shared_buffer](ax::GC
//	//gc) {
//	//			ax::Image img((void*)shared_buffer.data(), ax::Size(300, 300));
//	//			gc.SetColor(ax::Color(1.0));
//	//			gc.DrawRectangle(win->dimension.GetDrawingRect());
//	//			gc.DrawImage(&img, ax::Point(0, 0));
//	//		});
//	//
//	//		frame->GetWindow()->node.Add(win);
//	//		frame->SetChildHandler(win);
//	//		desktop->AddFrame(frame);
//	//	}));
//	//
//	//	// Server thread.
//	//	std::thread win_server([](ax::Event::Object& obj) {
//	//		boost::asio::io_service io_service;
//	//		std::string sock_path("/Users/alexarse/Desktop/socky02");
//	//		std::remove(sock_path.c_str());
//	//		server s(io_service, sock_path.c_str(), obj);
//	//		io_service.run();
//	//	}, std::ref(obj));
//
//	// System.
//	std::shared_ptr<eos::Core::System> system(new eos::Core::System());
//	ax::GL::Shader shader(
//		"img_vertex_shader.glsl", "img_fragments_shader.glsl");
//
//	// This will eventually not be there.
//	system->LoginUser(1);
//
//	app.AddMainEntry([&app, system, &shader, &gc]() {
//		//		shader.CompileAndLink();
//		//		shader.Activate();
//
//		gc.shader_normal.CompileAndLink();
//		gc.shader_fb.CompileAndLink();
//		gc.shader_font.CompileAndLink();
//
//		gc.shader_fb.Activate();
//
//#ifdef __linux__
//		ax::Size size(app.GetScreenSize());
//#else
//	ax::Size size(ax::Size(1000, 700));
////				ax::Size size(ax::Size(1280, 800)); // Desktop.
////		ax::Size size(ax::Size(750, 450)); // Tablet.
//		app.SetFrameSize(size);
//#endif
//
//		std::shared_ptr<eos::Desktop> desktop(
//			new eos::Desktop(ax::Rect(ax::Point(0, 0), size)));
//
//		desktop->GetWindow()
//			->dimension.GetFrameBuffer()
//			->AssignCustomFBDrawFunction([desktop](ax::GL::FrameBuffer& fb) {
//				glEnable(GL_TEXTURE_2D);
//
//				glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
//
//				ax::FloatPoint pos(0.0, 0.0);
//				const ax::Size& ss(
//					desktop->GetWindow()->dimension.GetShownRect().size);
//				ax::FloatSize size(ss.x, ss.y);
//
//				// Bind framebuffer texture.
//				glBindTexture(GL_TEXTURE_2D, desktop->GetWindow()
//												 ->dimension.GetFrameBuffer()
//												 ->GetFrameBufferTexture());
//				glEnableClientState(GL_VERTEX_ARRAY);
//				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//				float vertices[8] = { pos.x, pos.y, pos.x, pos.y + size.y,
//					pos.x + size.x, pos.y + size.y, pos.x + size.x, pos.y };
//
//				float tex_coords[8]
//					= { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
//
//				glVertexPointer(2, GL_FLOAT, 0, vertices);
//				glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
//
//				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//
//				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//				glDisableClientState(GL_VERTEX_ARRAY);
//
//				glDisable(GL_TEXTURE_2D);
//			});
//
//		app.AddTopLevel(desktop);
////		system->GetDesktopManager()->SetDesktop(desktop);
//	});
//
//	app.AddAfterGUILoadFunction([&app]() {
//		app.SetFrameSize(ax::Size(1000, 700));
//	});
//
//	app.MainLoop();
//
//	//	eos::Core::System* system = new eos::Core::System();
//	//	system->LoginUser(1);
//	//
//	//	system->GetManager()->AddMainEntry([system]()
//	//	{
//	//		ax::App* app = system->GetManager()->GetMainApp();
//	//
//	//	#ifdef __linux__
//	//		ax::Size size(app->GetScreenSize());
//	//	#else
//	//		ax::Size size(ax::Size(1000, 700));
//	//		app->SetFrameSize(size);
//	//	#endif
//	//
//	//		ax::Rect deskRect(ax::Point(0, 0), size);
//	//		system->GetManager()->SetDesktop(new eos::Desktop(system,
//	// deskRect));
//	//	});
//	//
//	//	system->GetManager()->MainLoop();
//
//	return 0;
//}
