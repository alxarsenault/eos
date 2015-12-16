#ifndef __EOS_DOCK_H__
#define __EOS_DOCK_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axTimer.h"

#include "eosAppLoader.h"
#include "eosDesktop.h"
#include "eosFrame.h"

namespace eos {
	class DockIcon;
	
	/*
	 * eos::Dock
	 */
	class Dock : public ax::Window::Backbone
	{
	public:
		Dock(const ax::Rect& rect);
		
		bool IsDrop() const;
		
	private:
		ax::Rect _drop_rect, _up_rect;
		int _timer_interval, _timer_length;
		float _anim_percent;
		bool _anim_active, _anim_up;
		
		ax::Event::Timer* _timer_up;
		ax::Event::Timer* _timer_down;
		
		//ax::GL::Shader _shader;
		std::shared_ptr<ax::Image> _bg_img;
		
		bool _isDrop;
		std::map<std::string, AppLoader> _appLoaders;
		std::vector<eos::DockIcon*> _app_icons;
		
		axEVENT_ACCESSOR(ax::Event::StringMsg, OnAppSelect);
		void OnAppSelect(const ax::Event::StringMsg& msg);
		
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowMinimize);
		void OnWindowMinimize(const eos::Frame::Msg& msg);
		
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowClose);
		void OnWindowClose(const eos::Frame::Msg& msg);
		
		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowFullScreen);
		void OnWindowFullScreen(const eos::Frame::Msg& msg);
		
		axEVENT_ACCESSOR(ax::Event::Timer::Msg, OnAnimationTimerUp);
		void OnAnimationTimerUp(const ax::Event::Timer::Msg& msg);
		
		axEVENT_ACCESSOR(ax::Event::Timer::Msg, OnAnimationTimerDown);
		void OnAnimationTimerDown(const ax::Event::Timer::Msg& msg);
		
		void OnMouseEnter(const ax::Point& mouse);
		void OnMouseLeave(const ax::Point& mouse);
		
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_DOCK_H__
