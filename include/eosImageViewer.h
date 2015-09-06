#ifndef __EOS_IMAGE_VIEWER_H__
#define __EOS_IMAGE_VIEWER_H__

#include "axLib/axLib.h"

namespace eos
{
    /***************************************************************************
     * eos::ImageViewer.
     **************************************************************************/
    class ImageViewer : public axPanel
    {
    public:
        ImageViewer(axWindow* parent,
                    const ax::Rect& rect,
                    const std::string& img_path);
        
//        void Resize(const ax::Size& size);

    private:
        virtual void OnPaint();
    };
}

#endif // __EOS_IMAGE_VIEWER_H__