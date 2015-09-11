#include "eosDock.h"
#include "eosFrame.h"

#include "axLib/axWindowManager.h"
#include "eosDesktop.h"
#include "eosAlert.h"

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
		   msg),
_anim_percent(1.0)
{
	
}

void eos::DockIcon::SetAnimationPercent(const double& percent)
{
	_anim_percent = percent;
	Update();
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

void eos::DockIcon::OnPaint()
{
    ax::GC gc;
    
    ax::Rect rect(GetRect());
    ax::Rect rect0(GetDrawingRect());
    
	//ax::Color col(*_currentColor);
	//col.SetAlpha(_anim_percent * _currentColor->GetAlpha());

    //gc.SetColor(col);
    //gc.DrawRectangle(rect0);
    
    if (_btnImg->IsImageReady())
    {
        gc.DrawImageResize(_btnImg, ax::Point(0, 0), rect0.size, _anim_percent);
    }
   
    //ax::Color ctr_col(static_cast<ax::Button::Info*>(_info)->contour);
    //ctr_col.SetAlpha(_anim_percent * ctr_col.GetAlpha());


	//gc.SetColor(ctr_col);
	//gc.DrawRectangleContour(rect0);
}


eos::Dock::Dock(axWindow* parent, const ax::Rect& rect):
axPanel(parent, rect),
_anim_percent(1.0),
_anim_active(false),
_timer_interval(20), 
_timer_length(200),
_anim_up(true),
_up_rect(rect),
_drop_rect(ax::Rect(rect.position.x, rect.position.y + rect.size.y - 10, rect.size.x, 10))
{
	_timer = new ax::Event::Timer(GetApp()->GetEventManager(),
								  GetOnAnimationTimer());		

	_timer_down = new ax::Event::Timer(GetApp()->GetEventManager(),
									   GetOnAnimationTimerDown());		

	ax::Button::Info btn_info(ax::Color(0.3, 0.3),
							  ax::Color(0.32, 0.3),
							  ax::Color(0.28, 0.3),
							  ax::Color(0.3, 0.3),
							  ax::Color(0.0, 0.0),
							  ax::Color(0.0, 0.0),
							  0);

	ax::Size icon_size(64, 64);

        ax::StringPairVector apps_icon_info =
    {
        ax::StringPair("resource/1441952929_house.png", "browser"), //resource/folder.png
        ax::StringPair("resource/1441951759_calculator.png", "calc"), // resource/calculator-icon.png
        ax::StringPair("resource/1441953077_notepad.png", "txtedit"), // Apps-text-editor-icon.png
        ax::StringPair("resource/1441952725_terminal.png", "term"), // terminal.png
        ax::StringPair("resource/1441953272_enveloppe-alt.png", "mail"), //image.png
        ax::StringPair("resource/1441952856_calendar.png", "calender"),
        ax::StringPair("resource/1441952883_book.png", "book"),
        ax::StringPair("resource/1441953050_image.png", "viewer"), //image.png
        ax::StringPair("resource/1441953912_wrench-screwdriver.png", "settings"),
        ax::StringPair("resource/1441954538_appicns_Trash_Empty.png", "trash")
    };

    ax::Point icon_pos(10, 5);
    
    for(auto& n : apps_icon_info)
    {
        eos::DockIcon* icon = new eos::DockIcon(this,
                                                ax::Rect(icon_pos, icon_size),
                                                GetOnAppSelect(),
                                                btn_info,
                                                n.first,
                                                n.second);
        _app_icons.push_back(icon);
        
        icon_pos = icon->GetRect().GetNextPosRight(10);
    }
    

//	_appLoaders["calc"] = AppLoader("/home/pi/Projects/eos/app/calculator.so");
//	_appLoaders["browser"] = AppLoader("/home/pi/Projects/eos/app/browser.so");	
//	_appLoaders["txtedit"] = AppLoader("/home/pi/Projects/eos/app/text_editor.so");
//  _appLoaders["term"] = AppLoader("/home/pi/Projects/eos/app/terminal.so");
    
    _appLoaders["calc"] = AppLoader("/Users/alexarse/Project/eos/app/calculator.so");
    _appLoaders["browser"] = AppLoader("/Users/alexarse/Project/eos/app/browser.so");
//    _appLoaders["browser"] = AppLoader("/Users/alexarse/Project/eos/app/terminal.so");
    _appLoaders["txtedit"] = AppLoader("/Users/alexarse/Project/eos/app/text_editor.so");
    _appLoaders["term"] = AppLoader("/Users/alexarse/Project/eos/app/terminal.so");
    _appLoaders["viewer"] = AppLoader("/Users/alexarse/Project/eos/app/image_viewer.so");
    _appLoaders["mail"] = AppLoader("/Users/alexarse/Project/eos/app/mail.so");
    _appLoaders["calender"] = AppLoader("/Users/alexarse/Project/eos/app/mail.so");
    _appLoaders["book"] = AppLoader("/Users/alexarse/Project/eos/app/mail.so");
    _appLoaders["settings"] = AppLoader("/Users/alexarse/Project/eos/app/mail.so");
    _appLoaders["trash"] = AppLoader("/Users/alexarse/Project/eos/app/mail.so");
    
    
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

		for(auto& n : _app_icons)
		{
			n->SetAnimationPercent(_anim_percent);
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

		for(auto& n : _app_icons)
		{
			n->SetAnimationPercent(1.0 - _anim_percent);
		}

		if(count == _timer_length)
		{
			_anim_percent = 1.0;
			_anim_active = false;
			
			SetRect(_drop_rect);

			for(auto& n : _app_icons)
			{
				n->Hide();
			}
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
		
		_isDrop = false;	
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = true;

		_timer->StartTimer(ax::Event::Timer::TimeMs(_timer_interval), // Interval.
						   ax::Event::Timer::TimeMs(_timer_length)); // Length.


		for(auto& n : _app_icons)
		{
			n->SetAnimationPercent(_anim_percent);
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
		_isDrop = true;
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = false;

		_timer_down->StartTimer(ax::Event::Timer::TimeMs(_timer_interval),
						  ax::Event::Timer::TimeMs(_timer_length)); 

//		for(auto& n : _app_icons)
//		{
//			n->Hide();
//		}
	}
}

void eos::Dock::OnAppSelect(const ax::Button::Msg& msg)                                 
{
	AppLoader& loader = _appLoaders[msg.GetMsg()];

	if(loader.GetHandle() == nullptr)
	{
		ax::Rect rect(500, 50, 162 + 2 * 9, 255 + 25 + 9);
		
        axWindow* frame = loader.Create(GetParent(), rect);
        
        if(frame != nullptr)
        {
            frame->AddConnection(eos::Frame::Events::MINIMIZE, GetOnWindowMinimize());
            frame->AddConnection(eos::Frame::Events::CLOSE, GetOnWindowClose());
            
            eos::Frame* osframe = static_cast<eos::Frame*>(frame);
            static_cast<eos::Desktop*>(GetParent())->AddFrame(osframe);
        }
        else
        {
            // WARNING MESSAGE BOX.
//            eos::Alert* alert = new eos::Alert(GetParent(),
//                                               ax::Rect(50, 50, 200, 200),
//                                               "Can't open application.");
        }
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

    ax::Color dock_color(0.7, 0.4);
	if(_isDrop)
	{
		if(_anim_active) 
		{
			gc.SetColor(dock_color);

			int y_size = 10 + (1.0 - _anim_percent) * (rect.size.y - 10);
			int y_pos = rect.size.y - y_size;

			gc.DrawRectangle(ax::Rect(0, y_pos, rect.size.x, y_size));
		}
		else
		{
			gc.SetColor(dock_color);
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
		}
	}
	else
	{
		if(_anim_active)
		{
			gc.SetColor(dock_color);

			int y_size = 10 + _anim_percent * (rect.size.y - 10); 
			int y_pos = rect.size.y - y_size;
			
            gc.DrawRectangle(ax::Rect(0, y_pos, rect.size.x, y_size));
		}
		else
		{
			gc.SetColor(dock_color);
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
            
//            gc.DrawRectangleColorFade(ax::Rect(0, rect.size.y - 20, rect.size.x, 20),
//                                      ax::Color(1.0, 0.0), dock_color);
//            gc.DrawRectangleColorFade(ax::Rect(0, 0, rect.size.x, rect.size.y),
//                                      ax::Color(1.0, 0.0), dock_color);
		}
	}
}
