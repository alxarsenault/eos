#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include <random>
#include "axLib/axGL.h"
#include "eosFrame.h"
#include <fstream>
#include <string>

#include <ImageMagick/Magick++.h>
extern "C"{
  #include <xdo.h>
}

namespace eos {
class SlideShow : public ax::Window::Backbone {
public:
	SlideShow(const ax::Rect& rect)
		: _current_slide_img(nullptr)
	{
		win = ax::Window::Create(rect);

		win->event.OnPaint = ax::WBind<ax::GC>(this, &SlideShow::OnPaint);

		p_xdo = xdo_new(NULL);
		// Allocate memory for search query.
		xdo_search_t s;
		// Clear the allocated memory.
		memset(&s, 0, sizeof(xdo_search_t));
		
		//s.pid = 1916;
		s.max_depth = -1;
		s.searchmask = SEARCH_NAME;
		s.require = xdo_search::SEARCH_ANY;
		s.winname = "chromium";

		Window* windows;
		int no_windows;
		xdo_search_windows(p_xdo, &s, &windows, &no_windows);
		std::cout << no_windows << std::endl;
		// Prints all windows' names with matching criteria
		for(int i = 0; i < no_windows; i++){

			unsigned char* name;
			int size;
			int type;
			xdo_get_window_name(p_xdo, windows[i], &name, &size, &type);
			std::cout << i << ":" << name << std::endl;
			_win_id = windows[i];
		}

	


		pixels = new char[500 * 500 * 3];

		win->event.OnMouseLeftDown = ax::WFunc<ax::Point>([&](const ax::Point& pos) {
				ax::Point mpos = pos - win->dimension.GetAbsoluteRect().position;
			
			xdo_move_mouse(p_xdo, mpos.x, mpos.y, 0);
			xdo_mouse_down(p_xdo, _win_id, 1);
			
			ax::Print("Motion.");			
			Magick::Image img;
			img.density(Magick::Geometry(1080, 1440));
			img.read("/var/run/Xvfb_screen0");
			//img.write("test.png");
			img.write(0, 0, 500, 500, "RGB", Magick::CharPixel, pixels);
	
			delete _current_slide_img;
			_current_slide_img = nullptr;	
			_current_slide_img = new ax::Image((void*)pixels, ax::Size(500, 500), 
				ax::Image::ColorType::RGB);
			win->Update();		
		});
		//delete pixels;
	}

private:
	ax::StringVector _imgs_path;
	ax::Image* _current_slide_img;
	xdo_t* p_xdo;
	Window _win_id;
	char* pixels;

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		if (_current_slide_img != nullptr
			&& _current_slide_img->IsImageReady()) {

			//DrawImageResize(ax::Image* img, const ax::Point& position,
			//		const ax::Size& size, float alpha = 1.0, bool vertical_inversion = false);
			gc.DrawImageResize(_current_slide_img, rect.position,
				rect.size, 1.0, true);
		}
	}
};
}

extern "C" {
ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
{
	ax::Size fixe_size(508, 520);

	eos::Frame* frame
		= new eos::Frame(ax::Rect(rect.position, fixe_size), "Chrome");

	frame->GetWindow()->property.AddProperty("Resizable");

	ax::Window::Ptr win
		= frame->GetWindow()->node.Add(std::shared_ptr<eos::SlideShow>(
			new eos::SlideShow(frame->GetChildRect())));

	frame->SetChildHandler(win);

	return frame;
}
}

int main()
{
	//	ax::App& app = ax::App::GetInstance();
	//
	//	app.AddMainEntry([&app]() {
	//
	//
	//		std::shared_ptr<eos::VideoPlayer> player(
	//			new eos::VideoPlayer(ax::Rect(0, 0, 600, 500)));
	//
	//		win->node.Add(player);
	//	});
	//
	//	app.MainLoop();

	return 0;
}
