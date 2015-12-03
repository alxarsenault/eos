#ifndef __EOS_CORE_TRACE_H__
#define __EOS_CORE_TRACE_H__

#include "axLib/axLib.h"
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <iomanip>

namespace eos {
	namespace Core {
		class Tracer : public ax::Event::Object
		{
		public:
			Tracer();
			
			~Tracer();
			
			const std::string GetTraceFilePath() const;
			
			template <typename T, typename ...P>
			void Write(T t, P ...p)
			{
				_fstream << GetTimeStamp() << " - " << t << ' ';
				Print(p...);
			}
			
		private:
			std::string _f_name;
			std::fstream _fstream;
			
			template <typename T, typename ...P>
			void Print(T t, P ...p)
			{
				_fstream << t << ' ';
				Print(p...);
			}
			
			void Print();
			
			std::string GetTimeStamp() const;
		};
	}
}

#endif //__EOS_CORE_TRACE_H__
