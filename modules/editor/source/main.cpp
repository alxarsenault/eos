#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"
#include "axLib/axKnob.h"
#include "axLib/axWindowManager.h"
#include "axLib/axWindowTree.h"
#include "axLib/axScrollBar.h"
#include "CodeEditor.h"
#include "Script.h"
#include "axLib/rapidxml.hpp"
#include "axLib/axGLShader.h"

#include "ctrlLineFollower.hpp"


#include "eosFrame.h"

#define SHARED_PTR(TYPE, ...) std::shared_ptr<TYPE>(new TYPE(__VA_ARGS__))

ax::Window::Ptr MAIN_WINDOW = nullptr;

class GridWindow : public ax::Window::Backbone {
public:
	GridWindow(const ax::Rect& rect)
		: _grid_space(10)
		, _selection(false, ax::Rect(0, 0, 0, 0))
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &GridWindow::OnPaint);
		win->event.OnMouseLeftDown
			= ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDown);
		win->event.OnMouseLeftDragging
			= ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftDragging);
		win->event.OnMouseLeftUp
			= ax::WBind<ax::Point>(this, &GridWindow::OnMouseLeftUp);
	}

	/// Set number of pixels between each grid lines.
	void SetGridSpace(const int& space)
	{
		if (space > 0 && _grid_space != space) {
			_grid_space = space;
			win->Update();
		}
	}

private:
	int _grid_space;
	std::pair<bool, ax::Rect> _selection;

	void OnMouseLeftDown(const ax::Point& pos)
	{
		ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);

		_selection.first = true;
		_selection.second.position = m_pos;
		_selection.second.size = ax::Size(1, 1);

		win->event.GrabMouse();
		win->Update();
	}

	void OnMouseLeftDragging(const ax::Point& pos)
	{
		ax::Point m_pos(pos - win->dimension.GetAbsoluteRect().position);
		_selection.second.size = m_pos - _selection.second.position;
		win->Update();
	}

	void OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();
			_selection.first = false;
			win->Update();
		}
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		// Background.
		gc.SetColor(ax::Color(0.9));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.8));

		// Vertical lines.
		for (int x = _grid_space; x < rect.size.x; x += _grid_space) {
			gc.DrawLineStripple(ax::Point(x, 0), ax::Point(x, rect.size.y));
		}

		// Horizontal lines.
		for (int y = _grid_space; y < rect.size.y; y += _grid_space) {
			gc.DrawLineStripple(ax::Point(0, y), ax::Point(rect.size.x, y));
		}

		// Selection rectangle.
		if (_selection.first) {
			gc.SetColor(ax::Color(0.7, 0.5));
			gc.DrawRectangle(_selection.second);
			gc.SetColor(ax::Color(0.7, 0.8));
			gc.DrawRectangleContour(_selection.second);
		}

		// Grid contour.
		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
};

class TopMenu : public ax::Window::Backbone {
public:
	TopMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &TopMenu::OnPaint);

		// Transparent toggle with image.
		ax::Toggle::Info tog_info;
		tog_info.normal = ax::Color(0.0, 0.0);
		tog_info.hover = ax::Color(0.0, 0.0);
		tog_info.clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.font_color = ax::Color(0.0, 0.0);

		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_hover = ax::Color(0.0, 0.0);
		tog_info.selected_clicking = ax::Color(0.0, 0.0);
		tog_info.selected = ax::Color(0.0, 0.0);
		tog_info.selected_font_color = ax::Color(0.0, 0.0);

		tog_info.contour = ax::Color(0.0, 0.0);
		tog_info.font_size = 12;
		tog_info.img = "resources/top_menu_toggle_left.png";
		tog_info.single_img = false;

		ax::Point pos(rect.size.x - 95, 2);
		std::shared_ptr<ax::Toggle> tog_left
			= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(25, 25)),
				ax::Toggle::Events(), tog_info, "", "");

		pos = tog_left->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		tog_info.img = "resources/top_menu_toggle_bottom.png";
		std::shared_ptr<ax::Toggle> tog_middle
			= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(25, 25)),
				ax::Toggle::Events(), tog_info, "", "");

		pos = tog_middle->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		tog_info.img = "resources/top_menu_toggle_right.png";
		std::shared_ptr<ax::Toggle> tog_right
			= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(25, 25)),
				ax::Toggle::Events(), tog_info, "", "");

		tog_left->SetSelected(true);
		tog_middle->SetSelected(true);
		tog_right->SetSelected(true);

		win->node.Add(tog_left);
		win->node.Add(tog_middle);
		win->node.Add(tog_right);

		pos = ax::Point(5, 2);

		tog_info.img = "resources/widget_menu.png";
		std::shared_ptr<ax::Toggle> widget_menu
			= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(25, 25)),
				ax::Toggle::Events(), tog_info, "", "");

		win->node.Add(widget_menu);
		pos = widget_menu->GetWindow()->dimension.GetRect().GetNextPosRight(5);

		std::shared_ptr<ax::Toggle> tree_menu
			= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(25, 25)),
				ax::Toggle::Events(), tog_info, "", "");

		win->node.Add(tree_menu);
	}

private:
	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.15));
		//		gc.DrawRectangle(rect);
		gc.DrawRectangleColorFade(rect, ax::Color(0.15), ax::Color(0.18));
		gc.DrawRectangleContour(rect);
	}
};

class WidgetMenuSeparator : public ax::Window::Backbone {
public:
	WidgetMenuSeparator(const ax::Rect& rect, const std::string& name)
		: _font("resources/FreeSansBold.ttf")
		, _name(name)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint
			= ax::WBind<ax::GC>(this, &WidgetMenuSeparator::OnPaint);
		win->event.OnMouseLeftDown
			= ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftDown);
		win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(
			this, &WidgetMenuSeparator::OnMouseLeftDragging);
		win->event.OnMouseLeftUp
			= ax::WBind<ax::Point>(this, &WidgetMenuSeparator::OnMouseLeftUp);
	}

private:
	ax::Font _font;
	std::string _name;

	void OnMouseLeftDown(const ax::Point& pos)
	{
	}

	void OnMouseLeftDragging(const ax::Point& pos)
	{
	}

	void OnMouseLeftUp(const ax::Point& pos)
	{
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.15));
		//		gc.DrawRectangle(rect);
		gc.DrawRectangleColorFade(rect, ax::Color(0.15), ax::Color(0.25));
		gc.DrawRectangleContour(rect);

		gc.SetColor(ax::Color(0.7));
		gc.DrawString(_font, _name, ax::Point(5, 2));
	}
};

class WidgetMenuObj : public ax::Window::Backbone {
public:
	WidgetMenuObj(const ax::Rect& rect, const std::string& name,
		const std::string& title, const std::string& info,
		const std::string& size, const std::string& img_path)
		: _font("resources/FreeSansBold.ttf")
		, _font_normal(0)
		, _name(name)
		, _title(title)
		, _info(info)
		, _size_str(size)
	{
		_font_normal.SetFontSize(11);

		_img = SHARED_PTR(ax::Image, img_path);

		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenuObj::OnPaint);
		win->event.OnMouseLeftDown
			= ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDown);
		win->event.OnMouseLeftDragging
			= ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftDragging);
		win->event.OnMouseLeftUp
			= ax::WBind<ax::Point>(this, &WidgetMenuObj::OnMouseLeftUp);
	}

private:
	ax::Font _font;
	ax::Font _font_normal;
	std::string _name, _title, _info, _size_str;
	std::shared_ptr<ax::Image> _img;

	void OnMouseLeftDown(const ax::Point& pos)
	{
		win->event.GrabMouse();
		MAIN_WINDOW->PushEvent(
			8000, new ax::Event::SimpleMsg<std::pair<std::string, ax::Point>>(
					  std::pair<std::string, ax::Point>(_name, pos)));

		win->Update();
	}

	void OnMouseLeftDragging(const ax::Point& pos)
	{
		MAIN_WINDOW->PushEvent(
			8001, new ax::Event::SimpleMsg<ax::Point>(pos));
	}

	void OnMouseLeftUp(const ax::Point& pos)
	{
		if (win->event.IsGrabbed()) {
			win->event.UnGrabMouse();
			MAIN_WINDOW->PushEvent(
				8002, new ax::Event::SimpleMsg<ax::Point>(pos));
			win->Update();
		}
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.85));
		//		gc.DrawRectangle(rect);
		gc.DrawRectangleColorFade(rect, ax::Color(0.85), ax::Color(0.86));

		ax::Size img_size(_img->GetSize());
		ax::Point img_pos(
			5 + (65 - img_size.x) / 2, 5 + (rect.size.y - 8 - img_size.y) / 2);
		gc.DrawImage(_img.get(), img_pos);

		gc.SetColor(ax::Color(0.2));
		gc.DrawString(_font, _title, ax::Point(75, 6));

		gc.SetColor(ax::Color(0.2));
		gc.DrawString(_font_normal, _info, ax::Point(75, 20));
		gc.DrawString(_font_normal, _size_str, ax::Point(75, 32));

		gc.SetColor(ax::Color(0.5));
		gc.DrawRectangleContour(rect);
	}
};

class WidgetMenu : public ax::Window::Backbone {
public:
	WidgetMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &WidgetMenu::OnPaint);

		_panel = ax::Window::Create(
			ax::Rect(ax::Point(1, 1), rect.size - ax::Size(2, 2)));
		win->node.Add(_panel);

		ax::Point pos(0, 0);
		ax::Size size(rect.size.x, 50);
		ax::Size separator_size(rect.size.x, 20);

		// Read the xml file into a vector.
		std::ifstream menu_file_path("resources/widget_menu.xml");
		std::vector<char> xml_buffer(
			(std::istreambuf_iterator<char>(menu_file_path)),
			std::istreambuf_iterator<char>());
		xml_buffer.push_back('\0');

		rapidxml::xml_document<> doc;

		try {
			// Parse the buffer using the xml file parsing library into doc.
			doc.parse<0>(&xml_buffer[0]);

			rapidxml::xml_node<>* root_node = doc.first_node("WidgetMenu");

			if (root_node) {
				rapidxml::xml_node<>* node = root_node->first_node();

				while (node) {
					std::string node_name(node->name(), node->name_size());

					if (node_name == "separator") {
						rapidxml::xml_attribute<>* att
							= node->first_attribute("name");
						if (att) {
							std::string separator_name(
								att->value(), att->value_size());
							ax::Rect sep_rect(pos, separator_size);
							pos = _panel->node.Add(
												  SHARED_PTR(
													  WidgetMenuSeparator,
													  sep_rect, separator_name))
									  ->dimension.GetRect()
									  .GetNextPosDown(0);
						}
					}
					else if (node_name == "widget") {
						rapidxml::xml_attribute<>* att
							= node->first_attribute("id");
						std::string widget_id(att->value(), att->value_size());

						att = node->first_attribute("label");
						std::string widget_label(
							att->value(), att->value_size());

						att = node->first_attribute("description");
						std::string widget_desc(
							att->value(), att->value_size());

						att = node->first_attribute("size");
						std::string widget_size(
							att->value(), att->value_size());

						att = node->first_attribute("img");
						std::string widget_img(att->value(), att->value_size());

						pos = _panel->node.Add(SHARED_PTR(WidgetMenuObj,
												   ax::Rect(pos, size),
												   widget_id, widget_label,
												   widget_desc, widget_size,
												   widget_img))
								  ->dimension.GetRect()
								  .GetNextPosDown(0);
					}

					node = node->next_sibling();
				}
			}
		}
		catch (rapidxml::parse_error err) {
			ax::Error("Widget menu xml", err.what());
		}

		ax::ScrollBar::Info sInfo;
		sInfo.normal = ax::Color(0.22);
		sInfo.hover = ax::Color(0.23);
		sInfo.clicking = ax::Color(0.21);
		sInfo.slider_contour = ax::Color(0.0, 0.2);
		sInfo.contour = ax::Color(0.0, 0.0);
		sInfo.bg_top = ax::Color(0.8, 0.2);
		sInfo.bg_bottom = ax::Color(0.82, 0.2);

		ax::Rect sRect(rect.size.x - 9, 0, 10, rect.size.y - 1);
		_scrollBar
			= SHARED_PTR(ax::ScrollBar, sRect, ax::ScrollBar::Events(), sInfo);

		win->node.Add(_scrollBar);

		_panel->property.AddProperty("BlockDrawing");
		_panel->dimension.SetSizeNoShowRect(ax::Size(rect.size.x, pos.y));

		_scrollBar->SetWindowHandle(_panel.get());
		_scrollBar->UpdateWindowSize(ax::Size(rect.size.x, pos.y));
	}

private:
	std::shared_ptr<WidgetMenuObj> _objs; // Fuckup for now.
	ax::Window::Ptr _panel;
	ax::ScrollBar::Ptr _scrollBar;

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.45));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
};

class InfoMenu : public ax::Window::Backbone {
public:
	InfoMenu(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &InfoMenu::OnPaint);

		ax::Point pos(0, 0);
		ax::Size size(rect.size.x, 50);
		ax::Size separator_size(rect.size.x, 20);

		pos = win->node.Add(SHARED_PTR(WidgetMenuSeparator,
								ax::Rect(pos, separator_size), "ax::Window"))
				  ->dimension.GetRect()
				  .GetNextPosDown(0);
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());

		gc.SetColor(ax::Color(0.85));
		gc.DrawRectangle(rect);

		gc.SetColor(ax::Color(0.7));
		gc.DrawRectangleContour(rect);
	}
};

class MainWindow : public ax::Window::Backbone {
public:
	MainWindow(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &MainWindow::OnPaint);
		win->event.OnResize = ax::WBind<ax::Size>(this, &MainWindow::OnResize);

		// Create top menu.
		ax::Rect top_menu_rect(0, 0, rect.size.x, 30);
		win->node.Add(_topMenu = SHARED_PTR(TopMenu, top_menu_rect));

		// Create widget menu.
		ax::Rect widget_menu_rect(0, 30, 250, rect.size.y - 30 - 18);
		win->node.Add(_widgetMenu = SHARED_PTR(WidgetMenu, widget_menu_rect));

		// Create info menu.
		ax::Rect info_rect(rect.size.x - 200, 30, 200, rect.size.y - 30 - 18);
		win->node.Add(_infoMenu = SHARED_PTR(InfoMenu, info_rect));

		// Create grid window.
		ax::Rect grid_rect(
			250, 30, rect.size.x - 250 - 200, rect.size.y - 30 - 200 - 18);
		win->node.Add(_gridWindow = SHARED_PTR(GridWindow, grid_rect));

		// Create code editor.
		TextEditor::Info txt_info;
		txt_info.bg_color = ax::Color(0.9);
		txt_info.cursor_color = ax::Color(0.0);
		txt_info.line_number_bg_color = ax::Color(0.2);
		txt_info.line_number_color = ax::Color(0.5);
		txt_info.text_color = ax::Color(0.0);

		ax::Rect editor_rect(
			251, rect.size.y - 200 - 18, rect.size.x - 250 - 200, 200);
		win->node.Add(
			_codeEditor = SHARED_PTR(TextEditor, editor_rect, txt_info));

		win->AddConnection(8000, GetOnCreateDraggingWidget());
		win->AddConnection(8001, GetOnDraggingWidget());
		win->AddConnection(8002, GetOnReleaseObjWidget());

		_codeEditor->GetWindow()->AddConnection(
			10020, ax::Event::Function([&](ax::Event::Msg* msg) {
				ax::Print("FUCK");
				std::vector<ax::Window::Ptr>& children
					= _gridWindow->GetWindow()->node.GetChildren();

				for (auto& n : children) {
					n->Update();
				}
			}));
	}

private:
	std::shared_ptr<TopMenu> _topMenu;
	std::shared_ptr<WidgetMenu> _widgetMenu;
	std::shared_ptr<GridWindow> _gridWindow;
	std::shared_ptr<TextEditor> _codeEditor;
	std::shared_ptr<InfoMenu> _infoMenu;
	std::vector<ax::Window::Ptr> _selected_windows;

	typedef std::pair<std::string, ax::Point> ObjMsg;

	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ObjMsg>, OnCreateDraggingWidget);
	void OnCreateDraggingWidget(const ax::Event::SimpleMsg<ObjMsg>& msg)
	{
		std::string obj_type = msg.GetMsg().first;
		ax::Point pos(msg.GetMsg().second);

		if (obj_type == "Window") {
			ax::Window::Ptr window
				= ax::Window::Create(ax::Rect(pos, ax::Size(200, 200)));

			window->property.AddProperty("AcceptWidget");

			window->event.OnPaint = ax::WFunc<ax::GC>([&, window](ax::GC gc) {
				ax::Rect rect(window->dimension.GetDrawingRect());
				gc.SetColor(0.4f, 0.4f, 0.4f);
				gc.DrawRectangle(rect);

				Script script;
				script.AddWindowHandle(window);
				script.ProcessFile(_codeEditor->GetFilePath());
			});

			window->event.OnMouseLeftDown = ax::WFunc<ax::Point>([window](
				const ax::Point& pos) {
				bool cmd_down
					= ax::App::GetInstance().GetWindowManager()->IsCmdDown();

				if (cmd_down) {
					ax::Point c_delta(
						pos - window->dimension.GetAbsoluteRect().position);
					window->resource.Add("click_delta", c_delta);

					if (c_delta.x > window->dimension.GetShownRect().size.x
							- 4) {
						ax::Print("Right resize");
						window->property.AddProperty("ResizeRight");
					}
					else if (c_delta.y > window->dimension.GetShownRect().size.y
							- 4) {
						ax::Print("Bottom resize");
						window->property.AddProperty("ResizeBottom");
					}
					else if (c_delta.x < 4) {
						ax::Print("Left resize");
						window->property.AddProperty("ResizeLeft");
					}

					window->event.GrabMouse();
				}
			});

			window->event.OnMouseLeftDragging = ax::WFunc<ax::Point>([window](
				const ax::Point& pos) {
				if (window->event.IsGrabbed()) {
					ax::Point c_delta
						= window->resource.GetResource("click_delta");

					if (window->property.HasProperty("ResizeRight")) {
						int size_y = window->dimension.GetSize().y;
						int size_x = pos.x
							- window->dimension.GetAbsoluteRect().position.x;
						window->dimension.SetSize(ax::Size(size_x, size_y));
					}
					else if (window->property.HasProperty("ResizeBottom")) {
						int size_x = window->dimension.GetSize().x;
						int size_y = pos.y
							- window->dimension.GetAbsoluteRect().position.y;
						window->dimension.SetSize(ax::Size(size_x, size_y));
					}
					else if (window->property.HasProperty("ResizeLeft")) {
						ax::Rect abs_rect(window->dimension.GetAbsoluteRect());
						int size_x = abs_rect.position.x + abs_rect.size.x
							- pos.x;
						int size_y = abs_rect.size.y;
						int pos_y = window->dimension.GetRect().position.y;
						int pos_x = pos.x
							- window->node.GetParent()
								  ->dimension.GetAbsoluteRect()
								  .position.x;
						window->dimension.SetSize(ax::Size(size_x, size_y));
						window->dimension.SetPosition(ax::Point(pos_x, pos_y));
					}
					else {
						window->dimension.SetPosition(pos
							- window->node.GetParent()
								  ->dimension.GetAbsoluteRect()
								  .position - c_delta);
					}
				}
			});

			window->event.OnMouseLeftUp
				= ax::WFunc<ax::Point>([window](const ax::Point& pos) {
					if (window->event.IsGrabbed()) {
						window->event.UnGrabMouse();
						ax::Print("Release.");

						if (window->property.HasProperty("ResizeRight")) {
							window->property.RemoveProperty("ResizeRight");
						}

						if (window->property.HasProperty("ResizeBottom")) {
							window->property.RemoveProperty("ResizeBottom");
						}

						if (window->property.HasProperty("ResizeLeft")) {
							window->property.RemoveProperty("ResizeLeft");
						}
					}
				});

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(window);
		}

		if (obj_type == "Button") {
			std::shared_ptr<ax::Window::Backbone> btn
				= ax::Button::Create(ax::Rect(pos, ax::Size(60, 25)),
					ax::Button::Events(), "Button");

			auto fct_ldown
				= btn->GetWindow()->event.OnMouseLeftDown.GetFunction();

			if (fct_ldown) {
				btn->GetWindow()->event.OnMouseLeftDown = ax::WFunc<ax::Point>(
					[btn, fct_ldown](const ax::Point& pos) {
						ax::Print("Button left down.");
						bool cmd_down = ax::App::GetInstance()
											.GetWindowManager()
											->IsCmdDown();

						ax::Print("Cmd down : ", cmd_down);
						fct_ldown(pos);
					});
			}

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(btn->GetWindow());
			btn->GetWindow()->backbone = btn;
		}

		if (obj_type == "ButtonBgImg1") {

			ax::Button::Info btn_info;
			btn_info.normal = ax::Color(0.0, 0.0);
			btn_info.hover = ax::Color(0.0, 0.0);
			btn_info.clicking = ax::Color(0.0, 0.0);
			btn_info.selected = ax::Color(0.0, 0.0);
			btn_info.contour = ax::Color(0.0, 0.0);

			std::shared_ptr<ax::Window::Backbone> btn = ax::Button::Create(
				ax::Rect(pos, ax::Size(60, 25)), ax::Button::Events(), "Button",
				"resources/axButtonImg.png", btn_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(btn->GetWindow());
			btn->GetWindow()->backbone = btn;
		}

		if (obj_type == "RoundedButton01") {

			ax::Button::Info btn_info;
			btn_info.normal = ax::Color(0.4);
			btn_info.hover = ax::Color(0.45);
			btn_info.clicking = ax::Color(0.35);
			btn_info.selected = ax::Color(0.4);
			btn_info.contour = ax::Color(0.4);
			btn_info.font_color = ax::Color(0.8);
			btn_info.round_corner_radius = 5.0;

			std::shared_ptr<ax::Window::Backbone> btn
				= ax::Button::Create(ax::Rect(pos, ax::Size(60, 25)),
					ax::Button::Events(), "Button", "", btn_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(btn->GetWindow());
			btn->GetWindow()->backbone = btn;
		}

		if (obj_type == "Toggle") {

			ax::Toggle::Info tog_info;
			tog_info.normal = ax::Color(0.7);
			tog_info.hover = ax::Color(0.75);
			tog_info.clicking = ax::Color(0.65);
			tog_info.selected = ax::Color(0.7);
			tog_info.font_color = ax::Color(0.3);

			tog_info.selected = ax::Color(0.4);
			tog_info.selected_hover = ax::Color(0.45);
			tog_info.selected_clicking = ax::Color(0.35);
			tog_info.selected = ax::Color(0.4);
			tog_info.selected_font_color = ax::Color(0.8);

			tog_info.contour = ax::Color(0.7);
			tog_info.font_size = 12;

			std::shared_ptr<ax::Window::Backbone> tog
				= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(60, 25)),
					ax::Toggle::Events(), tog_info, "", "Toggle");

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Toggle2") {

			ax::Toggle::Info tog_info;
			tog_info.normal = ax::Color(0.0, 0.0);
			tog_info.hover = ax::Color(0.0, 0.0);
			tog_info.clicking = ax::Color(0.0, 0.0);
			tog_info.selected = ax::Color(0.0, 0.0);
			tog_info.font_color = ax::Color(0.3);

			tog_info.selected = ax::Color(0.0, 0.0);
			tog_info.selected_hover = ax::Color(0.0, 0.0);
			tog_info.selected_clicking = ax::Color(0.0, 0.0);
			tog_info.selected = ax::Color(0.0, 0.0);
			tog_info.selected_font_color = ax::Color(0.8);

			tog_info.contour = ax::Color(0.0, 0.0);
			tog_info.font_size = 12;
			tog_info.img = "resources/seq_toggle.png";

			std::shared_ptr<ax::Window::Backbone> tog
				= SHARED_PTR(ax::Toggle, ax::Rect(pos, ax::Size(55, 55)),
					ax::Toggle::Events(), tog_info, "", "1");

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/LittlePhatty.png";
			k_info.selected_img_path = "resources/LittlePhatty.png";
			k_info.knob_size = ax::Size(64, 64);
			k_info.n_knob = 101;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob2") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/JP8000.png";
			k_info.selected_img_path = "resources/JP8000.png";
			k_info.knob_size = ax::Size(64, 64);
			k_info.n_knob = 101;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob3") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/BlackAngle.png";
			k_info.selected_img_path = "resources/BlackAngle.png";
			k_info.knob_size = ax::Size(100, 100);
			k_info.n_knob = 100;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob4") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/knob_dark.png";
			k_info.selected_img_path = "resources/knob_dark.png";
			k_info.knob_size = ax::Size(46, 46);
			k_info.n_knob = 128;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob5") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/knob_blue_36x36.png";
			k_info.selected_img_path = "resources/knob_blue_36x36.png";
			k_info.knob_size = ax::Size(36, 36);
			k_info.n_knob = 128;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "Knob6") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/axKnobTB303_64x64.png";
			k_info.selected_img_path = "resources/axKnobTB303_64x64.png";
			k_info.knob_size = ax::Size(64, 64);
			k_info.n_knob = 101;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}
		if (obj_type == "Knob7") {

			ax::Knob::Info k_info;
			k_info.bgColorNormal = ax::Color(0.0, 0.0);
			k_info.bgColorHover = ax::Color(0.0, 0.0);
			k_info.bgColorClicked = ax::Color(0.0, 0.0);

			k_info.img_path = "resources/axKnobReaktor.png";
			k_info.selected_img_path = "resources/axKnobReaktor.png";
			k_info.knob_size = ax::Size(46, 46);
			k_info.n_knob = 128;

			std::shared_ptr<ax::Window::Backbone> tog = SHARED_PTR(ax::Knob,
				ax::Rect(pos, k_info.knob_size), ax::Knob::Events(), k_info);

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(tog->GetWindow());
			tog->GetWindow()->backbone = tog;
		}

		if (obj_type == "LineFollower") {
			std::shared_ptr<ax::Window::Backbone> ctrl = SHARED_PTR(
				ctrl::LineFollower, ax::Rect(pos, ax::Size(300, 300)));

			ax::App::GetInstance()
				.GetPopupManager()
				->GetWindowTree()
				->AddTopLevel(ctrl->GetWindow());
			ctrl->GetWindow()->backbone = ctrl;
		}
	}

	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnDraggingWidget);
	void OnDraggingWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
	{
		ax::Point pos(msg.GetMsg());
		ax::Window::Ptr wobj = ax::App::GetInstance()
								   .GetPopupManager()
								   ->GetWindowTree()
								   ->GetTopLevel();

		if (wobj) {
			wobj->dimension.SetPosition(pos);
		}
	}

	axEVENT_ACCESSOR(ax::Event::SimpleMsg<ax::Point>, OnReleaseObjWidget);
	void OnReleaseObjWidget(const ax::Event::SimpleMsg<ax::Point>& msg)
	{
		std::vector<ax::Window::Ptr>& nodes = ax::App::GetInstance()
												  .GetPopupManager()
												  ->GetWindowTree()
												  ->GetNodeVector();
		ax::Window::Ptr win = nodes[0];

		nodes.clear();

		ax::Point pos(msg.GetMsg());
		bool inside_grid(
			_gridWindow->GetWindow()->dimension.GetAbsoluteRect().IsPointInside(
				pos));

		if (inside_grid) {
			std::vector<ax::Window::Ptr>& children(
				_gridWindow->GetWindow()->node.GetChildren());
			ax::Window::Ptr child_win = nullptr;

			for (auto& n : children) {
				if (n->property.HasProperty("AcceptWidget")) {
					ax::Rect abs_rect(n->dimension.GetAbsoluteRect());

					if (abs_rect.IsPointInside(pos)) {
						child_win = n;
					}
				}
			}

			if (child_win) {
				ax::Print("FOUND WINDOW");
				// Reparent.
				win->node.SetParent(child_win.get());
				child_win->node.GetChildren().push_back(win);
				win->dimension.SetPosition(
					pos - child_win->dimension.GetAbsoluteRect().position);
			}
			else {

				// Reparent.
				win->node.SetParent(_gridWindow->GetWindow().get());
				_gridWindow->GetWindow()->node.GetChildren().push_back(win);
				win->dimension.SetPosition(pos
					- _gridWindow->GetWindow()
						  ->dimension.GetAbsoluteRect()
						  .position);
			}
		}
	}

	void OnResize(const ax::Size& size)
	{
		// Resize main window.
		win->dimension.SetSize(size);

		// Resize top menu.
		ax::Size top_menu_size(_topMenu->GetWindow()->dimension.GetSize());
		top_menu_size.x = size.x;
		_topMenu->GetWindow()->dimension.SetSize(top_menu_size);

		// Resize widget menu.
		ax::Size widget_menu_size(
			_widgetMenu->GetWindow()->dimension.GetSize());
		widget_menu_size.y = size.y - 40 - 25;
		_widgetMenu->GetWindow()->dimension.SetSize(widget_menu_size);
	}

	void OnPaint(ax::GC gc)
	{
		ax::Rect rect(ax::Point(0, 0), win->dimension.GetSize());

		gc.SetColor(ax::Color(0.3));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);

		gc.SetColor(ax::Color(0.15));
		gc.DrawRectangle(ax::Rect(0, rect.size.y - 18, rect.size.x, 18));
	}
};

extern "C"
{
ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
{
	ax::Size fixe_size(1000, 700);
	eos::Frame* frame = new eos::Frame(ax::Rect(rect.position, fixe_size),
									   "Terminal");
	frame->GetWindow()->property.AddProperty("Resizable");
	
	std::shared_ptr<MainWindow> win(new MainWindow(frame->GetChildRect()));
	
	MAIN_WINDOW = frame->GetWindow()->node.Add(win);
	frame->SetChildHandler(win->GetWindow());
	
	return frame;
}
}

int main()
{
	ax::App& app(ax::App::GetInstance());

	ax::GL::Shader shaders("vertex_shader.glsl", "fragments_shader.glsl");

	ax::GL::Shader fb_shaders(
		"fb_vertex_shader.glsl", "fb_fragments_shader.glsl");

	ax::GL::Shader img_shaders(
		"img_vertex_shader.glsl", "img_fragments_shader.glsl");

	ax::Image* img = nullptr;

	app.AddMainEntry([&app, &shaders, &fb_shaders, &img, &img_shaders]() {
		shaders.CompileAndLink();
		fb_shaders.CompileAndLink();
		img_shaders.CompileAndLink();

		ax::Size size(
			app.GetScreenSize() - ax::Size(100, 100)); //(ax::Size(1024, 600));

		app.SetFrameSize(size);
		ax::Window::Ptr main_win = app.AddTopLevel(
			SHARED_PTR(MainWindow, ax::Rect(ax::Point(0, 0), size)));
		
//		main_win->node.Add(
//			SHARED_PTR(ctrl::LineFollower, ax::Rect(100, 100, 300, 300)));
	});

	app.AddAfterGUILoadFunction([&app]() {
		ax::Size size(app.GetScreenSize() - ax::Size(100, 100));
		app.SetFrameSize(size);
	});

	app.MainLoop();
}
