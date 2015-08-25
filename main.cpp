#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include <dlfcn.h>

//extern int StartApplication(ax::App* app);

class axModuleLoader
{
public:
	typedef std::function<int(axWindow*, ax::Rect)> EntryFunction;
	
	axModuleLoader(const std::string& path)
	{
		_binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);

		if(_binHandle != nullptr)
		{
			_fctHandle = reinterpret_cast<int(*)(axWindow*, ax::Rect)>(dlsym(_binHandle, "StartApplication"));
		}
		else
		{
			std::cerr << "failed to load library" << std::endl;
		}
    }
			    
    ~axModuleLoader()
    {
        dlclose(_binHandle);
    }
				    
    EntryFunction GetFunctionHandle()
    {
        return _fctHandle;
    }
					    
private:
    void* _binHandle;
    EntryFunction _fctHandle;
};


class MainPanel : public axPanel
{
public:
    MainPanel(ax::App* app, const ax::Rect& rect):
    // Parent.
    axPanel(app, rect)
    {
		ax::Print("MainPanel");

		ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5),
								  ax::Color(0.6, 0.6, 0.6),
								  ax::Color(0.4, 0.4, 0.4),
								  ax::Color(0.5, 0.5, 0.5),
								  ax::Color(0.0, 0.0, 0.0),
								  ax::Color(0.0, 0.0, 0.0),
								  0);
		           	
		ax::Button* btn = new ax::Button(this, ax::Rect(500, 500, 70, 50),
										 ax::Button::Events(GetOnButtonClick()), 
										 btn_info, "", "Button");


		axToggle::Events tog_evts;

		//tog_evts.button_click = GetOnToggle();
    
		axToggle::Info tog_info;
		tog_info.normal = ax::Color(0.5, 0.5, 0.5, 1.0);
		tog_info.hover = tog_info.normal;
		tog_info.clicking = tog_info.normal;
    
		tog_info.selected = ax::Color(0.5, 0.5, 1.0, 1.0);
		tog_info.selected_hover = tog_info.selected;
		tog_info.selected_clicking = tog_info.selected; 
    
		tog_info.contour = ax::Color(0.0, 0.0, 0.0, 1.0);
		tog_info.font_color = ax::Color(0.0, 0.0, 0.0, 1.0);
		tog_info.img = "";//"./sdcard/Documents/43-Play-button 2.png";
		tog_info.single_img = true;
		tog_info.font_size = 16;
    
		axToggle* togg = new axToggle(this,
		                              ax::Rect(300, 10, 70, 70),
		                              tog_evts,
		                              tog_info, "", "Test");


		_bg_img = new ax::Image("F18-alpha-wallpaper-standard.png");
    }
   
private:
	axModuleLoader* _moduleLoader;
	ax::Image* _bg_img;

	axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
	void OnButtonClick(const ax::Button::Msg& msg)
	{
		ax::Print("Button click.");

		const std::string binaryFilePath = "/home/pi/Projects/axFrameworks/samples/pi/libmyaxapp.so";

		_moduleLoader = new axModuleLoader(binaryFilePath);

		axModuleLoader::EntryFunction fct = _moduleLoader->GetFunctionHandle();
				   
	   	if(fct)
		{
			fct(this, ax::Rect(500, 10, 500, 500));
		}

	}

    void OnPaint()
   {
        ax::GC gc;
        ax::Rect rect(GetDrawingRect());  

		//gc.SetColor(0.6, 0.6, 0.6);
        //gc.DrawRectangle(rect);
	
		//gc.SetColor(0.0, 1.0, 0.0);
		//gc.DrawRectangle(ax::Rect(10, 10, 50, 50));
		
		gc.DrawImageResize(_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));


		gc.SetColor(ax::Color(0.7, 0.7, 0.7, 0.4));
		gc.DrawRectangle(ax::Rect(0, rect.size.y - 50, rect.size.x, 50));
		//gc.SetColor(0.0, 0.0, 1.0);
		//gc.DrawRectangle(ax::Rect(100, 100, 50, 50)); 
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
