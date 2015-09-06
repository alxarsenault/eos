#include "eosWindowManager.h"
#include "eosDesktop.h"

#include "axLib/axWindowManager.h"
#include "axLib/axWindowTree.h"

eos::WindowManager::WindowManager()
{
    _axApp = new ax::App();
}

void eos::WindowManager::MainLoop()
{
    _axApp->MainLoop();
}

void eos::WindowManager::AddFrame(eos::Frame* frame)
{
    if(frame != nullptr)
    {
        axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();

        axWindowNode* main_node = tree->FindWinNode(_desktop);
        
        std::vector<axWindowNode*>& node_childs = main_node->GetChild();
        
        if(_childs.size() > 1)
        {
            // First added child.
            axWindow* first_desktop_child = _childs[0];
            std::vector<axWindowNode*>::iterator node = node_childs.end();
            
            // Find node.
            for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
            {
                if((*it)->GetWindow() == first_desktop_child)
                {
                    node = it;
                    break;
                }
            }
            
            if(node != node_childs.end())
            {
                axWindowNode* frame_node = node_childs.back();
                node_childs.insert(node, frame_node);
                node_childs.pop_back();
            }
        }
    }
}

void eos::WindowManager::BringToFront(eos::Frame* frame)
{
    if(frame != nullptr)
    {
        axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
        axWindowNode* main_node = tree->FindWinNode(_desktop);
        
        std::vector<axWindowNode*>& node_childs = main_node->GetChild();
        std::vector<axWindowNode*>::iterator node = node_childs.end();
        
        if(_childs.size())
        {
            axWindow* first_desktop_child = _childs[0];
            
            // Find node.
            for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
            {
                if((*it)->GetWindow() == first_desktop_child)
                {
                    node = it;
                    break;
                }
            }
        }
        else
        {
            node = node_childs.end() - 1;
        }
        
        std::vector<axWindowNode*>::iterator frame_node =  node_childs.end();
        
        // Find frame node.
        for(auto it = node_childs.begin(); it != node_childs.end(); ++it)
        {
            if((*it)->GetWindow() == frame)
            {
                frame_node = it;
                break;
            }
        }
    
        if(node != node_childs.end() && frame_node != node_childs.end())
        {
            node_childs.insert(node, *frame_node);
            node_childs.erase(frame_node);
        }
    }
}

std::vector<eos::Frame*> eos::WindowManager::GetFrameVector()
{
    std::vector<eos::Frame*> frames;
    axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
    axWindowNode* main_node = tree->FindWinNode(_desktop);
    
    axWindow* first_desktop_child = nullptr;
    
    if(_childs.size())
    {
        first_desktop_child = _childs[0];
    }
    
    for(auto& n : main_node->GetChild())
    {
        if(n->window == first_desktop_child)
        {
            break;
        }
        
        frames.push_back(static_cast<eos::Frame*>(n->window));
    }
    
    return frames;
}