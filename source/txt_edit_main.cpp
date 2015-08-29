#include "axLib/axLib.h"
//#include "eosBrowser.h"

//#include <ctime>
#include "axLib/axTimer.h"
#include <fstream>

#include "eosFrame.h"
#include "eosTextEditor.h"


class MainPanel : public axPanel
{
public:
    MainPanel(axWindow* parent, const ax::Rect& rect):
    // Parent.
    axPanel(parent, rect)
    {
        ax::Rect r(1, 20, rect.size.x - 1, rect.size.y - 20);
        
        eos::TextEditor::Info txt_info;
//        txt_info.text_color = ax::Color(0.9647059, 0.9686275, 0.9333333);
//        txt_info.bg_color = ax::Color(0.1137255, 0.1176471, 0.09803922);
//        txt_info.line_number_color = ax::Color();

        _editor = new eos::TextEditor(this, r, txt_info);
    }
    
private:
    eos::TextEditor* _editor;
    
    void OnResize(const ax::Size& size)
    {
        SetSize(size);
        _editor->Resize(ax::Size(size.x - 1, size.y - 20));
    }
    
    void OnPaint()
    {
        ax::GC gc;
        ax::Rect rect(GetDrawingRect());
        
        gc.SetColor(ax::Color(0.96));
        gc.DrawRectangle(rect);
        
        gc.SetColor(ax::Color(0.4));
        gc.DrawRectangleContour(rect);
    }
};


extern "C"
{
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Print("Start TextEditor.");
		rect.size = ax::Size(500, 500);

		eos::Frame* frame = new eos::Frame(parent, rect, "TextEditor");
		frame->AddProperty("Resizable");

		MainPanel* txt_edit = new MainPanel(frame, frame->GetChildRect());

		frame->SetChildHandler(txt_edit);
		return frame;
	}
}



//
//int main()
//{
//    ax::App app(ax::Size(500, 500));
//    
//    app.AddMainEntry([&app]()
//                     {
//                         new MainPanel(&app, ax::Rect(0, 0, 512, 384));
//                     });
//    
//    app.MainLoop();
//    
//    return 0;
//}
