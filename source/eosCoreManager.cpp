#include "eosCoreManager.h"
#include "eosDesktop.h"

#include "axLib/axWindowManager.h"
#include "axLib/axWindowTree.h"

eos::Core::Manager::Manager()
{
    _axApp = new ax::App();
}

void eos::Core::Manager::MainLoop()
{
    _axApp->MainLoop();
}

void eos::Core::Manager::AddFrame(eos::Frame* frame)
{
    if(frame != nullptr)
    {
        axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();

        axWindowNode* main_node = tree->FindWinNode(_desktop);
        
        std::vector<axWindowNode*>& node_childs = main_node->GetChild();
        
        if(_childs.size() > 1)
        {
            // First added child.
            ax::Window* first_desktop_child = _childs[0];
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

void eos::Core::Manager::BringToFront(eos::Frame* frame)
{
    if(frame != nullptr)
    {
        axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
        axWindowNode* main_node = tree->FindWinNode(_desktop);
        
        std::vector<axWindowNode*>& node_childs = main_node->GetChild();
        std::vector<axWindowNode*>::iterator node = node_childs.end();
        
        if(_childs.size())
        {
            ax::Window* first_desktop_child = _childs[0];
            
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

std::vector<eos::Frame*> eos::Core::Manager::GetFrameVector()
{
    std::vector<eos::Frame*> frames;
    axWindowTree* tree = _axApp->GetWindowManager()->GetWindowTree();
    axWindowNode* main_node = tree->FindWinNode(_desktop);
    
    ax::Window* first_desktop_child = nullptr;
    
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