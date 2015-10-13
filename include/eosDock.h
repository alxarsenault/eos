//#ifndef __EOS_DOCK_H__
//#define __EOS_DOCK_H__
//
//#include "axLib/axLib.h"
//#include "axLib/axButton.h"
//#include "axLib/axTimer.h"
//
//#include "eosAppLoader.h"
//#include "eosFrame.h"
//
////#include "eosWindowManager.h"
//#include "eosDesktop.h"
//
//namespace eos
//{
//    /**************************************************************************
//     * eos::DockIcon.
//     *************************************************************************/
//	class DockIcon : public ax::Button
//	{
//	public:
//		DockIcon(ax::Window* parent,
//				const ax::Rect& rect,
//				const ax::Button::Events& events,
//				const ax::Button::Info& info,
//				const std::string& img_path,
//				const std::string& msg);
//
//		void SetAnimationPercent(const double& percent);
//
//	private:
//		double _anim_percent;
//
//		virtual void OnMouseLeave();
//		virtual void OnPaint();
//	};
//
//	/**************************************************************************
//	 * eos::Dock
//	 *************************************************************************/
//    class Dock : public axPanel
//	{
//	public:
//		Dock(ax::Window* parent, const ax::Rect& rect);
//
//		bool IsDrop() const;
//
//	private:
//		ax::Rect _drop_rect, _up_rect;
//		int _timer_interval, _timer_length;
//		float _anim_percent;
//		bool _anim_active, _anim_up;
//		ax::Event::Timer* _timer;
//		ax::Event::Timer* _timer_down;
//
//		bool _isDrop;
//		std::map<std::string, AppLoader> _appLoaders;
//		std::vector<eos::DockIcon*> _app_icons;
//
//		axEVENT_ACCESSOR(ax::Button::Msg, OnAppSelect); 	
//		void OnAppSelect(const ax::Button::Msg& msg);
//
//		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowMinimize);
//		void OnWindowMinimize(const eos::Frame::Msg& msg);
//
//		axEVENT_ACCESSOR(eos::Frame::Msg, OnWindowClose);
//		void OnWindowClose(const eos::Frame::Msg& msg);
//		
//		axEVENT_ACCESSOR(ax::Event::Timer::Msg, OnAnimationTimer);
//		void OnAnimationTimer(const ax::Event::Timer::Msg& msg);
//
//		axEVENT_ACCESSOR(ax::Event::Timer::Msg, OnAnimationTimerDown);
//		void OnAnimationTimerDown(const ax::Event::Timer::Msg& msg);
//		
//		virtual void OnMouseEnter();
//		virtual void OnMouseLeave();
//
//		virtual void OnPaint();
//	};
//}
//
//#endif //__EOS_DOCK_H__
