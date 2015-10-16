#include "axLib/axLib.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"

int main()
{
	using System = std::shared_ptr<eos::Core::System>;
	ax::App& app(ax::App::GetInstance());
	
	System system = System(new eos::Core::System());
	
	// This will eventually not be there.
	system->LoginUser(1);
	
	app.AddMainEntry([&app, system]()
	{
	#ifdef __linux__
		ax::Size size(app->GetScreenSize());
	#else
		ax::Size size(ax::Size(1000, 700)); // Desktop.
//		ax::Size size(ax::Size(500, 700)); // Tablet.
		app.SetFrameSize(size);
	#endif
		
		std::shared_ptr<eos::Desktop> desktop(
				new eos::Desktop(ax::Rect(ax::Point(0, 0), size)));
		
		app.AddTopLevel(desktop);
		
		
		
		//ax::Window::Ptr desktop = system->GetDesktopManager()->GetWindow();
		
		
//		system->GetManager()->SetDesktop(new eos::Desktop(system, deskRect));
	
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
//		system->GetManager()->SetDesktop(new eos::Desktop(system, deskRect));
//	});
//
//	system->GetManager()->MainLoop();
	
	return 0;
}

