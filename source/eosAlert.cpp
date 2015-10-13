//#include "eosAlert.h"
//
//#include "eosDesktop.h"
//
//#include "eosCoreSystem.h"
//#include "eosCoreTracer.h"
//
//
///*******************************************************************************
// * eos::Frame.
// ******************************************************************************/
//eos::Alert::Alert(ax::Window* parent,
//                  const ax::Rect& rect,
//                  eos::Core::System* system):
//// Parent.
//axPanel(parent, rect),
//_font("resource/DejaVuSansMono.ttf"),
//_time_font("resource/DejaVuSansMono.ttf"),
//_system(system)
//{
//    _time_font.SetFontSize(10);
//    _tracer_stream.open(_system->GetTracer()->GetTraceFilePath(), std::fstream::in);
//    _system->GetTracer()->AddConnection(0, GetOnTracer());
//    _read_pos = 0;
//    
//    OnTracer(ax::Event::SimpleMsg<int>(0));
//}
//
//void eos::Alert::OnTracer(const ax::Event::SimpleMsg<int>& msg)
//{
//    _tracer_stream.clear();
//    _tracer_stream.seekg(_read_pos, _tracer_stream.beg);
//    
//    while(!_tracer_stream.eof())
//    {
//        _read_pos = _tracer_stream.tellg();
//        
//        std::string tmp;
//        std::getline(_tracer_stream, tmp);
//        
//        if(!tmp.empty())
//        {
//            std::size_t time_pos = tmp.find('-');
//            std::string time = tmp.substr(0, time_pos);
//            std::string content = tmp.substr(time_pos);
//            
//            _trace_time.push_back(time);
//            _trace.push_back(content);
//            
//        }
//    }
//    
//    Update();
//}
//
//int DrawString(ax::GC& gc, ax::Font& font, const std::string text, const ax::Point& pos)
//{
//    int x = pos.x;
//    
//    for (int i = 0; i < text.size(); i++)
//    {
//        font.SetChar(text[i]);
//        ax::Point delta = font.GetDelta();
//        
//        gc.DrawTexture(font.GetTexture(),
//                       ax::Rect(ax::Point(x + delta.x, pos.y - delta.y + font.GetFontSize()),
//                                font.GetSize()));
//        
//        x += font.GetNextPosition();
//    }
//    
//    return x;
//}
//
//void eos::Alert::OnPaint()
//{
//    ax::GC gc;
//    ax::Rect rect(GetDrawingRect());
//    
//    gc.SetColor(ax::Color(0.4, 0.4));
//    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//    
//    ax::Point pos(5, 5), delta(0, 15), tdelta(0, 2);
//    for(int i = 0; i < _trace.size(); i++)
//    {
//        gc.SetColor(ax::Color(0.0, 0.8, 0.0));
//        int x = DrawString(gc, _time_font, _trace_time[i], pos + tdelta);
//        
//        gc.SetColor(ax::Color(0.9));
//        gc.DrawString(_font, _trace[i], ax::Point(x, pos.y));
//        pos += delta;
//    }
//    
//    gc.SetColor(ax::Color(0.4, 0.5));
//    gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
//   
//}
//
//
//    
