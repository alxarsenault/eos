#ifndef __EOS_ALERT_H__
#define __EOS_ALERT_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"

namespace eos
{
	class Alert : public axPanel
	{
	public:
		Alert(axWindow* parent,
			  const ax::Rect& rect, 
			  const std::string& alert_message);
	
	private:
        std::string _alert_str;
		ax::Font _font;
		
		void OnPaint();
	};
}

#endif //__EOS_ALERT_H__
