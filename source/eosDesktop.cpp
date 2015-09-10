#include "eosDesktop.h"



#include "axLib/axWindowTree.h"
#include "axLib/axWindowManager.h"
#include "eosCoreTracer.h"
eos::Desktop::Desktop(eos::Core::System* system,
                      const ax::Rect& rect):
// Parent.
axPanel(system->GetManager()->GetMainApp(), rect),
_terminal(nullptr),
_system(system)
{
    _desktop_apps_status[DSKT_APP_TERMINAL] = false;
    _desktop_apps_status[DSKT_APP_TRACE]    = false;
    _desktop_apps_status[DSKT_APP_NOTIFY]   = false;
    
    _desktop_apps[DSKT_APP_TERMINAL] = nullptr;
    _desktop_apps[DSKT_APP_TRACE]    = nullptr;
    _desktop_apps[DSKT_APP_NOTIFY]   = nullptr;
    
    _showView = false;
    
    _terminal_app = new eos::AppLoader("/Users/alexarse/Project/eos/app/terminal.so",
                                       "TerminalMode");
    
    _bg_img = new ax::Image("resource/wallpaper-standard.png");
    
    eos::Core::Manager* man = system->GetManager();
    
    
    
    ax::Rect status_rect(0, 0, rect.size.x, 25);
    eos::StatusBar* status = new eos::StatusBar(this, status_rect);
    man->AddChild(status);
    
    ax::Rect dock_rect(100, rect.size.y - 64 - (2 * 5),
                       rect.size.x - 200, 64 + 2 * 5);
    eos::Dock* dock = new eos::Dock(this, dock_rect);
    man->AddChild(dock);
    
    axWindow* term = _terminal_app->Create(this, ax::Rect(0, 24, rect.size.x, 300));
    
    if(term != nullptr)
    {
        term->Hide();
        man->AddChild(term);
        _desktop_apps[DSKT_APP_TERMINAL] = term;
    }
    
    
    eos::Notification* notif = new eos::Notification(this,
                                                     ax::Rect(0, 24, 200, rect.size.y - 25));
    notif->Hide();
    man->AddChild(notif);
    _desktop_apps[DSKT_APP_NOTIFY] = notif;
    
    
    eos::Alert* trace = new eos::Alert(this,
                                       ax::Rect(rect.size.x - 350, 24, 350, 100),
                                       _system);
    trace->Hide();
    man->AddChild(trace);
    _desktop_apps[DSKT_APP_TRACE] = trace;
    
}

void eos::Desktop::ShowView()
{
    _system->GetTracer()->Write("Desktop view.");
    
    _showView = true;
    
    eos::Core::Manager* man = _system->GetManager();
    std::vector<eos::Frame*> frames = man->GetFrameVector();
    
    for(auto& n : frames)
    {
        if(n->IsShown())
        {
            n->AddProperty("ViewMode");
            n->Hide();
        }
    }
    
    for(int i = 0; i < DSKT_APP_COUNT; i++)
    {
        if(_desktop_apps[i] &&  _desktop_apps_status[i])
        {
            _desktop_apps[i]->AddProperty("ViewMode");
            _desktop_apps[i]->Hide();
        }
    }
    
    Update();
}

void eos::Desktop::ToggleDesktopApp(const DesktopApps& app)
{
    if(app < DesktopApps::DSKT_APP_COUNT && _desktop_apps[app])
    {
        if(_desktop_apps_status[app])
        {
            _desktop_apps[app]->Hide();
        }
        else
        {
            _desktop_apps[app]->Show();
        }
        
        // Toggle app status.
        _desktop_apps_status[app] = !_desktop_apps_status[app];
        
        ReposAllDesktopBuiltInApps();
    }
}

void eos::Desktop::ReposAllDesktopBuiltInApps()
{
    if(_desktop_apps_status[DSKT_APP_TERMINAL])
    {
        bool c = _desktop_apps_status[DSKT_APP_TRACE];
        bool n = _desktop_apps_status[DSKT_APP_NOTIFY];
        
        ax::Size noti_size = n ? _desktop_apps[DSKT_APP_NOTIFY]->GetSize() : ax::Size(0, 0);
        ax::Size trac_size = c ? _desktop_apps[DSKT_APP_TRACE]->GetSize() : ax::Size(0, 0);
        ax::Size term_size = ax::Size(GetSize().x, 300) - ax::Size(noti_size.x + trac_size.x);
        
        ax::Point term_pos(noti_size.x, 24);
        
        if(_desktop_apps[DSKT_APP_TERMINAL])
        {
            _desktop_apps[DSKT_APP_TERMINAL]->SetRect(ax::Rect(term_pos, term_size));
        }
        
        if(_desktop_apps[DSKT_APP_TRACE])
        {
            _desktop_apps[DSKT_APP_TRACE]->SetSize(ax::Size(350, 300));
        }
    }
    else
    {
        if(_desktop_apps_status[DSKT_APP_TRACE] && _desktop_apps[DSKT_APP_TRACE])
        {
            ax::Size tr_size(_desktop_apps[DSKT_APP_TRACE]->GetSize());
            if(tr_size == ax::Size(350, 300))
            {
                _desktop_apps[DSKT_APP_TRACE]->SetSize(ax::Size(350, 100));
            }
        }
    }
}

void eos::Desktop::OnMouseLeftDown(const ax::Point& mouse)
{
    if(_showView)
    {
        eos::Core::Manager* man = _system->GetManager();
       std::vector<eos::Frame*> frames = man->GetFrameVector();
        
        for(auto& n : frames)
        {
            if(n->HasProperty("ViewMode"))
            {
                n->Show();
                n->RemoveProperty("ViewMode");
            }
        }
        
        for(int i = 0; i < DSKT_APP_COUNT; i++)
        {
            if(_desktop_apps[i] &&  _desktop_apps_status[i])
            {
                if(_desktop_apps[i]->HasProperty("ViewMode"))
                {
                    _desktop_apps[i]->Show();
                    _desktop_apps[i]->RemoveProperty("ViewMode");
                }
            }
        }
        
        _showView = false;
        Update();
    }
}

void eos::Desktop::OnMouseMotion(const ax::Point& mouse)
{
    
}

void eos::Desktop::OnMouseLeftUp(const ax::Point& mouse)
{
    
}

void eos::Desktop::PaintView(ax::GC& gc)
{
    ax::Rect rect(GetDrawingRect());
    
    gc.DrawImageResize(_bg_img,
                       ax::Point(0, 0),
                       ax::Size(rect.size.x + 1, rect.size.y + 1));
    
    gc.SetColor(ax::Color(0.2, 0.2, 0.2, 0.45));
    gc.DrawRectangle(rect);
    
    ax::Point img_pos(50, 50);
    ax::Size img_size(250, 250);
    
    axWindowTree* tree = GetApp()->GetWindowManager()->GetWindowTree();
    axWindowNode* main_node = tree->FindWinNode(this);
    
    eos::Core::Manager* man = _system->GetManager();
    std::vector<eos::Frame*> frames = man->GetFrameVector();
    
    std::function<void(axWindowNode*, ax::Rect, ax::Rect, ax::Point)> fct =
    [&](axWindowNode* node, ax::Rect main_rect, ax::Rect draw_rect, ax::Point delta_pos)
    {
        axWindow* win = node->window;
        ax::Rect win_rect = win->GetRect();
        win_rect.position += delta_pos;
        
        ax::Point delta(win_rect.position.x / double(main_rect.size.x) * draw_rect.size.x,
                        win_rect.position.y / double(main_rect.size.y) * draw_rect.size.y);
        
        ax::Point pos = draw_rect.position + delta;
        
        ax::Size size = ax::Size(win_rect.size.x / double(main_rect.size.x) * draw_rect.size.x,
                                 win_rect.size.y / double(main_rect.size.y) * draw_rect.size.y);
        
        gc.DrawTexture(win->GetFrameBuffer()->GetFrameBufferTexture(),
                       ax::Rect(pos, size));
        
        for(auto& k : node->GetChild())
        {
            fct(k, main_rect, draw_rect, win_rect.position);
        }
    };
    
    for(auto& n : frames)
    {
        gc.DrawTexture(n->GetFrameBuffer()->GetFrameBufferTexture(),
                       ax::Rect(img_pos, img_size));
        
        
        for(auto& k : main_node->Get(n)->GetChild())
        {
            fct(k,
                ax::Rect(ax::Point(0, 0), n->GetSize()),
                ax::Rect(img_pos, img_size),
                ax::Point(0, 0));
        }
        
        img_pos += ax::Point(img_size.x + 20, 0);
    }
}

void eos::Desktop::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    if(_showView)
    {
        PaintView(gc);
        return;
    }
    
    
    gc.DrawImageResize(_bg_img, ax::Point(0, 0),
                       ax::Size(rect.size.x + 1, rect.size.y + 1));
}