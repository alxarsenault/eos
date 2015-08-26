#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include <dlfcn.h>
#include "eosStatusBar.h"
#include "axLib/axAudioCore.h"
#include "axLib/axAudioWaveTable.h"
//extern int StartApplication(ax::App* app);

class axModuleLoader
{
public:
	typedef std::function<axWindow*(axWindow*, ax::Rect)> EntryFunction;
	
	axModuleLoader(const std::string& path)
	{
		_binHandle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);

		if(_binHandle != nullptr)
		{
			_fctHandle = reinterpret_cast<axWindow*(*)(axWindow*, ax::Rect)>(dlsym(_binHandle, "StartApplication"));
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

namespace eos
{
	class Dock : public axPanel
	{
	public:
		Dock(axWindow* parent, const ax::Rect& rect):
		// Parent.
		axPanel(parent, rect),
		_browser(nullptr),
		_calc(nullptr)
		{
				
			ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5),
									  ax::Color(0.6, 0.6, 0.6),
									  ax::Color(0.4, 0.4, 0.4),
									  ax::Color(0.5, 0.5, 0.5),
									  ax::Color(0.0, 0.0, 0.0),
									  ax::Color(0.0, 0.0, 0.0),
									  0);
		           	
			ax::Button* btn = new ax::Button(this, ax::Rect(10, 5, 32, 32),
											 GetOnFileClick(), 
											 btn_info, "FileMan_32x32.png", "",
											 ax::Button::Flags::SINGLE_IMG);


			ax::Button* cal = new ax::Button(this, ax::Rect(10 + 35, 5, 32, 32),
											 GetOnCalcClick(), 
											 btn_info, "FileMan_32x32.png", "",
											 ax::Button::Flags::SINGLE_IMG);;
		}


	private:
		axModuleLoader* _browser_module;
		axModuleLoader* _calc_module;
		axWindow* _browser;
		axWindow* _calc;

		axEVENT_ACCESSOR(ax::Button::Msg, OnFileClick);                                                         
		void OnFileClick(const ax::Button::Msg& msg)                                                            
		{
			if(_browser == nullptr)
			{
				_browser_module = new axModuleLoader("/home/pi/Projects/eos/app/browser.so"); 

				axModuleLoader::EntryFunction fct = _browser_module->GetFunctionHandle();
	                                   
				if(fct)             
				{
					_browser = fct(GetParent(), ax::Rect(500, 50, 500, 500));
					_browser->AddConnection(189, GetOnBrowserMinimize());
					_browser->AddConnection(190, GetOnBrowserClose());   
				}      
			}
			else
			{
				if(_browser->IsShown())
				{
					_browser->Hide();
				}
				else
				{
					_browser->Show();
				}
			}
		}	
	
		axEVENT_ACCESSOR(ax::Button::Msg, OnCalcClick);                                                         
		void OnCalcClick(const ax::Button::Msg& msg)                                                            
		{
			if(_calc == nullptr)
			{
				_calc_module = new axModuleLoader("/home/pi/Projects/eos/app/calculator.so"); 

				axModuleLoader::EntryFunction fct = _calc_module->GetFunctionHandle();
	                                   
				if(fct)             
				{
					//165, 255
					_calc = fct(GetParent(), ax::Rect(500, 50, 162 + 2 * 9, 255 + 25 + 9));
					//_calc->AddConnection(189, GetOnBrowserMinimize());
					//_calc->AddConnection(190, GetOnBrowserClose());   
				}      
			}
			else
			{
				if(_calc->IsShown())
				{
					_calc->Hide();
				}
				else
				{
					_calc->Show();
				}
			}
		}	
		axEVENT_ACCESSOR(ax::Button::Msg, OnBrowserMinimize);	
		void OnBrowserMinimize(const ax::Button::Msg& msg)
		{
			ax::Print("Minimize broser.");
		}


		axEVENT_ACCESSOR(ax::Button::Msg, OnBrowserClose);	
		void OnBrowserClose(const ax::Button::Msg& msg)
		{
			_browser = nullptr;
		}


		void OnPaint()
		{
			ax::GC gc;
			ax::Rect rect(GetDrawingRect());

			gc.SetColor(ax::Color(0.4, 0.4));
			gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
		}
	};
}
class MainPanel : public axPanel
{
public:
    MainPanel(ax::App* app, const ax::Rect& rect):
    // Parent.
    axPanel(app, rect)
    {

		_bg_img = new ax::Image("F18-alpha-wallpaper-standard.png");
		
		eos::StatusBar* status = new eos::StatusBar(this, ax::Rect(0, 0, rect.size.x, 25));
		eos::Dock* dock = new eos::Dock(this, ax::Rect(0, rect.size.y - 40, rect.size.x, 40));

		//ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5),
		//						  ax::Color(0.6, 0.6, 0.6),
		//						  ax::Color(0.4, 0.4, 0.4),
		//						  ax::Color(0.5, 0.5, 0.5),
		//						  ax::Color(0.0, 0.0, 0.0),
		//						  ax::Color(0.0, 0.0, 0.0),
		//						  0);
		           	
		//ax::Button* btn = new ax::Button(this, ax::Rect(500, 500, 70, 50),
		//								 ax::Button::Events(GetOnButtonClick()), 
		//								 btn_info, "", "Button");


//		axToggle::Events tog_evts;

		//tog_evts.button_click = GetOnToggle();
    
//		axToggle::Info tog_info;
//		tog_info.normal = ax::Color(0.5, 0.5, 0.5, 1.0);
//		tog_info.hover = tog_info.normal;
//		tog_info.clicking = tog_info.normal;
    
//		tog_info.selected = ax::Color(0.5, 0.5, 1.0, 1.0);
//		tog_info.selected_hover = tog_info.selected;
//		tog_info.selected_clicking = tog_info.selected; 
    
//		tog_info.contour = ax::Color(0.0, 0.0, 0.0, 1.0);
//		tog_info.font_color = ax::Color(0.0, 0.0, 0.0, 1.0);
//		tog_info.img = "";//"./sdcard/Documents/43-Play-button 2.png";
//		tog_info.single_img = true;
//		tog_info.font_size = 16;
    
//		axToggle* togg = new axToggle(this,
//		                              ax::Rect(300, 10, 70, 70),
//		                              tog_evts,
//		                              tog_info, "", "Test");



		//axModuleLoader* module = new axModuleLoader("/home/pi/Projects/eos/app/browser.so");
		//axModuleLoader::EntryFunction fct = module->GetFunctionHandle();
	                                   
		//if(fct)             
        //{
		//	fct(this, ax::Rect(500, 50, 500, 500));
		//}      
    }
   
private:
	axModuleLoader* _moduleLoader;
	ax::Image* _bg_img;

    void OnPaint()
	{
		ax::GC gc;
        ax::Rect rect(GetDrawingRect());  
		
		gc.DrawImageResize(_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));
   }
};

class Audio : public ax::Audio::Core
{
	public:
		Audio():
			ax::Audio::Core()
		{
			_waveTable = new axAudioWaveTable();
			_waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SINE);
			_waveTable->SetFreq(440.0);
		}

	

		virtual int CoreCallbackAudio(const float* input,
			                          float* output,
									  unsigned long frameCount)
		{
			_waveTable->ProcessBlock(output, frameCount);
			return 0;
		}
	private:
		axAudioWaveTable* _waveTable;

};


int main()
{
	ax::App app;
//	Audio audio;

//	audio.InitAudio();
//	audio.StartAudio();

	app.AddMainEntry([&app]()
	{
		new MainPanel(&app, ax::Rect(ax::Point(0, 0), app.GetScreenSize()));
	});

	app.MainLoop();

	return 0;
}
