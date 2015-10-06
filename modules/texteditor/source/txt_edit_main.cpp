#include "axLib/axLib.h"
//#include "eosBrowser.h"

//#include <ctime>
#include "axLib/axTimer.h"
#include <fstream>

#include "eosFrame.h"
#include "eosTextEditor.h"

class TextEditorMenu : public axPanel
{
public:
    TextEditorMenu(ax::Window* parent, const ax::Rect& rect):
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

class MainPanel : public axPanel
{
public:
    MainPanel(ax::Window* parent, const ax::Rect& rect):
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
        
        TextEditorMenu* menu = new TextEditorMenu(frame, frame->GetChildRect());
        frame->SetChildMenuHandler(menu);
        
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
