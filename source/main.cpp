#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include "eosStatusBar.h"
#include "eosFrame.h"
#include "eosDock.h"

class MainPanel : public axPanel
{
public:
    MainPanel(ax::App* app, const ax::Rect& rect):
    // Parent.
    axPanel(app, rect)
    {
		_bg_img = new ax::Image("resource/wallpaper-standard.png");
		
		ax::Rect status_rect(0, 0, rect.size.x, 25);		
		eos::StatusBar* status = new eos::StatusBar(this, status_rect);

		ax::Rect dock_rect(100, rect.size.y - 64 - (2 * 5), rect.size.x - 200, 64 + 2 * 5);
		eos::Dock* dock = new eos::Dock(this, dock_rect);
    }
   
private:
	ax::Image* _bg_img;

    void OnPaint()
	{
		ax::GC gc;
        ax::Rect rect(GetDrawingRect());  
		
		gc.DrawImageResize(_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));
   }
};

int main()
{
	ax::App app;

	app.AddMainEntry([&app]()
	{
		new MainPanel(&app, ax::Rect(ax::Point(0, 0), app.GetScreenSize()));
	});

	app.MainLoop();

	return 0;
}
