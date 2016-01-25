#include "eosKeyboard.h"
#include <axLib/axButton.h>

namespace eos {
	Keyboard::Keyboard(const ax::Rect& rect)
	{
		// Create window.
		win = ax::Window::Create(rect);
		win->event.OnPaint = ax::WBind<ax::GC>(this, &Keyboard::OnPaint);
		
		ax::Button::Info btn_info;
		btn_info.normal = ax::Color(0.7);
		btn_info.hover = btn_info.normal;
		btn_info.clicking = ax::Color(0.5);
		btn_info.selected = btn_info.normal;
		btn_info.contour = btn_info.normal;
		btn_info.font_color = ax::Color(0.0);
		btn_info.round_corner_radius = 0;
		
		const int key_delta = 5;
		const int key_width = 60;
		const int key_height = 60;
		
		const ax::Size tab_size(85, key_height);
		const ax::Size delete_size(85, key_height);
		const ax::Size caps_size(95, key_height);
		const ax::Size return_size(115, key_height);
		const ax::Size shift_size(125, key_height);
		const ax::Size cmd_size(80, key_height);
		const ax::Size space_size(390, key_height);
		
		const ax::Size letter_size(key_width, key_height);
		const ax::Point init_btn_pos(10, 10);
		
		ax::Button::Events btn_evts;
		ax::Point btn_pos(init_btn_pos);
		
		ax::StringVector first_row_letters = { "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=" };
		ax::StringVector second_row_letters
		= { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\" };
		
		ax::StringVector third_row_letters
		= { "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'"};
		
		ax::StringVector fourth_row_letters
		= { "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"};
		
		
		for (auto& letter : first_row_letters) {
			ax::Button::Ptr btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", letter));
			btn_pos = win->node.Add(btn)->dimension.GetRect().GetNextPosRight(key_delta);
		}
		
		ax::Button::Ptr delete_btn(new ax::Button(ax::Rect(btn_pos, delete_size), btn_evts, btn_info, "", "delete"));
		win->node.Add(delete_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		// Second row.
		btn_pos = ax::Point(init_btn_pos);
		btn_pos.y += (key_height + key_delta);
		
		ax::Button::Ptr tab_btn(new ax::Button(ax::Rect(btn_pos, tab_size), btn_evts, btn_info, "", "tab"));
		btn_pos = win->node.Add(tab_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		for (auto& letter : second_row_letters) {
			ax::Button::Ptr btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", letter));
			btn_pos = win->node.Add(btn)->dimension.GetRect().GetNextPosRight(key_delta);
		}
		
		// Third row.
		btn_pos = ax::Point(init_btn_pos);
		btn_pos.y += (2 * key_height + 2 * key_delta);
		
		ax::Button::Ptr caps_btn(new ax::Button(ax::Rect(btn_pos, caps_size), btn_evts, btn_info, "", "caps lock"));
		btn_pos = win->node.Add(caps_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		for (auto& letter : third_row_letters) {
			ax::Button::Ptr btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", letter));
			btn_pos = win->node.Add(btn)->dimension.GetRect().GetNextPosRight(key_delta);
		}
		
		// Return.
		ax::Button::Ptr return_btn(new ax::Button(ax::Rect(btn_pos, return_size), btn_evts, btn_info, "", "return"));
		win->node.Add(return_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		// Fourth row.
		btn_pos = ax::Point(init_btn_pos);
		btn_pos.y += (3 * key_height + 3 * key_delta);
		
		ax::Button::Ptr shift_btn(new ax::Button(ax::Rect(btn_pos, shift_size), btn_evts, btn_info, "", "shift"));
		btn_pos = win->node.Add(shift_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		for (auto& letter : fourth_row_letters) {
			ax::Button::Ptr btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", letter));
			btn_pos = win->node.Add(btn)->dimension.GetRect().GetNextPosRight(key_delta);
		}
		
		ax::Button::Ptr shift_right_btn(new ax::Button(ax::Rect(btn_pos + ax::Point(25, 0), letter_size), btn_evts, btn_info, "", "up"));
		btn_pos = win->node.Add(shift_right_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		// Fifth row.
		btn_pos = ax::Point(init_btn_pos);
		btn_pos.y += (4 * key_height + 4 * key_delta);
		
		ax::Button::Ptr func_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "fn"));
		btn_pos = win->node.Add(func_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr ctrl_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "ctrl"));
		btn_pos = win->node.Add(ctrl_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr alt_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "alt"));
		btn_pos = win->node.Add(alt_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr cmd_btn(new ax::Button(ax::Rect(btn_pos, cmd_size), btn_evts, btn_info, "", "cmd"));
		btn_pos = win->node.Add(cmd_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr space_btn(new ax::Button(ax::Rect(btn_pos, space_size), btn_evts, btn_info, "", ""));
		btn_pos = win->node.Add(space_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr opt_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "alt"));
		btn_pos = win->node.Add(opt_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr left_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "left"));
		btn_pos = win->node.Add(left_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr down_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "down"));
		btn_pos = win->node.Add(down_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		ax::Button::Ptr right_btn(new ax::Button(ax::Rect(btn_pos, letter_size), btn_evts, btn_info, "", "right"));
		btn_pos = win->node.Add(right_btn)->dimension.GetRect().GetNextPosRight(key_delta);
		
		//	Button(const ax::Rect& rect, const Button::Events& events,
		//		   const Button::Info& info, std::string img_path = "",
		//		   std::string label = "", ax::Flag flags = 0, std::string msg = "");
	}
	
	void Keyboard::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		
		gc.SetColor(ax::Color(0.25));
		gc.DrawRectangle(rect);
		gc.DrawRectangleContour(rect);
	}
}