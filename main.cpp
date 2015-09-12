#include "axLib/axLib.h"

#include "eosCoreSystem.h"
#include "eosDesktop.h"

int main()
{
    eos::Core::System* system = new eos::Core::System();
    system->LoginUser(1);
    
    system->GetManager()->AddMainEntry([system]()
    {
        ax::App* app = system->GetManager()->GetMainApp();

        #ifdef __linux__
        ax::Size size(app->GetScreenSize());
        #else
        ax::Size size(ax::Size(1000, 700));
        app->SetFrameSize(size);
        #endif
        
        ax::Rect deskRect(ax::Point(0, 0), size);
        system->GetManager()->SetDesktop(new eos::Desktop(system, deskRect));
    });

    system->GetManager()->MainLoop();
    
	return 0;
}

