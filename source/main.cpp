#include "axLib/axLib.h"

#include "eosWindowManager.h"
#include "eosDesktop.h"

int main()
{
    eos::WindowManager eosManager;
    
    eosManager.AddMainEntry([&eosManager]()
    {
        ax::App* app = eosManager.GetMainApp();

        #ifdef __linux__
        ax::Size size(app.GetScreenSize());
        #else
//        ax::Size size(ax::Size(1000, 700));
        ax::Size size(ax::Size(1000, 700));
        app->SetFrameSize(size);
        #endif
        
        ax::Rect deskRect(ax::Point(0, 0), size);
        eosManager.SetDesktop(new eos::Desktop(&eosManager, deskRect));
    });

    eosManager.MainLoop();
    
	return 0;
}

