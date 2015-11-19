#include "axLib/axLib.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"

int main()
{
	ax::App& app(ax::App::GetInstance());

	std::shared_ptr<eos::Core::System> system(new eos::Core::System());

	// This will eventually not be there.
	system->LoginUser(1);

	app.AddMainEntry([&app, system]() {
		//#ifdef __linux__
		//		ax::Size size(app.GetScreenSize());
		//#else
		// 1280 x 800
		ax::Size size(ax::Size(1280, 800)); // Desktop.
//		ax::Size size(ax::Size(700, 500)); // Tablet.
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
