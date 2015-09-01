#include "axLib/axLib.h"

#include "eosFrame.h"
#include "axOSTerminal.h"
#include "eosTerminal.h"

//class MainPanel : public axPanel
//{
//public:
//    MainPanel(axWindow* parent, const ax::Rect& rect):
//    // Parent.
//    axPanel(parent, rect)
//    {
//        eos::Terminal* term = new eos::Terminal(this, rect);
//    }
//    
//private:
//    
//    void OnResize(const ax::Size& size)
//    {
//        SetSize(size);
//        
////        _editor->Resize(ax::Size(size.x - 1, size.y - 20));
//    }
//    
//    void OnPaint()
//    {
//        ax::GC gc;
//        ax::Rect rect(GetDrawingRect());
//        
//        gc.SetColor(ax::Color(0.96));
//        gc.DrawRectangle(rect);
//        
//        gc.SetColor(ax::Color(0.4));
//        gc.DrawRectangleContour(rect);
//    }
//};


extern "C"
{
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Print("Start terminal");

		ax::Size fixe_size(500, 500);
		eos::Frame* frame = new eos::Frame(parent, 
										   ax::Rect(rect.position, fixe_size), 
										   "Terminal");
		frame->AddProperty("Resizable");

		eos::Terminal* term = new eos::Terminal(frame, frame->GetChildRect());
		//MainPanel* term = new MainPanel(frame, frame->GetChildRect());
		frame->SetChildHandler(term);
		
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
