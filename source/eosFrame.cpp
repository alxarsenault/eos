#include "eosFrame.h"


namespace eos
{

	Frame::Msg::Msg():
	_sender(nullptr)
	{

	}

	Frame::Msg::Msg(Frame* sender):
	_sender(sender)
	{

	}

	Frame* Frame::Msg::GetSender() const
	{
		return _sender;
	}

	ax::Event::Msg* Frame::Msg::GetCopy()
	{
		return new eos::Frame::Msg(*this);
	}



	Frame::Frame(axWindow* parent, 
				 const ax::Rect& rect, 
				 const std::string& window_name):
    // Parent.
    axPanel(parent, rect),
	_font(0),
	_child(nullptr),
	_window_name(window_name)	
    {
		_abs_click_pos = ax::Point(0, 0);
		_frame_status = 0;
		_highlight = false;
		ax::Print("MainPanel in app.cpp");

		ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
								  ax::Color(1.0, 0.0, 0.0),
								  ax::Color(0.95, 0.0, 0.0),
								  ax::Color(0.5, 0.5, 0.5, 0.0),
								  ax::Color(0.0, 0.0, 0.0, 0.0),
								  ax::Color(0.0, 0.0, 0.0),
								  0);
		           	
		_close_btn = new ax::Button(this, ax::Rect(rect.size.x - 25 - 5 - 1, 0, 23, 25),
									ax::Button::Events(GetOnButtonClick()), 
									btn_info, "", "x");

		btn_info.hover = ax::Color(0.0, 0.0, 1.0);
		btn_info.clicking = ax::Color(0.0, 0.0, 0.95);
		_min_btn = new ax::Button(this, ax::Rect(rect.size.x - 25 - 5 - 1 - 25, 0, 23, 25),
								  ax::Button::Events(GetOnMinimize()), 
								  btn_info, "", "_");
    }


	ax::Rect Frame::GetChildRect() const
	{
		ax::Rect rect(GetRect());
		int s_w = 4 + 5;
		return ax::Rect(s_w, 25, rect.size.x - 2 * s_w, rect.size.y - 25 - s_w);
	}

	void Frame::SetChildHandler(axWindow* child)
	{
		_child = child;
	}

	void Frame::OnButtonClick(const ax::Button::Msg& msg)
	{
		PushEvent(Frame::Events::CLOSE, new Frame::Msg(this));
		//PushEvent(Frame::Events::
		//ax::Print("Button click in module.");
	
		//PushEvent(190, new ax::Button::Msg(msg));
		//this->DeleteWindow();
	}
	
	void Frame::OnMinimize(const ax::Button::Msg& msg)
	{
		PushEvent(Frame::Events::MINIMIZE, new Frame::Msg(this));
		//PushEvent(189, new ax::Button::Msg(msg));
		//Hide();
	}

	void Frame::OnMouseLeave()
	{
		if(_highlight)
		{
			_highlight = false;
			GetApp()->SetCursor(0);
			Update();
		}
	}

	void Frame::OnMouseMotion(const ax::Point& pos)
	{
		ax::Point rel_pos = pos - GetAbsoluteRect().position;  

		bool can_resize = HasProperty("Resizable"); 

		 if(can_resize && rel_pos.x <= 4) 
		 {
			GetApp()->SetCursor(87);
			_highlight = true;
			Update();
			return;
		 }

		 else if(can_resize && rel_pos.y >= GetSize().y - 4) 
		 {
			GetApp()->SetCursor(89);
			_highlight = true;
			Update();
			return;
		 }
		 if(_highlight)
		 {
			GetApp()->SetCursor(0);
			_highlight = false;
			Update();
		 }
	}

	void Frame::OnMouseLeftDown(const ax::Point& pos)
	{
		_abs_click_pos = pos;
		ax::Point rel_pos = pos - GetAbsoluteRect().position;
	
		bool can_resize = HasProperty("Resizable");
		
		// Resize from left size.	
		if(can_resize && rel_pos.x <= 4)
		{
			_frame_status = 2;
			_click_pos = rel_pos; 
			_click_size = GetSize();
			_highlight = true;
			GetApp()->SetCursor(87);
			Update();
			GrabMouse(); 
		}

		// Move from top frame bar.
		else if(rel_pos.y < 25)
		{
			_frame_status = 1;
			_click_pos = rel_pos;
			_highlight = true;
			Update();
			GrabMouse();
			GetApp()->SetCursor(0);
		}

		// Resize from bottom.
		else if(can_resize && rel_pos.y >= GetSize().y - 4)
		{
			_frame_status = 3;
			_click_pos = rel_pos;
			_highlight = true;
			_click_size = GetSize();
			
			GetApp()->SetCursor(89);
			Update();
			GrabMouse();
		}

		// In child window.
		else
		{
			GetApp()->SetCursor(0);
		}
	}

	void Frame::OnMouseLeftUp(const ax::Point& pos)
	{
		if (IsGrabbed())
		{
			ax::Point rel_pos = pos - GetAbsoluteRect().position; 

			if(rel_pos.x > 4 || rel_pos.y < GetSize().y - 4)
			{
				_highlight = false; 
				GetApp()->SetCursor(0);
			}

			if(_frame_status == 2 || _frame_status == 3)
			{
				if(_child != nullptr)
				{
					_child->SetSize(GetSize() - ax::Size(8 + 2 * 5, 25 + 4 + 5));
				}
			}

			Update();
			UnGrabMouse();
		}
	}

	void Frame::OnMouseLeftDragging(const ax::Point& pos)
	{

		if(_frame_status == 1)
		{
			ax::Rect parentRect = GetParent()->GetAbsoluteRect();
			ax::Point rel_pos = pos - parentRect.position;
			
			ax::Point prel_pos(rel_pos - _click_pos);			
			if(prel_pos.y < 25)
			{
				prel_pos.y = 25;
			}

			SetPosition(prel_pos);	
		}
		else if(_frame_status == 2)
		{
			ax::Rect parentRect = GetParent()->GetAbsoluteRect();
			ax::Point rel_pos = pos - parentRect.position;  
			
			//ax::Point local_rel_pos = pos - GetAbsoluteRect().position;
			ax::Point win_pos(GetRect().position);
			SetPosition(ax::Point(rel_pos.x - _click_pos.x, win_pos.y));
			
			ax::Size new_size(_click_size + ax::Size(_abs_click_pos.x - pos.x, 0));	

			//ax::Print("New size :", new_size.x, new_size.y);
			SetSize(new_size);
			_close_btn->SetPosition(ax::Point(GetRect().size.x - 25 - 5 - 1, 0));

			_min_btn->SetPosition(ax::Point(GetRect().size.x - 25 - 5 - 1 - 25, 0));
//			if(_child != nullptr)
//			{
//				_child->SetSize(new_size - ax::Size(2, 26));
//			}

		}
		else if(_frame_status == 3)
		{
			ax::Rect parentRect = GetParent()->GetAbsoluteRect();
			ax::Point rel_pos = pos - parentRect.position;  
			
			//ax::Point local_rel_pos = pos - GetAbsoluteRect().position;
			//ax::Point win_pos(GetRect().position);
			//SetPosition(ax::Point(win_pos.x, rel_pos.y - _click_pos.y));
			
			ax::Size new_size(_click_size + ax::Size(0, pos.y - _abs_click_pos.y));	

			//ax::Print("New size :", new_size.x, new_size.y);
			SetSize(new_size);
			//_close_btn->SetPosition(ax::Point(GetRect().size.x - 25, 0));
			

//			if(_child != nullptr)
//			{
//				_child->SetSize(new_size - ax::Size(2, 26));
//			}
		}
	}

	void Frame::OnPaint()
	{
		ax::GC gc;
        ax::Rect rect(GetDrawingRect());  

		gc.SetColor(ax::Color(0.95));
        gc.DrawRectangle(ax::Rect(4 + 5, 25, rect.size.x - 10 - 8, rect.size.y - 25 - 5 - 4));
		



		if(_highlight)
		{
			gc.SetColor(ax::Color(0.67, 0.67, 0.67, 0.4));
		}
		else
		{
			gc.SetColor(ax::Color(0.6, 0.6, 0.6, 0.4));
		}
	
		// Title bar.	
		//gc.DrawRectangle(ax::Rect(5, 0, rect.size.x - 10, 25));

		gc.DrawRectangle(ax::Rect(5, 0, rect.size.x - 10, rect.size.y - 5));

		gc.SetColor(ax::Color(0.95));
		gc.DrawString(_font, _window_name, ax::Point(12, 5));

		if(_highlight)
		{
			gc.SetColor(ax::Color(0.85));
		}
		else
		{
			gc.SetColor(ax::Color(0.0));
		}
		
		ax::Rect contour_rect(5, 1, rect.size.x - 10, rect.size.y - 6);
		gc.DrawRectangleContour(contour_rect);

		//gc.SetColor(ax::Color(0.0, 1.0, 0.0));
		
		float a = 0.1;
		float da = 0.05;
		for(int i = 0; i < 5; i++)
		{
			gc.SetColor(ax::Color(0.0, a + da * i));
			std::vector<ax::Point> cont_pts(2);
			cont_pts[0] = ax::Point(i, 0);
			cont_pts[1] = ax::Point(i, rect.size.y - 5);

			gc.DrawLines(cont_pts);
			//gc.DrawRectangleContour(rect);
		}

		for(int i = 0; i < 5; i++)
		{
			gc.SetColor(ax::Color(0.0, a + da * i));
			std::vector<ax::Point> cont_pts(2);
			cont_pts[0] = ax::Point(rect.size.x - i - 1, 0);
			cont_pts[1] = ax::Point(rect.size.x - i - 1, rect.size.y - 5);

			gc.DrawLines(cont_pts);
			//gc.DrawRectangleContour(rect);
		}

		for(int i = 0; i < 5; i++)
		{
			gc.SetColor(ax::Color(0.0, a + da * i));
			std::vector<ax::Point> cont_pts(2);
			cont_pts[0] = ax::Point(5, rect.size.y - i);
			cont_pts[1] = ax::Point(rect.size.x - 4, rect.size.y - i);

			gc.DrawLines(cont_pts);
			//gc.DrawRectangleContour(rect);
		}
   }   

}
