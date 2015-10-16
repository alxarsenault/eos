#ifndef __EOS_STATUS_BAR_H__
#define __EOS_STATUS_BAR_H__

#include <ctime>
#include "axLib/axLib.h"
#include "axLib/axTimer.h"
#include "axLib/axButton.h"

#include "eosCoreSystem.h"
#include "eosClock.h"

namespace eos {
	/*
	 * eos::StatusBar.
	 */
	class StatusBar : public ax::Window::Backbone
	{
	public:
		StatusBar(const ax::Rect& rect);
		
	private:
		std::string _user_name;
		
		std::shared_ptr<eos::Clock> _clock;
		
		ax::Image* _setting_img;
		ax::Font _font;
		
		void OnView(ax::Event::Msg* msg);
		
		void OnNotificationMode(ax::Event::Msg* msg);
		
		void OnTerminalMode(ax::Event::Msg* msg);
		
		void OnTraceMode(ax::Event::Msg* msg);

		void OnHome(ax::Event::Msg* msg);
		
		void OnAppViewer(ax::Event::Msg* msg);
		
		void OnSettings(ax::Event::Msg* msg);
		
		void OnPaint(ax::GC gc);
	};
}

#endif // __EOS_STATUS_BAR_H__
