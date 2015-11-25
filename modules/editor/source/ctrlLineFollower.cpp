#include "ctrlLineFollower.hpp"
#include "axLib/axButton.h"

namespace ctrl {
LineFollower::LineFollower(const ax::Rect& rect)
{
	win = ax::Window::Create(rect);

	win->event.OnPaint = ax::WBind<ax::GC>(this, &LineFollower::OnPaint);
	win->event.OnMouseLeftDown
		= ax::WBind<ax::Point>(this, &LineFollower::OnMouseLeftDown);
	win->event.OnMouseLeftUp
		= ax::WBind<ax::Point>(this, &LineFollower::OnMouseLeftUp);
	win->event.OnMouseLeftDragging
		= ax::WBind<ax::Point>(this, &LineFollower::OnMouseLeftDragging);

	_round_img = ax::Image::Ptr(new ax::Image("round.png"));

	_play_timer = std::shared_ptr<ax::Event::Timer>(new ax::Event::Timer(
		ax::App::GetInstance().GetEventManager(),
		ax::Event::Function([&](ax::Event::Msg* msg) {

			ax::Event::Timer::Msg* tmsg
				= static_cast<ax::Event::Timer::Msg*>(msg);

			_play_length = (tmsg->GetTime().count() / (3000.0 - 100)) * _length;

			if (tmsg->GetTime() >= ax::Event::Timer::TimeMs(3000 - 100)) {
				_play_timer->StopTimer();
				ax::Print("STOP TIMER");
			}

			win->Update();
		})));

	win->node.Add(ax::Button::Create(
		ax::Point(0, 0), ax::Event::Function([&](ax::Event::Msg* msg) {
			_play_length = 0.0;
			_play_timer->StartTimer(
				ax::Event::Timer::TimeMs(100), ax::Event::Timer::TimeMs(3000));
		})));
}

ax::Point HermiteInterpole(const ax::Point& p0, const ax::Point& p1,
	const ax::Point& p2, const ax::Point& p3, const double& mu)
{
	double fu[4];

	GLfloat u = mu;
	GLfloat u2 = u * u;
	GLfloat u3 = u2 * u;
	fu[0] = 2.0 * u3 - 3.0 * u2 + 1.0;
	fu[1] = -2.0 * u3 + 3.0 * u2;
	fu[2] = u3 - 2.0 * u2 + u;
	fu[3] = u3 - u2;

	GLfloat x = 0.0;
	GLfloat y = 0.0;

	// p0 = ctrlPoints[0]
	x += fu[0] * p0.x;
	y += fu[0] * p0.y;

	// p1 = ctrlPoints[3]
	x += fu[1] * p3.x;
	y += fu[1] * p3.y;

	//  tangent at p0 = ctrlPoints[1]-ctrlPoints[0]
	x += fu[2] * (3.0 * (p1.x - p0.x));
	y += fu[2] * (3.0 * (p1.y - p0.y));

	//  tangent at p1 = ctrlPoints[3]-ctrlPoints[2]
	x += fu[3] * (6.0 * (p3.x - p2.x));
	y += fu[3] * (6.0 * (p3.y - p2.y));

	return ax::Point(x, y);
}

void LineFollower::OnMouseLeftDown(const ax::Point& mouse)
{
	ax::Rect rect(win->dimension.GetAbsoluteRect());
	ax::Point pos(mouse - rect.position);
	win->event.GrabMouse();
	_points.clear();
	_length_vec.clear();

	_points.push_back(pos);
	
	win->Update();
}

void LineFollower::OnMouseLeftUp(const ax::Point& mouse)
{
	if (win->event.IsGrabbed()) {
		win->event.UnGrabMouse();

		_length = 0;
		_length_vec.push_back(0.0);
		
		for (int i = 0; i < _points.size() - 1; i++) {
			double x = _points[i + 1].x - _points[i].x;
			double y = _points[i + 1].y - _points[i].y;
			_length += sqrt(x * x + y * y);
			_length_vec.push_back(_length);
		}

		win->Update();
	}
}

void LineFollower::OnMouseLeftDragging(const ax::Point& mouse)
{
	ax::Rect rect(win->dimension.GetAbsoluteRect());
	ax::Point pos(mouse - rect.position);

	_points.push_back(pos);
	win->Update();
}

std::vector<ax::FloatPoint> DrawThickLine(const ax::Point& p0_i,
	const ax::Point& p1_i, const ax::Point& p2_i, const float& thickness,
	std::vector<ax::FloatPoint> begin = std::vector<ax::FloatPoint>())
{
	std::function<ax::FloatPoint(ax::FloatPoint)> normalize(
		[](ax::FloatPoint point) {
			ax::FloatPoint pt(point);
			float norm = sqrt(pt.x * pt.x + pt.y * pt.y);
			pt.x /= norm;
			pt.y /= norm;
			return pt;
		});

	ax::FloatPoint p0(p0_i.x, p0_i.y);
	ax::FloatPoint p1(p1_i.x, p1_i.y);

	ax::FloatPoint p2(p2_i.x, p2_i.y);
	//		ax::FloatPoint p3(p3_i.x, p3_i.y);

	ax::FloatPoint line = p1 - p0;
	ax::FloatPoint normal(-line.y, line.x);
	float norm = sqrt(normal.x * normal.x + normal.y * normal.y);
	normal.x /= norm;
	normal.y /= norm;

	//		Vec2f normal = Vec2f( -line.y, line.x).normalized();
	ax::FloatPoint a = p0 - (normal * thickness);
	ax::FloatPoint b = p0 + (normal * thickness);
	ax::FloatPoint c = p1 - (normal * thickness);
	ax::FloatPoint d = p1 + (normal * thickness);

	if (begin.size() == 2) {
		a = begin[0];
		b = begin[1];
	}

	ax::FloatPoint tangent = normalize(normalize(p2 - p1) + normalize(p1 - p0));
	ax::FloatPoint miter(-tangent.y, tangent.x);
	float miter_dot = miter.x * normal.x + miter.y * normal.y;
	float length = thickness / miter_dot;

	if (p2 != p1) {
		c = p1 - miter * length;
		d = p1 + miter * length;
	}

	{
		ax::Utils::RectPoints<ax::FloatPoint> points = { p0, b, d, p1 };
		ax::Color c0(1.0f, 0.0f, 0.0f, 0.8f);
		ax::Color c1(1.0f, 0.0f, 0.0f, 0.0f);
		ax::Color colors[4] = { c0, c1, c1, c0 };

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &points);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	{
		ax::Utils::RectPoints<ax::FloatPoint> points = { a, p0, p1, c };
		ax::Color c0(1.0f, 0.0f, 0.0f, 0.0f);
		ax::Color c1(1.0f, 0.0f, 0.0f, 0.8f);
		ax::Color colors[4] = { c0, c1, c1, c0 };

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &points);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	std::vector<ax::FloatPoint> ret = { c, d };
	return ret;
}

void LineFollower::OnPaint(ax::GC gc)
{
	const ax::Rect& rect(win->dimension.GetDrawingRect());

	gc.SetColor(ax::Color(0.35));
	gc.DrawRectangle(rect);

	gc.SetColor(ax::Color(0.39));
	int _grid_space = 20;

	// Vertical lines.
	for (int x = _grid_space; x < rect.size.x; x += _grid_space) {
		gc.DrawLine(ax::Point(x, 0), ax::Point(x, rect.size.y));
	}

	// Horizontal lines.
	for (int y = _grid_space; y < rect.size.y; y += _grid_space) {
		gc.DrawLine(ax::Point(0, y), ax::Point(rect.size.x, y));
	}

	// Draw line.
	if (_points.size() > 3) {
		float thickness = 4.4;
		gc.SetColor(ax::Color(255, 0, 0));
		std::vector<ax::FloatPoint> p_last;

		for (int i = 1; i < _points.size() - 1; i++) {
			p_last = DrawThickLine(_points[i - 1], _points[i],
				_points[i + 1], thickness, p_last);
		}
	}

	if (_play_timer->IsRunning()) {

		ax::Point play_pos;

		for (int i = 0; i < _length_vec.size() - 1; i++) {
			if (_play_length < _length_vec[i]) {

				double mu = (_length_vec[i] - _play_length)
					/ (_length_vec[i] - _length_vec[i - 1]);
				
				
				double x = ax::Utils::Interpole::Lineair<double>(
					_points[i - 1].x, _points[i].x, 1.0 - mu);
				double y = ax::Utils::Interpole::Lineair<double>(
					_points[i - 1].y, _points[i].y, 1.0 - mu);
				play_pos = ax::Point(x, y);
				break;
			}
		}

		gc.DrawPartOfImageResize(_round_img.get(), ax::Point(0, 0),
			ax::Size(40, 40),
			ax::Rect(play_pos - ax::Point(10, 10), ax::Size(20, 20)));
	}
}
}