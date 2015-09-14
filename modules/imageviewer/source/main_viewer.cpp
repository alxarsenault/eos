#include "axLib/axLib.h"

#include "eosFrame.h"
#include "eosImageViewer.h"

extern "C"
{
	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
	{
		ax::Size fixe_size(500, 500);
		eos::Frame* frame = new eos::Frame(parent, 
										   ax::Rect(rect.position, fixe_size), 
										   "Terminal");
		frame->AddProperty("Resizable");

		eos::ImageViewer* viewer = new eos::ImageViewer(frame,
                                                        frame->GetChildRect(),
                                                        "");
		frame->SetChildHandler(viewer);
		
		return frame;
	}
}
