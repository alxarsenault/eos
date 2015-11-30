#include "axlib/axLib.h"
#include "axLib/axButton.h"
#include "opencv2/opencv.hpp"
#include <random>
#include "axLib/axGL.h"
#include "eosFrame.h"
#include <fstream>
#include <string>

namespace eos {
class SlideShow : public ax::Window::Backbone {
public:
	SlideShow(const ax::Rect& rect)
		: _current_slide_img(nullptr)
	{
		win = ax::Window::Create(rect);

		win->event.OnPaint = ax::WBind<ax::GC>(this, &SlideShow::OnPaint);

		std::ifstream file("resources/presentation.txt");
		std::string str;

		while (std::getline(file, str)) {
			_imgs_path.push_back(str);
		}

		for (auto& n : _imgs_path) {
			ax::Print(n);
		}

		if (!_imgs_path.empty()) {
			_current_slide_img = new ax::Image(_imgs_path[0]);
		}
	}

private:
	ax::StringVector _imgs_path;
	ax::Image* _current_slide_img;

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(1.0));
		gc.DrawRectangle(rect);

		if (_current_slide_img != nullptr
			&& _current_slide_img->IsImageReady()) {

			gc.DrawImageResize(_current_slide_img, rect.position,
				rect.size);
		}
	}
};
}

extern "C" {
ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
{
	ax::Size fixe_size(1000, 700);

	eos::Frame* frame
		= new eos::Frame(ax::Rect(rect.position, fixe_size), "Terminal");

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
