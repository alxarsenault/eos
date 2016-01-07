#ifndef __EOS_FRAME_H__
#define __EOS_FRAME_H__

#include "axLib/axButton.h"
#include "axLib/axLib.h"

namespace eos {
/***************************************************************************
 * eos::Frame.
 **************************************************************************/
class Frame : public ax::Window::Backbone {
public:
	class Info {
	public:
		Info()
		{
		}
	};

	/***********************************************************************
	 * eos::Frame::Msg
	 **********************************************************************/
	class Msg : public ax::Event::Msg {
	public:
		Msg();

		Msg(Frame* sender);

		Frame* GetSender() const;

		ax::Event::Msg* GetCopy();

	private:
		Frame* _sender;
	};

	class Events {
	public:
		enum : ax::Event::Id { MINIMIZE, CLOSE, MOVE, FULL_SCREEN, RESIZE };

		Events()
		{
		}
	};

	Frame(const ax::Rect& rect, const std::string& window_name);

	void SetFullScreen(const ax::Rect& rect);
	void UnSetFullScreen(const ax::Rect& rect);

	ax::Rect GetChildRect() const;

	static ax::Size GetFrameSizeFromChildSize(const ax::Size& size)
	{
		int s_w = 4;
		return ax::Size(size.x + 2 * s_w + 1, size.y + _title_bar_height + s_w);
	}

	void SetChildHandler(ax::Window::Ptr child);

	void SetChildMenuHandler(ax::Window::Ptr menu);

	std::string GetAppName() const;

	// Property.
	// Resizable

private:
	static const int _title_bar_height;

	bool _menuMode;
	bool _isFullScreen;

	ax::Window::Ptr _child;
	ax::Window::Ptr _child_menu;

	std::string _window_name;
	ax::Button::Ptr _close_btn, _min_btn, _fscreen_btn;
	ax::Button::Ptr _menu_btn;

	ax::Size _click_size;
	int _frame_status;
	ax::Font _font;
	ax::Point _click_pos, _abs_click_pos;
	;
	bool _highlight;

	ax::Image* _bg_img;

	axEVENT_ACCESSOR(ax::Button::Msg, OnOpenMenu);
	void OnOpenMenu(const ax::Button::Msg& msg);

	axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
	void OnButtonClick(const ax::Button::Msg& msg);

	axEVENT_ACCESSOR(ax::Button::Msg, OnMinimize);
	void OnMinimize(const ax::Button::Msg& msg);

	axEVENT_ACCESSOR(ax::Button::Msg, OnFullScreen);
	void OnFullScreen(const ax::Button::Msg& msg);

	void OnMouseLeave(const ax::Point& pos);

	void OnMouseMotion(const ax::Point& pos);

	void OnMouseLeftDown(const ax::Point& pos);

	void OnMouseLeftUp(const ax::Point& pos);

	void OnMouseLeftDragging(const ax::Point& pos);

	void OnPaint(ax::GC gc);
	void OnPaintOverFrameBuffer(ax::GC gc);
	void DrawMenuMode(ax::GC& gc);

	void DrawQuarterCircle(ax::GC gc, const ax::FloatPoint& pos, const int& radius, const double& angle,
		const ax::Color& middle_color, const ax::Color& contour_color);
};
}

#endif //__EOS_FRAME_H__
