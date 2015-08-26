#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include <dirent.h>

#include "eosFrame.h"
#include "eosBrowser.h"

class NotePad : public axPanel
{
public:
	NotePad(axWindow* parent, const ax::Rect& rect):
    // Parent.
    axPanel(parent, rect),
	_font(0)
	{
		std::string file_path("/home/pi/Projects/axFrameworks/samples/pi/tri_backup.cpp");
		std::ifstream file_stream(file_path);
		std::string file_data((std::istreambuf_iterator<char>(file_stream)),
							  std::istreambuf_iterator<char>());
		_file_content = ax::Utils::String::Split(file_data, "\n");
	}


private:
	ax::StringVector _file_content;
	ax::Font _font;

	void OnPaint()
	{
		ax::GC gc;
		ax::Rect rect(GetDrawingRect());


		gc.SetColor(ax::Color(0.95));                                                                         
		gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));


		gc.SetColor(ax::Color(0.0));
		
		// Draw file content.	
		ax::Point str_pos(10, 10);
		
		for(int i = 0; i < _file_content.size(); i++)
		{
			gc.DrawString(_font, _file_content[i], str_pos);
			str_pos += ax::Point(0, 15);

			if(str_pos.y > rect.size.y - 15)
			{
				break;
			}
		}
	}
};

extern "C"
{
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Print("Start application.");
		//MainPanel* main_panel = new MainPanel(parent, rect);
		eos::Frame* frame = new eos::Frame(parent, rect, "Browser");

	//	ax::Rect rel_rect(4 + 5, 25, rect.size.x - 2 * (4 + 5), rect.size.y - 25 - (4+5));		
		eos::Browser* browser = new eos::Browser(frame, frame->GetChildRect());
		
		//NotePad* note = new NotePad(main_panel, ax::Rect(4 + 5, 25, rect.size.x - 2*(4 + 5), rect.size.y - 25 - (4 + 5)));

		frame->SetChildHandler(browser);
		return frame;
	}
}

//int main()
//{
//	ax::Print("My app.");
	//ax::App app;

	//app.AddMainEntry([&app]()
	//{
	//	new MainPanel(&app, ax::Rect(ax::Point(0, 0), app.GetScreenSize()));
	//});

	//app.MainLoop();

//	return 0;
//}
