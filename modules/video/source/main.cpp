#include "axlib/axLib.h"
#include "axLib/axButton.h"
#include "opencv2/opencv.hpp"
#include <random>
#include "axLib/axGL.h"
#include "eosFrame.h"

namespace eos {
class VideoPlayer : public ax::Window::Backbone {
public:
	VideoPlayer(const ax::Rect& rect)
		: _timer(ax::App::GetInstance().GetEventManager())
		, _load_drawing_timer(ax::App::GetInstance().GetEventManager())
		, _is_loading(false)
		, _has_video(false)
		, _is_playing(false)
		, _is_pause(false)
		, _font(0)
		, _loading_color(1.0f, 1.0f, 1.0f)
		, _distribution(0.5, 1.0)
	{
		_font.SetFontSize(20);

		win = ax::Window::Create(rect);

		win->event.OnPaint = ax::WBind<ax::GC>(this, &VideoPlayer::OnPaint);

		_timer.AddConnection(ax::Event::Timer::TIMER_ID,
			ax::Event::Bind(this, &VideoPlayer::OnTimer));

		_load_drawing_timer.AddConnection(ax::Event::Timer::TIMER_ID,
			ax::Event::Bind(this, &VideoPlayer::LoadingDrawingWorker));

		win->AddConnection(
			8983, ax::Event::Bind(this, &VideoPlayer::OnDoneLoadingVideo));
	}

	void Open(const std::string& video_path)
	{
		_has_video = false;
		_is_loading = true;
		_is_playing = false;
		_is_loading = false;

		_load_drawing_timer.StartTimer(ax::Event::Timer::TimeMs(100));

		win->Update();

		_video_load_thread = std::thread(
			std::bind(&VideoPlayer::OpenWorker, this, std::placeholders::_1),
			video_path);
	}

	void Play()
	{
		if (_is_loading) {
			return;
		}

		_video_mutex.lock();
		if (!_video.isOpened()) {
			_video_mutex.unlock();
			return;
		}
		_video_mutex.unlock();

		if (_timer.IsRunning()) {
			return;
		}

		_is_pause = false;

		win->PushEvent(9000, new ax::Event::SimpleMsg<int>(0));

		_video_mutex.lock();
		long fps_ms = 1000.0 / _video.get(CV_CAP_PROP_FPS);
		_timer.StartTimer(ax::Event::Timer::TimeMs(fps_ms));
		_video_mutex.unlock();
	}

	void Stop()
	{
		_timer.StopTimer();
		_is_playing = false;
		_is_pause = false;
		_video_mutex.lock();
		_video.set(CV_CAP_PROP_POS_FRAMES, 0);
		_video_mutex.unlock();
		win->Update();
	}

	void Pause()
	{
		_timer.StopTimer();
		_is_playing = false;
		_is_pause = true;
		win->Update();
	}

private:
	cv::VideoCapture _video;
	std::mutex _video_mutex;
	std::thread _video_load_thread;
	ax::Size _video_size;
	std::shared_ptr<ax::Image> _img;
	ax::Event::Timer _timer, _load_drawing_timer;
	ax::Font _font;
	ax::Color _loading_color;
	std::default_random_engine _random_generator;
	std::uniform_real_distribution<float> _distribution;

	bool _is_loading, _has_video, _is_playing, _is_pause;

	void OnDoneLoadingVideo(ax::Event::Msg* msg)
	{
		if (_video_load_thread.joinable()) {
			_video_load_thread.join();
			_video_load_thread = std::thread();
		}
	}

	void OpenWorker(std::string video_path)
	{
		_video_mutex.lock();
		_is_loading = true;
		win->Update();
		_video.open(video_path);

		if (!_video.isOpened()) {
			ax::Error("Video didn't load.");
		}

		_video_size = ax::Size(_video.get(CV_CAP_PROP_FRAME_WIDTH),
			_video.get(CV_CAP_PROP_FRAME_HEIGHT));

		_has_video = true;
		_is_loading = false;
		win->Update();
		_video_mutex.unlock();

		_load_drawing_timer.StopTimer();

		win->PushEvent(8983, new ax::Event::SimpleMsg<int>(0));
	}

	void OnTimer(ax::Event::Msg* msg)
	{
		_video_mutex.lock();
		if (!_video.isOpened()) {
			return;
		}

		// Optain next frame.
		cv::Mat frame;
		_video >> frame;
		_video_mutex.unlock();

		// Video end has no size.
		if (!frame.size[0] || !frame.size[1]) {
			return;
		}

		// Convert to rgb.
		cv::Mat rgb_frame;
		cvtColor(frame, rgb_frame, CV_BGR2RGB);

		_img = std::shared_ptr<ax::Image>(new ax::Image(
			rgb_frame.data, _video_size, ax::Image::ColorType::RGB));

		_is_playing = true;

		win->Update();
	}

	void LoadingDrawingWorker(ax::Event::Msg* msg)
	{
		float r = _distribution(_random_generator);
		float g = _distribution(_random_generator);
		float b = _distribution(_random_generator);

		_loading_color = ax::Color(r, g, b);
		win->Update();
	}

	void DrawLoading(ax::GC gc)
	{
		gc.SetColor(ax::Color(_loading_color));
		gc.DrawString(_font, "Video loading ...", ax::Point(50, 50));
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.0f));
		gc.DrawRectangle(rect);

		if (_is_loading) {
			DrawLoading(gc);
			return;
		}

		if (!_has_video) {
			gc.SetColor(ax::Color(1.0f));
			gc.DrawString(_font, "No video loaded", ax::Point(50, 50));
			return;
		}

		if (!_is_playing && !_is_pause) {
			return;
		}

		if (_img) {
			gc.DrawImageResize(
				_img.get(), rect.position, rect.size, 1.0f, true);
		}
	}
};
}

extern "C" {
ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
{
	ax::Size fixe_size(600, 500);
	
	eos::Frame* frame
		= new eos::Frame(ax::Rect(rect.position, fixe_size), "Terminal");
	frame->GetWindow()->property.AddProperty("Resizable");

	ax::Window::Ptr win(ax::Window::Create(frame->GetChildRect()));
	ax::Size size(frame->GetChildRect().size);
//	app.AddTopLevel(win);

	win->event.OnPaint = ax::WFunc<ax::GC>([win](ax::GC gc) {
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.98));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.0f));
		gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
	});

	std::shared_ptr<eos::VideoPlayer> player(
		new eos::VideoPlayer(ax::Rect(ax::Point(0, 0), size)));

	win->node.Add(player);

	ax::Window::Ptr ctrl_bar(
		ax::Window::Create(ax::Rect(10, 0, size.x - 20, 30)));

	win->node.Add(ctrl_bar);

	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.3f, 0.2f);
	btn_info.hover = ax::Color(0.5f, 0.6f);
	btn_info.clicking = ax::Color(0.45f, 0.6f);
	btn_info.selected = ax::Color(0.3f, 0.2f);
	btn_info.contour = ax::Color(0.3f, 0.0f);
	btn_info.font_color = ax::Color(0.8f, 0.7f);

	int btn_space(0);

	ax::Rect btn_rect(0, 0, 60, 20);

	ax::Window::Ptr open_btn = ctrl_bar->node.Add(ax::Button::Create(
		btn_rect, ax::Event::Function([player](ax::Event::Msg* msg) {
			std::string v_path = ax::App::GetInstance().OpenFileDialog();
			ax::Print(v_path);
			player->Open(v_path);
		}),
		"Open", "", btn_info));

	btn_rect.position
		= open_btn->dimension.GetRect().GetNextPosRight(btn_space);

	ax::Window::Ptr play_btn = ctrl_bar->node.Add(ax::Button::Create(btn_rect,
		ax::Event::Function([player](ax::Event::Msg* msg) { player->Play(); }),
		"Play", "", btn_info));

	btn_rect.position
		= play_btn->dimension.GetRect().GetNextPosRight(btn_space);

	ax::Window::Ptr stop_btn = ctrl_bar->node.Add(ax::Button::Create(btn_rect,
		ax::Event::Function([player](ax::Event::Msg* msg) { player->Stop(); }),
		"Stop", "", btn_info));

	btn_rect.position
		= stop_btn->dimension.GetRect().GetNextPosRight(btn_space);

	ax::Window::Ptr pause_btn = ctrl_bar->node.Add(ax::Button::Create(btn_rect,
		ax::Event::Function([player](ax::Event::Msg* msg) { player->Pause(); }),
		"Pause", "", btn_info));

//	std::shared_ptr<MainWindow> win(new MainWindow(frame->GetChildRect()));

	frame->GetWindow()->node.Add(win);
	frame->SetChildHandler(win);

	return frame;
}
}

int main()
{
	ax::App& app = ax::App::GetInstance();

	app.AddMainEntry([&app]() {
		ax::Size size(600, 500);
		app.SetFrameSize(size);

		ax::Window::Ptr win(ax::Window::Create(ax::Rect(0, 0, 600, 500)));
		app.AddTopLevel(win);

		win->event.OnPaint = ax::WFunc<ax::GC>([win](ax::GC gc) {
			ax::Rect rect(win->dimension.GetDrawingRect());
			gc.SetColor(ax::Color(0.98));
			gc.DrawRectangle(rect);

			gc.SetColor(ax::Color(0.0f));
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
		});

		std::shared_ptr<eos::VideoPlayer> player(
			new eos::VideoPlayer(ax::Rect(0, 0, 600, 500)));

		win->node.Add(player);

		ax::Window::Ptr ctrl_bar(
			ax::Window::Create(ax::Rect(10, 0, size.x - 20, 30)));

		win->node.Add(ctrl_bar);

		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.3f, 0.2f);
		btn_info.hover = ax::Color(0.5f, 0.6f);
		btn_info.clicking = ax::Color(0.45f, 0.6f);
		btn_info.selected = ax::Color(0.3f, 0.2f);
		btn_info.contour = ax::Color(0.3f, 0.0f);
		btn_info.font_color = ax::Color(0.8f, 0.7f);

		int btn_space(0);

		ax::Rect btn_rect(0, 0, 60, 20);

		ax::Window::Ptr open_btn = ctrl_bar->node.Add(ax::Button::Create(
			btn_rect, ax::Event::Function([player](ax::Event::Msg* msg) {
				std::string v_path = ax::App::GetInstance().OpenFileDialog();
				ax::Print(v_path);
				player->Open(v_path);
			}),
			"Open", "", btn_info));

		btn_rect.position
			= open_btn->dimension.GetRect().GetNextPosRight(btn_space);

		ax::Window::Ptr play_btn = ctrl_bar->node.Add(ax::Button::Create(
			btn_rect, ax::Event::Function(
						  [player](ax::Event::Msg* msg) { player->Play(); }),
			"Play", "", btn_info));

		btn_rect.position
			= play_btn->dimension.GetRect().GetNextPosRight(btn_space);

		ax::Window::Ptr stop_btn = ctrl_bar->node.Add(ax::Button::Create(
			btn_rect, ax::Event::Function(
						  [player](ax::Event::Msg* msg) { player->Stop(); }),
			"Stop", "", btn_info));

		btn_rect.position
			= stop_btn->dimension.GetRect().GetNextPosRight(btn_space);

		ax::Window::Ptr pause_btn = ctrl_bar->node.Add(ax::Button::Create(
			btn_rect, ax::Event::Function(
						  [player](ax::Event::Msg* msg) { player->Pause(); }),
			"Pause", "", btn_info));
	});

	app.MainLoop();

	return 0;
}
