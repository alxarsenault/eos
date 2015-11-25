#ifndef __CTRL_LINE_FOLLOWER_H__
#define __CTRL_LINE_FOLLOWER_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"

namespace ctrl {
	class LineFollower : public ax::Window::Backbone {
	public:
		LineFollower(const ax::Rect& rect);
		
	private:
		std::vector<ax::Point> _points;
		std::vector<double> _length_vec;
		
		ax::Image::Ptr _round_img;
		std::shared_ptr<ax::Event::Timer> _play_timer;

		double _length, _play_length;
		
		void OnMouseLeftDown(const ax::Point& mouse);
		
		void OnMouseLeftUp(const ax::Point& mouse);
		
		void OnMouseLeftDragging(const ax::Point& mouse);
		
		void OnPaint(ax::GC gc);
	};
}

#endif // __CTRL_LINE_FOLLOWER_H__
