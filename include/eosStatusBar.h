#ifndef __EOS_STATUS_BAR_H__
#define __EOS_STATUS_BAR_H__

#include <ctime>
#include "axLib/axLib.h"
#include "axLib/axTimer.h"
#include "axLib/axButton.h"

#include "eosCoreSystem.h"
#include "eosClock.h"
#include "eosFrame.h"

//#include "axLib/axGLShader.h"

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
		bool _is_down;
		
		std::shared_ptr<eos::Clock> _clock;
		
		std::pair<bool, std::string> _fullscreen_frame;
		
		ax::Image* _setting_img;
		ax::Font _font;
		//ax::GL::Shader _shader;
		std::shared_ptr<ax::Image> _bg_img;
		
		std::thread _zoom_in_thread;
		
		void OnView(ax::Event::Msg* msg);
		
		void OnNotificationMode(ax::Event::Msg* msg);
		
		void OnTerminalMode(ax::Event::Msg* msg);
		
		void OnTraceMode(ax::Event::Msg* msg);

		void OnHome(ax::Event::Msg* msg);
		
		void OnApp3D(ax::Event::Msg* msg);
		
		void OnAppViewer(ax::Event::Msg* msg);
		
		void OnFrameFullScreen(ax::Event::Msg* msg);
		void OnFrameUnFullScreen(ax::Event::Msg* msg);
		
		void OnSettings(ax::Event::Msg* msg);
		
		void OnMouseEnter(const ax::Point& pos);
		
		void OnMouseLeave(const ax::Point& pos);
		
		void OnPaint(ax::GC gc);
	};
}

#endif // __EOS_STATUS_BAR_H__
