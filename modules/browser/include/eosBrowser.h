//
//  eosBrowser.h
//  Minimal
//
//  Created by Alexandre Arsenault on 2015-08-24.
//  Copyright (c) 2015 axLib. All rights reserved.
//

#ifndef __Minimal__eosBrowser__
#define __Minimal__eosBrowser__

#include "axLib/axLib.h"
#include "axLib/axScrollBar.h"
#include "axLib/axButton.h"

#include "axLib/axOSFileSystem.h"

namespace eos
{
    class FileBrowser : public ax::Window::Backbone
    {
    public:
        FileBrowser(const ax::Rect& rect):
        _font(0),
        _selected_index(-1),
        _file_start_index(0)
        {
		
			win = ax::Window::Create(rect);
			
			win->event.OnPaint = ax::WBind<ax::GC>(this, &FileBrowser::OnPaint);
			win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &FileBrowser::OnMouseLeftDown);
			win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &FileBrowser::OnMouseLeftUp);
			
            _folder_img = new ax::Image("resources/1441961651_folder.png");//folder_blue_15x15.png");
            _file_img = new ax::Image("resources/1441961890_text.png");//file_15x15.png");
            
            
            //_dir.Goto("/Users/alexarse/Project/axFrameworks");
//            _dir.Goto("/home/pi/");
            _dir.Goto("/");
            
            const std::vector<ax::os::File> files = _dir.GetContent();
            
            
//            ax::ScrollBar::Info scrollInfo(ax::Color(0.6), // Normal.
//                                       ax::Color(0.7), // Hover.
//                                       ax::Color(0.5), // Clicked.
//                                       ax::Color(0.0), // Slider contour.
//                                       ax::Color(0.2, 0.4), // Contour.
//                                       ax::Color(0.3), // Bg top.
//                                       ax::Color(0.45)); // Bg bottom.
			
            ax::ScrollBar::Events scrollEvents;
            scrollEvents.value_change = GetOnScroll();
			
			
	
//            _scrollBar = new axScrollBar(this, nullptr,
//                                         ax::Rect(rect.size.x - 10, 0, 10, rect.size.y),
//                                         scrollEvents,
//                                         scrollInfo);
			
//			win->node.Add
			
            
//            int h_size = (int)_dir.GetContent().size() * 20;
//            _scrollBar->SetPanelSize(ax::Size(rect.size.x, h_size));
			
        }
        
        void ResetParams()
        {
            _selected_index = -1;
            _file_start_index = 0;
            
            int h_size = (int)_dir.GetContent().size() * 20;
            
//            _scrollBar->SetPanelSize(ax::Size(GetSize().x, h_size));
        }
        
        std::string GetPath() const
        {
            return _dir.GetPath();
        }
        
        void GoBack()
        {
            _dir.GoBack();
            ResetParams();
            
            
//            ax::Print("Should go back");
//            
//            ax::Print("File folder :", _dir.GetPath());
            win->Update();
            
        }
        
        
    private:
        ax::Font _font;
        ax::os::Directory _dir;
        ax::Image *_folder_img, *_file_img;
        
        int _file_start_index;
        int _selected_index;
        std::shared_ptr<ax::ScrollBar> _scrollBar;
        
        axEVENT_ACCESSOR(ax::ScrollBar::Msg, OnScroll)
        void OnScroll(const ax::ScrollBar::Msg& msg)
        {
//            ax::Size w_size(win->dimension.GetSize());
//            int n_shown = w_size.y / 20;
//            int diff = (int)_dir.GetContent().size() - n_shown;
//            
//            double scroll_ratio = msg.GetSender()->GetZeroToOneValue();
//            _file_start_index = scroll_ratio * diff;
//            
//            Update();
        }
        
        void OnMouseLeftDown(const ax::Point& mouse_pos)
        {
            
        }
        
        void OnMouseLeftUp(const ax::Point& mouse_pos)
        {
            ax::Point pos = mouse_pos - win->dimension.GetAbsoluteRect().position;
            ax::Size w_size(win->dimension.GetSize());
            
            int n_shown = w_size.y / 20;
            
            _selected_index = (pos.y / (double)w_size.y) * n_shown;
            
            int index = _file_start_index + _selected_index;
            
            const std::vector<ax::os::File>& content = _dir.GetContent();
            
            if(index < content.size())
            {
                if(content[index].type == ax::os::File::FOLDER)
                {
                    _dir.Goto(_dir.GetPath() + _dir.GetContent()[index].name + "/");
                    ResetParams();
                }
            }
            
            win->Update();
        }
        
        void OnPaint(ax::GC gc)
        {
            ax::Rect rect(win->dimension.GetDrawingRect());
            
            gc.SetColor(ax::Color(0.95));
            gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
            
            gc.SetColor(ax::Color(0.0));
            
            // Draw file content.
            ax::Point str_pos(22, 3);
            
            const std::vector<ax::os::File>& files = _dir.GetContent();
            
            int n_shown = rect.size.y / 20;
            
            //            ;// = nullptr;
            
            for(int i = 0; i < files.size() && i < n_shown; i++)
            {
                ax::Image* icon_img = nullptr;
                const ax::os::File& file = files[i + _file_start_index];
                
                if(file.type == ax::os::File::FileType::FOLDER)
                {
                    icon_img = _folder_img;
                }
                else
                {
                    icon_img = _file_img;
                    
                }
                
                if(_selected_index == i)
                {
                    gc.SetColor(ax::Color(0.2f, 0.2f, 0.8f));
                }
                else if(i % 2)
                {
                    gc.SetColor(ax::Color(0.95));
                    
                }
                else
                {
                    gc.SetColor(ax::Color(0.90));
                }
                
                gc.DrawRectangle(ax::Rect(0, str_pos.y, rect.size.x - 10, 20));
                
                if(icon_img)
                {
                    gc.DrawImage(icon_img, ax::Point(3, str_pos.y));
                }
                
                
                //                if(file.type == ax::os::File::FOLDER)
                //                {
                //                    gc.DrawImage(_folder_img, ax::Point(3, str_pos.y));
                //                }
                
                if(_selected_index == i)
                {
                    gc.SetColor(ax::Color(1.0));
                }
                else
                {
                    gc.SetColor(ax::Color(0.0));
                }
                
                
                gc.DrawString(_font, file.name, str_pos);
                
                str_pos += ax::Point(0, 20);
            }
        }
    };
    
    class Browser : public ax::Window::Backbone
    {
    public:
        Browser(const ax::Rect& rect):
        _font(0)
        {
			win = ax::Window::Create(rect);
			win->event.OnPaint = ax::WBind<ax::GC>(this, &Browser::OnPaint);
			
            ax::Button::Info fbtn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
                                       ax::Color(1.0f, 0.0f, 0.0f),
                                       ax::Color(0.95f, 0.0f, 0.0f),
                                       ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
                                       ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
                                       ax::Color(0.0f, 0.0f, 0.0f),
                                       0);

//            win->node.Add(ax::Button::Ptr(new ax::Button(ax::Rect(40, 4, 20, 20),
//                                             GetOnButtonBack(),
//                                             fbtn_info, "resources/go10.png", "")));
			
            _file_browser = std::shared_ptr<eos::FileBrowser>(new eos::FileBrowser(ax::Rect(160, 0, rect.size.x - 160, rect.size.y)));
			
			win->node.Add(_file_browser);
            
            ax::Button::Info btn_info(ax::Color(0.5f, 0.5f, 0.5f),
                                      ax::Color(0.6f, 0.6f, 0.6f),
                                      ax::Color(0.4f, 0.4f, 0.4f),
                                      ax::Color(0.5f, 0.5f, 0.5f),
                                      ax::Color(0.4f, 0.4f),
                                      ax::Color(0.9f),
                                      0);
            
            ax::Point pos(0, 0);
            ax::Point delta(0, 25);
            
            ax::StringVector folders = {"Computer", "Home", "Desktop"};
            
            for(auto& n : folders)
            {
                win->node.Add(ax::Button::Ptr(new ax::Button(
                                                  ax::Rect(pos, ax::Size(158, 24)),
                                                  ax::Button::Events(),
                                                  btn_info,
                                                  "", n)));
                pos += delta;
            }
            
            
        }
		
		axEVENT_ACCESSOR(ax::Button::Msg, OnButtonBack);
        
    private:
        ax::Font _font;
        std::shared_ptr<eos::FileBrowser> _file_browser;
        
		
        void OnButtonBack(const ax::Button::Msg& msg)
        {
            ax::Print("Back");
            _file_browser->GoBack();
        }
        
        void OnPaint(ax::GC gc)
        {
            ax::Rect rect(win->dimension.GetDrawingRect());
            
//            gc.SetColor(ax::Color(0.95));
//            gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
            
            
            gc.SetColor(ax::Color(0.4, 0.7));
            gc.DrawRectangle(ax::Rect(0, 00, 160, rect.size.y));
            //gc.SetColor(ax::Color(0.0));
            //gc.DrawString(_font, _file_browser->GetPath(), ax::Point(45, 8));
            
//            gc.SetColor(ax::Color(0.0));
//            gc.DrawRectangleContour(ax::Rect(0, 0, rect.size.x, 30));
            
        }
    };
};

#endif /* defined(__Minimal__eosBrowser__) */
