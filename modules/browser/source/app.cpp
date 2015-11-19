#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include <dirent.h>

#include "eosFrame.h"
#include "eosBrowser.h"

extern "C"
{
	ax::Window::Ptr StartApplication(ax::Window::Ptr parent, ax::Rect rect)
	{
		rect.size = ax::Size(500, 500);

		eos::Frame* frame = new eos::Frame(parent, rect, "Browser");
		frame->AddProperty("Resizable");
	
		eos::Browser* browser = new eos::Browser(frame, frame->GetChildRect());
		
		frame->SetChildHandler(browser);
		
		
		return frame;
	}
}
