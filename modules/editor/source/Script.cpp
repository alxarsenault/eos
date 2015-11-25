#include "Script.h"
#include "axLib/axLib.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

std::vector<chaiscript::ModulePtr> Script::_modules;

Script::Script()
{
	_chai = std::shared_ptr<chaiscript::ChaiScript>(
		new chaiscript::ChaiScript(chaiscript::Std_Lib::library()));

	// Create all axFrameworks modules.
	if (!_modules.size()) {
		CreateModules();
	}

	// Assign modules to script interpreter.
	for (auto& n : _modules) {
		_chai->add(n);
	}
}

void Script::ProcessFile(const std::string& file_path)
{
	_chai->eval_file(file_path);
}

void Script::ProcessString(const std::string& content)
{
	_chai->eval(content);
}

void Script::AddWindowHandle(ax::Window::Ptr win)
{
	_chai->add(chaiscript::var(win), "win");
}

void Script::CreateModules()
{
	// ax::Var.
	chaiscript::ModulePtr m_var
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_var->add(chaiscript::user_type<ax::Var>(), "Var");
	m_var->add(chaiscript::fun([](ax::Var& c1, ax::Var& c2) { c1 = c2; }), "=");
	m_var->add(chaiscript::fun(&ax::Var::as<ax::Color>), "as_color");
	//	chai->add(m_var);
	_modules.push_back(m_var);

	// ax::Point.
	chaiscript::ModulePtr m_point
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_point->add(chaiscript::user_type<ax::Point>(), "Point");
	m_point->add(chaiscript::constructor<ax::Point(int, int)>(), "Point");
	m_point->add(chaiscript::fun([](ax::Var& c) {
					 if (c.is<ax::Point>()) {
						 return c.as<ax::Point>();
					 }
					 return ax::Point(0, 0);
				 }),
		"Point");

	m_point->add(chaiscript::fun(&ax::Point::x), "x");
	m_point->add(chaiscript::fun(&ax::Point::y), "y");

	_modules.push_back(m_point);

	// ax::Size.
	chaiscript::ModulePtr m_size
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_size->add(chaiscript::user_type<ax::Size>(), "Size");
	m_size->add(chaiscript::constructor<ax::Size(int, int)>(), "Size");
	m_size->add(chaiscript::fun(&ax::Size::x), "x");
	m_size->add(chaiscript::fun(&ax::Size::y), "y");

	_modules.push_back(m_size);

	// ax::Rect.
	chaiscript::ModulePtr m_rect
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_rect->add(chaiscript::user_type<ax::Rect>(), "Rect");
	m_rect->add(
		chaiscript::constructor<ax::Rect(int, int, int, int)>(), "Rect");
	m_rect->add(
		chaiscript::constructor<ax::Rect(ax::Point, ax::Size)>(), "Rect");
	m_rect->add(chaiscript::fun(&ax::Rect::position), "position");
	m_rect->add(chaiscript::fun(&ax::Rect::size), "size");

	_modules.push_back(m_rect);

	// ax::Color.
	chaiscript::ModulePtr m_color
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_color->add(chaiscript::user_type<ax::Color>(), "Color");
	m_color->add(chaiscript::constructor<ax::Color(
					 const float&, const float&, const float&)>(),
		"Color");
	m_color->add(chaiscript::constructor<ax::Color(
					 const float&, const float&, const float&, const float&)>(),
		"Color");
	m_color->add(
		chaiscript::fun([](ax::Color& c1, const ax::Color& c2) { c1 = c2; }),
		"=");
	_modules.push_back(m_color);

	// ax::Window::Dimension.
	chaiscript::ModulePtr m_dimension
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_dimension->add(chaiscript::user_type<ax::Window::Ptr>(), "Dimension");
	m_dimension->add(
		chaiscript::fun(&ax::Window::Dimension::GetRect), "GetRect");
	m_dimension->add(chaiscript::fun(&ax::Window::Dimension::GetDrawingRect),
		"GetDrawingRect");
	_modules.push_back(m_dimension);

	// ax::Window::Event.
	chaiscript::ModulePtr m_event
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_event->add(chaiscript::user_type<ax::Window::Event>(), "Event");
	m_event->add(
		chaiscript::fun([](ax::Window::Event* evt) { return evt; }), "Event");

	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
					 std::function<void(const char&)> fct) {
					 evt->OnKeyDown = ax::WFunc<char>(fct);
				 }),
		"OnKeyDown");

	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
					 std::function<void(ax::Window::Ptr, ax::GC)> fct) {
					 evt->OnPaint = ax::WFunc<ax::GC>(
						 [evt, fct](ax::GC gc) { fct(evt->GetWindow(), gc); });
				 }),
		"OnPaint");

	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
					 std::function<void(ax::Window::Ptr, ax::Point)> fct) {
					 evt->OnMouseEnter = ax::WFunc<ax::Point>([evt, fct](
						 const ax::Point& pos) { fct(evt->GetWindow(), pos); });
				 }),
		"OnMouseEnter");

	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
					 std::function<void(ax::Window::Ptr, ax::Point)> fct) {
					 evt->OnMouseLeave = ax::WFunc<ax::Point>([evt, fct](
						 const ax::Point& pos) { fct(evt->GetWindow(), pos); });
				 }),
		"OnMouseLeave");

	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
					 std::function<void(ax::Window::Ptr, ax::Point)> fct) {
					 evt->OnMouseMotion = ax::WFunc<ax::Point>(
						 [evt, fct](const ax::Point& pos) {
							 fct(evt->GetWindow(), pos
									 - evt->GetWindow()
										   ->dimension.GetAbsoluteRect()
										   .position);
						 });
				 }),
		"OnMouseMotion");

	//	m_event->add(chaiscript::fun([](ax::Window::Event* evt,
	//std::function<void(ax::GC)> fct) {
	//		evt->OnPaint = ax::WFunc<ax::GC>(fct); }), "OnPaint");
	_modules.push_back(m_event);

	// ax::Window::Ptr.
	chaiscript::ModulePtr m_window
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_window->add(chaiscript::user_type<ax::Window::Ptr>(), "Window");
	m_window->add(chaiscript::fun(&ax::Window::Create), "Window");
	m_window->add(chaiscript::fun(&ax::Window::dimension), "dimension");
	m_window->add(chaiscript::fun(&ax::Window::event), "event");
	m_window->add(chaiscript::fun(&ax::Window::Update), "Update");
	m_window->add(chaiscript::fun([](ax::Window::Ptr win,
					  ax::Window::Ptr child) { win->node.Add(child); }),
		"Add");
	m_window->add(chaiscript::fun([](ax::Window::Ptr win,
					  std::shared_ptr<ax::Window::Backbone> child) {
					  win->node.Add(child);
				  }),
		"Add");

	m_window->add(
		chaiscript::fun([](ax::Window::Ptr win, const std::string& name,
			ax::Color value) { win->resource.Add(name, value); }),
		"Assign");

	m_window->add(
		chaiscript::fun([](ax::Window::Ptr win, const std::string& name,
			ax::Point value) { win->resource.Add(name, value); }),
		"Assign");

	m_window->add(
		chaiscript::fun([](ax::Window::Ptr win, const std::string& name,
			ax::Rect value) { win->resource.Add(name, value); }),
		"Assign");

	m_window->add(
		chaiscript::fun([](ax::Window::Ptr win, const std::string& name) {
			return win->resource.GetResource(name);
		}),
		"[]");

	_modules.push_back(m_window);

	// ax::GC.
	chaiscript::ModulePtr m_gc
		= chaiscript::ModulePtr(new chaiscript::Module());
	m_gc->add(chaiscript::user_type<ax::GC>(), "GC");
	m_gc->add(chaiscript::constructor<ax::GC()>(), "GC");
	m_gc->add(chaiscript::fun([](
				  ax::GC* gc, const ax::Color& color) { gc->SetColor(color); }),
		"Color");

	m_gc->add(chaiscript::fun([](ax::GC* gc, ax::Var& c) {
				  if (c.is<ax::Color>()) {
					  gc->SetColor(c.as<ax::Color>());
				  }
			  }),
		"Color");

	m_gc->add(chaiscript::fun(&ax::GC::DrawRectangle), "Rectangle");
	m_gc->add(
		chaiscript::fun(&ax::GC::DrawRectangleContour), "RectangleContour");
	m_gc->add(chaiscript::fun(&ax::GC::DrawLineColorfade), "LineColorfade");
	m_gc->add(chaiscript::fun(&ax::GC::DrawLine), "Line");

	//	m_gc->add(chaiscript::fun<void (ax::GC::*)(const ax::Point&, const
	//ax::Point&, float)>(&ax::GC::DrawLine), "DrawLine");
	_modules.push_back(m_gc);
}