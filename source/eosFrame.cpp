#include "eosFrame.h"
#include "eosDesktop.h"

const int eos::Frame::_title_bar_height = 25;

/*******************************************************************************
 * eos::Frame::Msg
 ******************************************************************************/
eos::Frame::Msg::Msg():
_sender(nullptr)
{
    
}

eos::Frame::Msg::Msg(Frame* sender):
_sender(sender)
{
    
}

eos::Frame* eos::Frame::Msg::GetSender() const
{
    return _sender;
}

ax::Event::Msg* eos::Frame::Msg::GetCopy()
{
    return new eos::Frame::Msg(*this);
}

/*******************************************************************************
 * eos::Frame.
 ******************************************************************************/
eos::Frame::Frame(axWindow* parent,
                  const ax::Rect& rect,
                  const std::string& window_name):
// Parent.
axPanel(parent, rect),
_font(0),
_child(nullptr),
_child_menu(nullptr),
_window_name(window_name),
_menuMode(false)
{
    _abs_click_pos = ax::Point(0, 0);
    _frame_status = 0;
    _highlight = false;
    
    
    ax::Button::Info menu_btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
                                   ax::Color(1.0, 0.0, 0.0, 0.0),
                                   ax::Color(0.95, 0.0, 0.0, 0.0),
                                   ax::Color(0.5, 0.5, 0.5, 0.0),
                                   ax::Color(0.0, 0.0, 0.0, 0.0),
                                   ax::Color(0.0, 0.0, 0.0, 0.0),
                                   0);
    
//    _info_btn = new ax::Button(this, ax::Rect(50, 50, 48, 48),
//                               ax::Button::Events(),
//                               menu_btn_info,
//                               "resource/info.png", "",
//                               ax::Button::Flags::SINGLE_IMG);
    
//    _info_btn->Hide();
    
    _menu_btn = new ax::Button(this, ax::Rect(10, 4, 20, 20),
                               GetOnOpenMenu(),
                               menu_btn_info,
                               "resource/show6.png", "");
    
    ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(1.0, 0.0, 0.0),
                              ax::Color(0.95, 0.0, 0.0),
                              ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(0.0, 0.0, 0.0, 0.0),
                              ax::Color(0.9),
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


ax::Rect eos::Frame::GetChildRect() const
{
    ax::Rect rect(GetRect());
    int s_w = 4 + 5;
    return ax::Rect(s_w + 1, _title_bar_height, rect.size.x - 2 * s_w - 1, rect.size.y - _title_bar_height - s_w);
}

void eos::Frame::SetChildHandler(axWindow* child)
{
    _child = child;
}

void eos::Frame::SetChildMenuHandler(axWindow* child_menu)
{
    _child_menu = child_menu;
    
    if(_child_menu)
    {
        _child_menu->Hide();
    }
}

void eos::Frame::OnOpenMenu(const ax::Button::Msg& msg)
{
    if(_menuMode)
    {
        _menuMode = false;
        
        if(_child_menu)
        {
            _child_menu->Hide();
        }

        _child->AddProperty("Selectable");
    }
    else
    {
        _menuMode = true;
        
        if(_child_menu)
        {
            _child_menu->Show();
        }
        
        _child->RemoveProperty("Selectable");
    }
    
    Update();
}

void eos::Frame::OnButtonClick(const ax::Button::Msg& msg)
{
    PushEvent(Frame::Events::CLOSE, new Frame::Msg(this));
}

void eos::Frame::OnMinimize(const ax::Button::Msg& msg)
{
    PushEvent(Frame::Events::MINIMIZE, new Frame::Msg(this));
}

void eos::Frame::OnMouseLeave()
{
    if(_highlight)
    {
        _highlight = false;
        GetApp()->SetCursor(0);
        Update();
    }
}

void eos::Frame::OnMouseMotion(const ax::Point& pos)
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

void eos::Frame::OnMouseLeftDown(const ax::Point& pos)
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
        
        eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
        desktop->BringToFront(this);
        
        
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

void eos::Frame::OnMouseLeftUp(const ax::Point& pos)
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

void eos::Frame::OnMouseLeftDragging(const ax::Point& pos)
{
    
    if(_frame_status == 1)
    {
        ax::Rect parentRect = GetParent()->GetAbsoluteRect();
        ax::Point rel_pos = pos - parentRect.position;
        
        ax::Point prel_pos(rel_pos - _click_pos);
        if(prel_pos.y < 24)
        {
            prel_pos.y = 24
            ;
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

void eos::Frame::DrawMenuMode(ax::GC& gc)
{
    
}

void eos::Frame::OnPaintStatic()
{
//    if(_menuMode)
//    {
//        ax::GC gc;
//        ax::Rect r(GetDrawingRect());
//        
//        // Frame rect.
//        ax::Rect rect(r.position + ax::Point(5, 0), r.size - ax::Size(10, 5));
//        ax::Rect winRect(rect.position.x + 5 - 1,
//                         rect.position.y + _title_bar_height - 1,
//                         rect.size.x - 2 * 5 + 2,
//                         rect.size.y - _title_bar_height - 5 + 1);
//        
//        gc.SetColor(ax::Color(0.05, 0.85));
//        gc.DrawRectangle(winRect);
//    }
}

void eos::Frame::OnPaint()
{
    ax::GC gc;
    ax::Rect r(GetDrawingRect());
    
    
    // Frame rect.
    ax::Rect rect(r.position + ax::Point(5, 0), r.size - ax::Size(10, 5));
    
    // Title bar.
    ax::Rect topRect(rect.position, ax::Size(rect.size.x, _title_bar_height));
    gc.DrawRectangleColorFade(topRect,
                              ax::Color(0.4, 0.4),
                              ax::Color(0.42, 0.42));
//    gc.DrawRectangleColorFade(topRect,
//                              ax::Color(0.7, 0.6),
//                              ax::Color(0.72, 0.6));
//    gc.DrawRectangleColorFade(topRect,
//                              ax::Color(0.888),
//                              ax::Color(0.737, 1.0));
    
    // Frame title.
    gc.SetColor(ax::Color(0.9));
    gc.DrawStringAlignedCenter(_font, _window_name,
                               ax::Rect(rect.position,
                                        ax::Size(rect.size.x, _title_bar_height)));
    
//    // Frame contour.
    ax::Rect bottomRect(topRect.GetNextPosDown(0),
                        ax::Size(rect.size.x, rect.size.y - _title_bar_height));
    
    gc.DrawRectangleColorFade(bottomRect,
                              ax::Color(0.42, 0.42),
                              ax::Color(0.44, 0.44));
    
//    gc.DrawRectangleColorFade(bottomRect,
//                              ax::Color(0.72, 0.6),
//                              ax::Color(0.74, 0.7));

//    gc.DrawRectangleColorFade(bottomRect,
//                              ax::Color(0.737),
//                              ax::Color(0.7, 1.0));
    
    // Frame contour interior.
//    gc.SetColor(ax::Color(0.7));
    gc.SetColor(ax::Color(0.4, 0.4));
    
    ax::Rect inContour(rect.GetInteriorRect(ax::Point(1, 1)));
    gc.DrawRectangleContour(inContour);
    
    
//    gc.SetColor(ax::Color(0.6));
    gc.SetColor(ax::Color(0.2, 0.4));
    ax::Rect winRect(rect.position.x + 5 - 1,
                     rect.position.y + _title_bar_height - 1,
                     rect.size.x - 2 * 5 + 2,
                     rect.size.y - _title_bar_height - 5 + 2);
    
    gc.DrawRectangleContour(winRect);

    // Frame contour.
    gc.SetColor(ax::Color(0.2, 0.4));
    gc.DrawRectangleContour(rect);
    
//    if(_highlight)
//    {
//        gc.SetColor(ax::Color(0.67, 0.67, 0.67, 0.4));
//    }
//    else
//    {
//        gc.SetColor(ax::Color(0.6, 0.6, 0.6, 0.4));
//    }
    
    

    
//    if(_highlight)
//    {
//        gc.SetColor(ax::Color(0.85));
//    }
//    else
//    {
//        gc.SetColor(ax::Color(0.0));
//    }
//    
//    ax::Rect contour_rect(5, 1, rect.size.x - 10, rect.size.y - 6);
//    gc.DrawRectangleContour(contour_rect);
//    
//    //gc.SetColor(ax::Color(0.0, 1.0, 0.0));
//    
//    float a = 0.1;
//    float da = 0.05;
//    for(int i = 0; i < 5; i++)
//    {
//        gc.SetColor(ax::Color(0.0, a + da * i));
//        std::vector<ax::Point> cont_pts(2);
//        cont_pts[0] = ax::Point(i, 0);
//        cont_pts[1] = ax::Point(i, rect.size.y - 5);
//        
//        gc.DrawLines(cont_pts);
//        //gc.DrawRectangleContour(rect);
//    }
//
//    for(int i = 0; i < 5; i++)
//    {
//        gc.SetColor(ax::Color(0.0, a + da * i));
//        std::vector<ax::Point> cont_pts(2);
//        cont_pts[0] = ax::Point(rect.size.x - i - 1, 0);
//        cont_pts[1] = ax::Point(rect.size.x - i - 1, rect.size.y - 5);
//        
//        gc.DrawLines(cont_pts);
//        //gc.DrawRectangleContour(rect);
//    }
//    
//    for(int i = 0; i < 5; i++)
//    {
//        gc.SetColor(ax::Color(0.0, a + da * i));
//        std::vector<ax::Point> cont_pts(2);
//        cont_pts[0] = ax::Point(5, rect.size.y - i);
//        cont_pts[1] = ax::Point(rect.size.x - 4, rect.size.y - i);
//        
//        gc.DrawLines(cont_pts);
//        //gc.DrawRectangleContour(rect);
//    }
}


    
