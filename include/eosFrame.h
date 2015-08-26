#include "axLib/axLib.h"
#include "axLib/axButton.h"

namespace eos
{
	class Frame : public axPanel
	{
	public:
		Frame(axWindow* parent, 
			  const ax::Rect& rect, 
			  const std::string& window_name);
	
		ax::Rect GetChildRect() const;

		void SetChildHandler(axWindow* child);
   
		// Property.
		// Resizable

	private:
		axWindow* _child;
		std::string _window_name;
		ax::Button *_close_btn, *_min_btn;
		ax::Size _click_size;
		int _frame_status;
		ax::Font _font;
		ax::Point _click_pos, _abs_click_pos;;
		bool _highlight;


		axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
		void OnButtonClick(const ax::Button::Msg& msg);
	
		axEVENT_ACCESSOR(ax::Button::Msg, OnMinimize);
		void OnMinimize(const ax::Button::Msg& msg);

		void OnMouseLeave();

		void OnMouseMotion(const ax::Point& pos);

		void OnMouseLeftDown(const ax::Point& pos);

		void OnMouseLeftUp(const ax::Point& pos);

		void OnMouseLeftDragging(const ax::Point& pos);

		void OnPaint();
	};
}
