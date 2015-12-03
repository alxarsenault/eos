#include "axLib/axLib.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"
#include "WindowServer.hpp"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

template<typename T>
class SharedMemory
{
public:
	SharedMemory(const key_t& key, const std::size_t& size)
	: _key(key)
	, _size(size)
	{
		_id = shmget(key, size * sizeof(T), IPC_CREAT | 0666);
		
		if(_id < 0) {
			std::cerr << "shget" << std::endl;
			return;
		}
		
		_data = (T*) shmat(_id, NULL, 0);
		
		if((char*)_data == (char*) -1) {
			std::cerr << "shmat" << std::endl;
			return;
		}
	}
	
	T* data()
	{
		return _data;
	}
	
	T& operator[] (const unsigned int& i)
	{
		return _data[i];
	}
	
	std::size_t size() const
	{
		return _size;
	}
	
private:
	key_t _key;
	std::size_t _size;
	int _id;
	T* _data;
};

int main()
{
	SharedMemory<char> shared_buffer(key_t(8322),
		std::size_t(sizeof(char) * 300 * 300 * 4));
	
	ax::App& app(ax::App::GetInstance());
	
	// Server obj.
	ax::Event::Object obj(app.GetEventManager());
	
	obj.AddConnection(ws::Event::CREATE_FRAME,
		ax::Event::Function([&shared_buffer](ax::Event::Msg* msg)
	{
		ax::Rect rect(static_cast<ax::Event::SimpleMsg<ax::Rect>*>(msg)->GetMsg());
		ax::Print("Rect :",rect.position.x, rect.position.y, rect.size.x, rect.size.y);
		
		std::shared_ptr<eos::Desktop> desktop = std::static_pointer_cast<eos::Desktop>(ax::App::GetInstance().GetTopLevel()->backbone);
	
		std::shared_ptr<eos::Frame> frame (new eos::Frame(rect, "Remote Window"));
		frame->GetWindow()->property.AddProperty("Resizable");
		
		ax::Window::Ptr win = ax::Window::Create(frame->GetChildRect());
		
		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([win, &shared_buffer](ax::Point pos) {
			win->Update();
		});
		
		win->event.OnPaint = ax::WFunc<ax::GC>([win, &shared_buffer](ax::GC gc) {
			ax::Image img((void*)shared_buffer.data(), ax::Size(300, 300));
			gc.SetColor(ax::Color(1.0));
			gc.DrawRectangle(win->dimension.GetDrawingRect());
			gc.DrawImage(&img, ax::Point(0, 0));
		});
		
		frame->GetWindow()->node.Add(win);
		frame->SetChildHandler(win);
		desktop->AddFrame(frame);
	}));
	
	// Server thread.
	std::thread win_server([](ax::Event::Object& obj) {
		boost::asio::io_service io_service;
		std::string sock_path("/Users/alexarse/Desktop/socky02");
		std::remove(sock_path.c_str());
		server s(io_service, sock_path.c_str(), obj);
		io_service.run();
	}, std::ref(obj));

	// System.
	std::shared_ptr<eos::Core::System> system(new eos::Core::System());

	// This will eventually not be there.
	system->LoginUser(1);

	app.AddMainEntry([&app, system]() {
		//#ifdef __linux__
		//		ax::Size size(app.GetScreenSize());
		//#else
		// 1280 x 800
		ax::Size size(ax::Size(1280, 800)); // Desktop.
		//ax::Size size(ax::Size(700, 500)); // Tablet.
		app.SetFrameSize(size);
		//#endif

		std::shared_ptr<eos::Desktop> desktop(
			new eos::Desktop(ax::Rect(ax::Point(0, 0), size)));

		app.AddTopLevel(desktop);
		system->GetDesktopManager()->SetDesktop(desktop);

		// Test frame.
//		desktop->GetWindow()->node.Add(std::shared_ptr<eos::Frame>(
//			new eos::Frame(ax::Rect(100, 100, 500, 500), "Test")));

		// ax::Window::Ptr desktop = system->GetDesktopManager()->GetWindow();

		//		system->GetManager()->SetDesktop(new eos::Desktop(system,
		// deskRect));

	});

	app.AddAfterGUILoadFunction(
		[&app]() {
//		app.SetFrameSize(ax::Size(700, 500));
	});

	app.MainLoop();

	//	eos::Core::System* system = new eos::Core::System();
	//	system->LoginUser(1);
	//
	//	system->GetManager()->AddMainEntry([system]()
	//	{
	//		ax::App* app = system->GetManager()->GetMainApp();
	//
	//	#ifdef __linux__
	//		ax::Size size(app->GetScreenSize());
	//	#else
	//		ax::Size size(ax::Size(1000, 700));
	//		app->SetFrameSize(size);
	//	#endif
	//
	//		ax::Rect deskRect(ax::Point(0, 0), size);
	//		system->GetManager()->SetDesktop(new eos::Desktop(system,
	// deskRect));
	//	});
	//
	//	system->GetManager()->MainLoop();

	return 0;
}
