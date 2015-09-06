#include "axLib/axLib.h"

#include "eosFrame.h"
#include "axOSTerminal.h"
#include "eosTerminal.h"

class TerminalMenu : public axPanel
{
public:
    TerminalMenu(axWindow* parent, const ax::Rect& rect):
    // Parent.
    axPanel(parent, rect)
    {
        ax::Button::Info menu_btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
                                       ax::Color(1.0, 0.0, 0.0, 0.0),
                                       ax::Color(0.95, 0.0, 0.0, 0.0),
                                       ax::Color(0.5, 0.5, 0.5, 0.0),
                                       ax::Color(0.0, 0.0, 0.0, 0.0),
                                       ax::Color(0.0, 0.0, 0.0, 0.0),
                                       0);
        
        new ax::Button(this, ax::Rect(10, 10, 48, 48),
                       ax::Button::Events(GetOnInfoClick()),
                       menu_btn_info,
                       "resource/info.png", "",
                       ax::Button::Flags::SINGLE_IMG);
    }
    
private:
    
    axEVENT_ACCESSOR(ax::Button::Msg, OnInfoClick);
    void OnInfoClick(const ax::Button::Msg& msg)
    {
        ax::Print("INFO CLICK.");
    }
    
    void OnPaint()
    {
        ax::GC gc;
        ax::Rect rect(GetDrawingRect());
       
        gc.SetColor(ax::Color(1.0, 0.2));
        gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
        
        gc.SetColor(ax::Color(0.05, 0.6));
        gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
    }
};


extern "C"
{
    axWindow* TerminalMode(axWindow* parent, ax::Rect rect)
    {
        eos::Terminal::Info info(ax::Color(0.9, 0.9, 0.9),
                                 ax::Color(0.4, 0.4),
                                 ax::Color(0.0, 1.0, 0.0, 0.3));
        
        return new eos::Terminal(parent, rect, info);
    }
    
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Size fixe_size(500, 500);
		eos::Frame* frame = new eos::Frame(parent, 
										   ax::Rect(rect.position, fixe_size), 
										   "Terminal");
		frame->AddProperty("Resizable");

        eos::Terminal::Info info;
		eos::Terminal* term = new eos::Terminal(frame,
                                                frame->GetChildRect(),
                                                info);
		frame->SetChildHandler(term);
        
        TerminalMenu* menu = new TerminalMenu(frame, frame->GetChildRect());
        frame->SetChildMenuHandler(menu);
		
		return frame;
	}
}
//int main()
//{
//    ax::App app(ax::Size(600, 500));
//    
//    app.AddMainEntry([&app]()
//                     {
//                         new MainPanel(&app, ax::Rect(0, 0, 600, 500));
//                     });
//    
//    app.MainLoop();
//    
//    return 0;
//}
