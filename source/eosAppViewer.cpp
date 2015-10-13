//#include "eosAppViewer.h"
//#include "eosDesktop.h"
//
//eos::AppViewer::AppViewer(ax::Window* parent, const ax::Rect& rect):
//// Parent.
//axPanel(parent, rect)
//{
//    ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
//                              ax::Color(1.0, 0.0, 0.0, 0.0),
//                              ax::Color(0.95, 0.0, 0.0, 0.0),
//                              ax::Color(0.5, 0.5, 0.5, 0.0),
//                              ax::Color(0.0, 0.0, 0.0, 0.0),
//                              ax::Color(0.0, 0.0, 0.0, 0.0),
//                              0);
//    
//    ax::Size icon_size(64, 64);
//    ax::Point icon_pos(10, 10);
//    
//    ax::StringPairVector apps_icon_info =
//    {
//        ax::StringPair("resource/1441952844_computer.png", "browser"),
//        ax::StringPair("resource/1441952725_terminal.png", "calc"),
//        ax::StringPair("resource/1441953050_image.png", "txtedit"),
//        ax::StringPair("resource/1441952856_calendar.png", "term"),
//        ax::StringPair("resource/1441952883_book.png", "viewer"),
//        ax::StringPair("resource/1441953077_notepad.png", "browser"),
//        ax::StringPair("resource/1441953272_enveloppe-alt.png", "calc"),
//        ax::StringPair("resource/1441953912_wrench-screwdriver.png", "txtedit"),
//        ax::StringPair("resource/1441951759_calculator.png", "term"),
//        ax::StringPair("resource/1441955424_address-book-alt.png", "viewer"),
//        ax::StringPair("resource/1441955471_movie-alt.png", "browser"),
//        ax::StringPair("resource/1441953867_social-facebook.png", "calc")
//    };
//    
//    for(auto& n : apps_icon_info)
//    {
//        ax::Button* btn = new ax::Button(this, ax::Rect(icon_pos, icon_size),
//                                         ax::Button::Events(),
//                                         btn_info, n.first, "",
//                                         ax::Button::Flags::SINGLE_IMG);
//        
//        icon_pos = btn->GetRect().GetNextPosRight(5);
//        
//        if(icon_pos.x > rect.size.x - 70)
//        {
//            icon_pos.x = 10;
//            icon_pos.y += 80;
//            
//        }
//    }
//    
//    
//    axScrollBarInfo sb_info(ax::Color(0.36), // Normal.
//                            ax::Color(0.38), // Hover.
//                            ax::Color(0.34), // Clicked.
//                            ax::Color(0.3), // Slider contour.
//                            ax::Color(0.36), // Contour.
//                            ax::Color(0.96), // Bg top.
//                            ax::Color(0.90)); // Bg bottom.
//    
//    axScrollBarEvents sb_evts;
////    sb_evts.value_change = GetOnScroll();
//    
//    ax::Rect sb_rect(rect.size.x - 9, 0, 8, rect.size.y - 1);
//    axScrollBar* sb = new axScrollBar(this, nullptr, sb_rect, sb_evts, sb_info);
//    
//    
//}
//
//void eos::AppViewer::OnPaint()
//{
//    ax::GC gc;
//    ax::Rect rect(GetDrawingRect());
//    
//    gc.SetColor(ax::Color(0.7, 0.4));
////    ax::Color dock_color(0.7, 0.4);
//    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//    
//    gc.SetColor(ax::Color(0.4, 0.5));
//    gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
//}
