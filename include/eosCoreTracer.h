//#ifndef __EOS_CORE_TRACE_H__
//#define __EOS_CORE_TRACE_H__
//
//#include "axLib/axLib.h"
//#include <fstream>
////#include <ctime>
//#include <sys/time.h>
//#include <sstream>
//#include <iomanip>
//
//namespace eos
//{
//    namespace Core
//    {
//        class Tracer : public ax::Event::Object
//        {
//        public:
//            Tracer(ax::Event::Manager* evt_manager);
//            
//            ~Tracer();
//            
//            const std::string GetTraceFilePath() const;
//            
//            template <typename T, typename ...P>
//            void Write(T t, P ...p)
//            {
//                _fstream << GetTimeStamp() << " - " << t << ' ';
//                {
//                    Print(p...);
//                }
//            }
//
//        private:
//            std::string _f_name;
//            std::fstream _fstream;
//            
//            template <typename T, typename ...P>
//            void Print(T t, P ...p)
//            {
//                _fstream << t << ' ';
//                {
//                    Print(p...);
//                }
//            }
//            
//            void Print();
//            
//            inline std::string GetTimeStamp() const
//            {
//                timeval tv;
//                time_t t = time(0);
//                gettimeofday(&tv, NULL);
//                tm* now = localtime(&t);
//                
//                int hours = now->tm_hour;
//                int minutes = (int)((tv.tv_sec % 3600) / 60);
//                int seconds = (int)(tv.tv_sec % 60);
//                int ms = (int)(tv.tv_usec / 1000);
//                
//                std::ostringstream oss;
//                oss << std::right << std::setw(2) << std::setfill('0') << hours << ':'
//                << std::right << std::setw(2) << std::setfill('0') << minutes << ':'
//                << std::right << std::setw(2) << std::setfill('0') << seconds << ':'
//                << std::right << std::setw(3) << std::setfill('0') << ms;
//                
//                return oss.str();
//            }
//        };
//    }
//}
//
//#endif //__EOS_CORE_TRACE_H__
