#include <axLib/axLib.h>
#include <axLib/axButton.h>
#include <axLib/axTimer.h>

#include <fstream>

//#include "eosFrame.h"
#include "eosTextEditor.h"
#include "eosFrame.h"

// class TextEditorMenu : public ax::Window::Backbone
//{
// public:
//    TextEditorMenu(const ax::Rect& rect):
//    {
//		win = ax::Window::Create(rect);
//
////        ax::Button::Info menu_btn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
////                                       ax::Color(1.0f, 0.0f, 0.0f, 0.0f),
////                                       ax::Color(0.95f, 0.0f, 0.0f, 0.0f),
////                                       ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
////                                       ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
////                                       ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
////                                       0);
//
////        new ax::Button(this, ax::Rect(10, 10, 48, 48),
////                       ax::Button::Events(GetOnInfoClick()),
////                       menu_btn_info,
////                       "resource/info.png", "",
////                       ax::Button::Flags::SINGLE_IMG);
//    }
//
// private:
//
//    axEVENT_ACCESSOR(ax::Button::Msg, OnInfoClick);
//    void OnInfoClick(const ax::Button::Msg& msg)
//    {
//        ax::Print("INFO CLICK.");
//    }
//
//    void OnPaint()
//    {
//        ax::GC gc;
//        ax::Rect rect(GetDrawingRect());
//
//        gc.SetColor(ax::Color(1.0, 0.2));
//        gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//
//        gc.SetColor(ax::Color(0.05, 0.6));
//        gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//    }
//};
//
// class MainPanel : public axPanel
//{
// public:
//    MainPanel(ax::Window* parent, const ax::Rect& rect):
//    // Parent.
//    axPanel(parent, rect)
//    {
//        ax::Rect r(1, 20, rect.size.x - 1, rect.size.y - 20);
//
//        eos::TextEditor::Info txt_info;
////        txt_info.text_color = ax::Color(0.9647059, 0.9686275, 0.9333333);
////        txt_info.bg_color = ax::Color(0.1137255, 0.1176471, 0.09803922);
////        txt_info.line_number_color = ax::Color();
//
//        _editor = new eos::TextEditor(this, r, txt_info);
//    }
//
// private:
//    eos::TextEditor* _editor;
//
//    void OnResize(const ax::Size& size)
//    {
//        SetSize(size);
//        _editor->Resize(ax::Size(size.x - 1, size.y - 20));
//    }
//
//    void OnPaint()
//    {
//        ax::GC gc;
//        ax::Rect rect(GetDrawingRect());
//
//        gc.SetColor(ax::Color(0.96));
//        gc.DrawRectangle(rect);
//
//        gc.SetColor(ax::Color(0.4));
//        gc.DrawRectangleContour(rect);
//    }
//};

// extern "C"
//{
//	axWindow* StartApplication(axWindow* parent, ax::Rect rect)
//	{
//		ax::Print("Start TextEditor.");
//		rect.size = ax::Size(500, 500);
//
//		eos::Frame* frame = new eos::Frame(parent, rect, "TextEditor");
//		frame->AddProperty("Resizable");
//
//		MainPanel* txt_edit = new MainPanel(frame, frame->GetChildRect());
//
//		frame->SetChildHandler(txt_edit);
//
//        TextEditorMenu* menu = new TextEditorMenu(frame, frame->GetChildRect());
//        frame->SetChildMenuHandler(menu);
//
//		return frame;
//	}
//}

extern "C"
{
	ax::Window::Backbone* StartApplication(ax::Window* parent, ax::Rect rect)
	{
		ax::Size fixe_size(500, 500);
		eos::Frame* frame = new eos::Frame(ax::Rect(rect.position, fixe_size),
										   "TextEditor");
		frame->GetWindow()->property.AddProperty("Resizable");
		
		eos::TextEditor::Info txt_info;
		txt_info.text_color = ax::Color(0.6647059f, 0.686275f, 0.6333333f);
		txt_info.bg_color = ax::Color(0.1137255f, 0.1176471f, 0.09803922f);
		txt_info.line_number_color = ax::Color();
		std::shared_ptr<eos::TextEditor> txtEdit(new eos::TextEditor(frame->GetChildRect(), txt_info));
		
		frame->GetWindow()->node.Add(txtEdit);
		frame->SetChildHandler(txtEdit->GetWindow());
		
		//		TerminalMenu* menu = new TerminalMenu(frame, frame->GetChildRect());
		//		frame->SetChildMenuHandler(menu);
		
		return frame;
	}
}

int main()
{
	ax::App& app = ax::App::GetInstance();

	ax::GC gc;
	gc.shader_normal = ax::GL::Shader("resources/vertex_shader.glsl",
		"resources/fragments_shader.glsl");

	gc.shader_fb = ax::GL::Shader("resources/fb_vertex_shader.glsl",
		"resources/fb_fragments_shader.glsl");

	gc.shader_font = ax::GL::Shader("resources/font_vertex_shader.glsl",
			"resources/font_fragments_shader.glsl");

	app.AddMainEntry([&app]() {

		ax::GC gc;
		gc.shader_normal.CompileAndLink();
		gc.shader_fb.CompileAndLink();
		gc.shader_font.CompileAndLink();

		gc.shader_fb.Activate();

		eos::TextEditor::Info txt_info;
		txt_info.text_color = ax::Color(0.6647059f, 0.686275f, 0.6333333f);
		txt_info.bg_color = ax::Color(0.1137255f, 0.1176471f, 0.09803922f);
		txt_info.line_number_color = ax::Color();
		std::shared_ptr<eos::TextEditor> win(new eos::TextEditor(ax::Rect(0, 0, 512, 384), txt_info));

		win->GetWindow()->dimension.GetFrameBuffer()->AssignCustomFBDrawFunction(
			[win](ax::GL::FrameBuffer& fb) {
				glEnable(GL_TEXTURE_2D);

				glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

				ax::FloatPoint pos(0.0, 0.0);
				const ax::Size& ss(win->GetWindow()->dimension.GetShownRect().size);
				ax::FloatSize size(ss.x, ss.y);

				// Bind framebuffer texture.
				glBindTexture(
					GL_TEXTURE_2D, win->GetWindow()->dimension.GetFrameBuffer()->GetFrameBufferTexture());
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				float vertices[8] = { pos.x, pos.y, pos.x, pos.y + size.y, pos.x + size.x, pos.y + size.y,
					pos.x + size.x, pos.y };

				float tex_coords[8] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };

				glVertexPointer(2, GL_FLOAT, 0, vertices);
				glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				glDisable(GL_TEXTURE_2D);
			});

		app.AddTopLevel(win);
	});

	app.AddAfterGUILoadFunction([&app]() { app.SetFrameSize(ax::Size(512, 384)); });

	app.MainLoop();

	return 0;
}
