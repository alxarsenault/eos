#include "eosDock.h"
#include "eosFrame.h"

#include "axLib/axWindowManager.h"


eos::DockIcon::DockIcon(axWindow* parent,
						const ax::Rect& rect,
						const ax::Button::Events& events,
						const ax::Button::Info& info,
						const std::string& img_path,
						const std::string& msg):
ax::Button(parent, 
		   rect, 
		   events, 
		   info, 
		   img_path, 
		   "", 
		   ax::Button::Flags::SINGLE_IMG,
		   msg)
{
	
}

void eos::DockIcon::OnMouseLeave()
{
	ax::Button::OnMouseLeave();

	ax::Core::WindowManager* wm = ax::App::GetMainApp()->GetWindowManager();
	bool over_parent = wm->IsMouseHoverWindow(GetParent());
	
	if(!over_parent)
	{
		GetParent()->OnMouseLeave();
	}
}

eos::Dock::Dock(axWindow* parent, const ax::Rect& rect):
axPanel(parent, rect),
_anim_percent(1.0),
_anim_active(false),
_timer_interval(20), 
_timer_length(200),
_anim_up(true),
_up_rect(rect),
_drop_rect(ax::Rect(rect.position.x, rect.position.y + 30, rect.size.x, 10))
{
	_timer = new ax::Event::Timer(GetApp()->GetEventManager(),
								  GetOnAnimationTimer());		

	_timer_down = new ax::Event::Timer(GetApp()->GetEventManager(),
									   GetOnAnimationTimerDown());		

	ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5),
							  ax::Color(0.6, 0.6, 0.6),
							  ax::Color(0.4, 0.4, 0.4),
							  ax::Color(0.5, 0.5, 0.5),
							  ax::Color(0.0, 0.0, 0.0),
							  ax::Color(0.0, 0.0, 0.0),
							  0);
	eos::DockIcon* btn = new eos::DockIcon(this, 
										   ax::Rect(10, 5, 32, 32),
										   GetOnAppSelect(), 
										   btn_info, 
										   "FileMan_32x32.png",
										   "browser");

	_app_icons.push_back(btn);	

	eos::DockIcon* cal = new eos::DockIcon(this, 
										   ax::Rect(10 + 35, 5, 32, 32),
										   GetOnAppSelect(), 
										   btn_info, 
										   "FileMan_32x32.png",
										   "calc");

	_app_icons.push_back(cal);

	_appLoaders["calc"] = AppLoader("/home/pi/Projects/eos/app/calculator.so");
	_appLoaders["browser"] = AppLoader("/home/pi/Projects/eos/app/browser.so");
	
	//SetSize(ax::Size(rect.size.x, 10));
	//SetPosition(ax::Point(rect.position.x, _start_position.y + 30));
	SetRect(_drop_rect);
	_isDrop = true;

	for(auto& n : _app_icons) 
	{
		n->Hide();
	}
}

void eos::Dock::OnAnimationTimer(const ax::Event::Timer::Msg& msg)
{
	if(_anim_up == true)
	{

		int count = msg.GetTime().count() + _timer_interval;
		_anim_percent = float(count) / float(_timer_length);

		if(count == _timer_length)
		{
			_anim_percent = 1.0;
			_anim_active = false;
		}

		Update();
	}
}

void eos::Dock::OnAnimationTimerDown(const ax::Event::Timer::Msg& msg)
{
	if(_anim_up == false)
	{
		int count = msg.GetTime().count() + _timer_interval;
		_anim_percent = float(count) / float(_timer_length);

		if(count == _timer_length)
		{
			_anim_percent = 1.0;
			_anim_active = false;
			
			SetRect(_drop_rect);
//				ax::Rect rect(GetRect());
//				SetSize(ax::Size(rect.size.x, 10));
//				SetPosition(ax::Point(rect.position.x, rect.position.y + 30));
		}

		Update();
	}
}

bool eos::Dock::IsDrop() const
{
	return _isDrop;
}

void eos::Dock::OnMouseEnter()
{
	if(_isDrop)
	{
		SetRect(_up_rect);
		//ax::Rect rect(GetRect());
		//SetSize(ax::Size(rect.size.x, 40));
		//SetPosition(ax::Point(rect.position.x, rect.position.y - 30));
		
		_isDrop = false;	
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = true;

		_timer->StartTimer(ax::Event::Timer::TimeMs(_timer_interval), // Interval.
						   ax::Event::Timer::TimeMs(_timer_length)); // Length.


		for(auto& n : _app_icons)
		{
			n->Show();
		}
	}
}

void eos::Dock::OnMouseLeave()
{
	ax::Core::WindowManager* wm = ax::App::GetMainApp()->GetWindowManager();
	bool over = wm->IsMouseStillInChildWindow(this);

	if(over == false)
	{
//		ax::Rect rect(GetRect());
//		SetSize(ax::Size(rect.size.x, 10));
//		SetPosition(ax::Point(rect.position.x, rect.position.y + 30));
		
		_isDrop = true;
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = false;

		_timer_down->StartTimer(ax::Event::Timer::TimeMs(_timer_interval),
						  ax::Event::Timer::TimeMs(_timer_length)); 

		for(auto& n : _app_icons)
		{
			n->Hide();
		}
	}
}

void eos::Dock::OnAppSelect(const ax::Button::Msg& msg)                                 
{
	AppLoader& loader = _appLoaders[msg.GetMsg()];

	if(loader.GetHandle() == nullptr)
	{
		ax::Rect rect(500, 50, 162 + 2 * 9, 255 + 25 + 9);
		axWindow* frame = loader.Create(GetParent(), rect);
		frame->AddConnection(eos::Frame::Events::MINIMIZE, GetOnWindowMinimize()); 
		frame->AddConnection(eos::Frame::Events::CLOSE, GetOnWindowClose()); 
		return;
	}

	axWindow* frame = loader.GetHandle();

	if(frame != nullptr)
	{
		if(frame->IsShown())
		{
			frame->Hide();
		}
		else
		{
			frame->Show();
		}
	}
}	

void eos::Dock::OnWindowMinimize(const eos::Frame::Msg& msg)
{
	ax::Print("Window minimize.");
	eos::Frame* frame = msg.GetSender();
			
	if(frame != nullptr)
	{
		frame->Hide();
	}
}

void eos::Dock::OnWindowClose(const eos::Frame::Msg& msg)
{
	//ax::Print("Window close.");
	eos::Frame* frame = msg.GetSender();
	
	if(frame != nullptr)
	{
		for(auto& n : _appLoaders)
		{
			axWindow* handle = n.second.GetHandle();

			if(handle != nullptr)
			{
				if(handle->GetId() == frame->GetId())
				{
					//ax::Print("Before remove handle.");
					_appLoaders[n.first].RemoveHandle();
					frame->DeleteWindow();
					//ax::Print("Delete before return.");
					return;
				}
			}
		}
	}
}

void eos::Dock::OnPaint()
{
	ax::GC gc;
	ax::Rect rect(GetDrawingRect());

	if(_isDrop)
	{
		if(_anim_active) 
		{
			gc.SetColor(ax::Color(0.4, 0.4));  

			int y_size = 10 + (1.0 - _anim_percent) * (rect.size.y - 10);
			int y_pos = rect.size.y - y_size;

			gc.DrawRectangle(ax::Rect(0, y_pos, rect.size.x, y_size));
		}
		else
		{
			gc.SetColor(ax::Color(0.4, 0.4));
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
		}
	}
	else
	{
		if(_anim_active)
		{
			gc.SetColor(ax::Color(0.4, 0.4));

			int y_size = 10 + _anim_percent * (rect.size.y - 10); 
			int y_pos = rect.size.y - y_size;
			gc.DrawRectangle(ax::Rect(0, y_pos, rect.size.x, y_size));
		}
		else
		{
			gc.SetColor(ax::Color(0.4, 0.4));
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y)); 
		}
	}
}
