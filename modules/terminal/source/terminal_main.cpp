#include "axLib/axLib.h"

#include "eosFrame.h"
#include "axOSTerminal.h"
#include "eosTerminal.h"

//class TerminalMenu : public axPanel
//{
//public:
//	TerminalMenu(ax::Window* parent, const ax::Rect& rect):
//	// Parent.
//	axPanel(parent, rect)
//	{
//		ax::Button::Info menu_btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
//									   ax::Color(1.0, 0.0, 0.0, 0.0),
//									   ax::Color(0.95, 0.0, 0.0, 0.0),
//									   ax::Color(0.5, 0.5, 0.5, 0.0),
//									   ax::Color(0.0, 0.0, 0.0, 0.0),
//									   ax::Color(0.0, 0.0, 0.0, 0.0),
//									   0);
//		
//		new ax::Button(this, ax::Rect(10, 10, 48, 48),
//					   ax::Button::Events(GetOnInfoClick()),
//					   menu_btn_info,
//					   "resource/info.png", "",
//					   ax::Button::Flags::SINGLE_IMG);
//	}
//	
//private:
//	
//	axEVENT_ACCESSOR(ax::Button::Msg, OnInfoClick);
//	void OnInfoClick(const ax::Button::Msg& msg)
//	{
//		ax::Print("INFO CLICK.");
//	}
//	
//	void OnPaint()
//	{
//		ax::GC gc;
//		ax::Rect rect(GetDrawingRect());
//		
//		gc.SetColor(ax::Color(1.0, 0.2));
//		gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//		
//		gc.SetColor(ax::Color(0.05, 0.6));
//		gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//	}
//};


extern "C"
{
	ax::Window::Backbone* TerminalMode(ax::Rect rect)
	{
		eos::Terminal::Info info(ax::Color(0.9f, 0.9f, 0.9f),
								 ax::Color(0.4f, 0.4f),
								 ax::Color(0.0f, 1.0f, 0.0f, 0.3f));
		
		return new eos::Terminal(rect, info);
//		return parent->node.Add(std::shared_ptr<eos::Terminal>(new eos::Terminal(rect, info)));
	}
	
	ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
	{
		ax::Size fixe_size(500, 500);
		eos::Frame* frame = new eos::Frame(ax::Rect(rect.position, fixe_size),
										   "Terminal");
		frame->GetWindow()->property.AddProperty("Resizable");
		
		eos::Terminal::Info info;
		std::shared_ptr<eos::Terminal> term(new eos::Terminal(frame->GetChildRect(), info));
		
		frame->GetWindow()->node.Add(term);
		frame->SetChildHandler(term->GetWindow());
		
//		TerminalMenu* menu = new TerminalMenu(frame, frame->GetChildRect());
//		frame->SetChildMenuHandler(menu);
		
		return frame;
	}
}


int main()
{
	ax::App& app(ax::App::GetInstance());
	
	app.AddMainEntry([&app]()
	{
		app.SetFrameSize(ax::Size(600, 500));
		
		eos::Terminal::Info term_info;
		
		std::shared_ptr<eos::Terminal> term(new eos::Terminal(
			ax::Rect(0, 0, 600, 500), term_info));
		
		app.AddTopLevel(term);
		
//		app
	});
	
	app.MainLoop();
	
	return 0;
}
