#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include <dirent.h>

#include "eosFrame.h"
#include "eosBrowser.h"

extern "C" {
//ax::Window::Ptr StartApplication(ax::Window::Ptr parent, ax::Rect rect)
//{
//	rect.size = ax::Size(500, 500);
//
//	eos::Frame* frame = new eos::Frame(parent, rect, "Browser");
//	frame->AddProperty("Resizable");
//
//	eos::Browser* browser = new eos::Browser(frame, frame->GetChildRect());
//
//	frame->SetChildHandler(browser);
//
//	return frame;
//}

ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
{
	ax::Size fixe_size(500, 500);
	eos::Frame* frame
		= new eos::Frame(ax::Rect(rect.position, fixe_size), "File browser");
	frame->GetWindow()->property.AddProperty("Resizable");

	std::shared_ptr<eos::Browser> browser(
		new eos::Browser(frame->GetChildRect()));
	
	ax::Button::Info fbtn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
							   ax::Color(0.7, 0.3),
							   ax::Color(0.65, 0.3),
							   ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
							   ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
							   ax::Color(0.0f, 0.0f, 0.0f),
							   0);
	
	frame->GetWindow()->node.Add(ax::Button::Ptr(new ax::Button(ax::Rect(40, 4, 20, 20),
												 browser->GetOnButtonBack(),
												 fbtn_info, "resources/go10.png", "")));
	frame->GetWindow()->node.Add(browser);
	frame->SetChildHandler(browser->GetWindow());

	return frame;
}
}
