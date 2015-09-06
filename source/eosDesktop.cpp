#include "eosDesktop.h"

#include "axLib/axWindowTree.h"
#include "axLib/axWindowManager.h"

eos::Desktop::Desktop(eos::WindowManager* eosManager,
                      const ax::Rect& rect):
// Parent.
axPanel(eosManager->GetMainApp(), rect),
_eosManager(eosManager),
_terminal(nullptr)
{
    _showView = false;
    _terminalMode = false;
    
    _terminal_app = new eos::AppLoader("/Users/alexarse/Project/eos/app/terminal.so",
                                       "TerminalMode");
    
    _bg_img = new ax::Image("resource/wallpaper-standard.png");
    
    ax::Rect status_rect(0, 0, rect.size.x, 25);
    eos::StatusBar* status = new eos::StatusBar(this, status_rect);
    _eosManager->AddChild(status);
    
    ax::Rect dock_rect(100, rect.size.y - 64 - (2 * 5),
                       rect.size.x - 200, 64 + 2 * 5);
    eos::Dock* dock = new eos::Dock(this, dock_rect);
    _eosManager->AddChild(dock);
    
    _notification = new eos::Notification(this, ax::Rect(0, 24, 200, rect.size.y - 25));
    _notification->Hide();
    _eosManager->AddChild(_notification);
}

void eos::Desktop::ShowView()
{
    _showView = true;
    std::vector<eos::Frame*> frames = _eosManager->GetFrameVector();
    for(auto& n : frames)
    {
        if(n->IsShown())
        {
            n->AddProperty("ViewMode");
            n->Hide();
        }
        
    }
    Update();
}

void eos::Desktop::ShowNotification()
{
    if(_notification->IsShown())
    {
        _notification->Hide();
    }
    else
    {
        _notification->Show();
    }
    
}

void eos::Desktop::ShowTerminal()
{
    if(_terminalMode)
    {
        if(_terminal)
        {
            _terminal->Hide();
        }
        
        _terminalMode = false;
    }
    else
    {
        if(_terminal)
        {
            _terminal->Show();
            _terminalMode = true;
        }
        else
        {
            ax::Size size(GetSize());
            _terminal = _terminal_app->Create(this, ax::Rect(0, 24, size.x, 300));
            
            if(_terminal)
            {
                _eosManager->AddChild(_terminal);
                _terminalMode = true;
            }
        }
    }
    
}


void eos::Desktop::OnMouseLeftDown(const ax::Point& mouse)
{
    if(_showView)
    {
       std::vector<eos::Frame*> frames = _eosManager->GetFrameVector();
        
        for(auto& n : frames)
        {
            if(n->HasProperty("ViewMode"))
            {
                n->Show();
                n->RemoveProperty("ViewMode");
            }
        }
        
        _showView = false;
        Update();
    }
}

void eos::Desktop::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    if(_showView)
    {
        
        gc.DrawImageResize(_bg_img,
                           ax::Point(0, 0),
                           ax::Size(rect.size.x + 1, rect.size.y + 1));
        
        gc.SetColor(ax::Color(0.2, 0.2, 0.2, 0.45));
        gc.DrawRectangle(rect);
        
        ax::Point img_pos(50, 50);
        ax::Size img_size(250, 250);
        
        axWindowTree* tree = GetApp()->GetWindowManager()->GetWindowTree();
        axWindowNode* main_node = tree->FindWinNode(this);
        
        std::vector<eos::Frame*> frames = _eosManager->GetFrameVector();
        
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
            ax::Size parent_win_size(n->GetRect().size);
            
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
        
        return;
    }
    
    
    gc.DrawImageResize(_bg_img,
                       ax::Point(0, 0),
                       ax::Size(rect.size.x + 1, rect.size.y + 1));
}