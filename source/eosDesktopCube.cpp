#include "eosDesktopCube.h"

namespace eos {
DesktopCube::DesktopCube(const ax::Rect& rect)
	: _cube_face_selected(1)
{
	win = ax::Window::Create(
		ax::Rect(ax::Point(100, 100), rect.size - ax::Size(200, 200)));

	win->event.OnPaint3D = ax::WBind<ax::GC>(this, &DesktopCube::OnPaint3D);
	win->event.OnLeftArrowDown
		= ax::WBind<char>(this, &DesktopCube::OnLeftArrowDown);
	win->event.OnRightArrowDown
		= ax::WBind<char>(this, &DesktopCube::OnRightArrowDown);
}

void DesktopCube::SetBackgroundImages(const std::vector<ax::Image::Ptr> & images)
{
	_bg_imgs = images;
}

int DesktopCube::GetFaceSelected() const
{
	return _cube_face_selected;
}

void DesktopCube::OnLeftArrowDown(const char& c)
{
	_cube_angle -= 5.0;

	if (_cube_angle < 0.0) {
		_cube_angle += 360.0;
	}

	if (_cube_angle > 360.0 * 0.25 * 0.5) {
		_cube_face_selected = 1;
	}
	else {
		_cube_face_selected = 0;
	}

	win->Update();
}

void DesktopCube::OnRightArrowDown(const char& c)
{
	_cube_angle += 5.0;

	if (_cube_angle > 360.0) {
		_cube_angle -= 360.0;
	}

	if (_cube_angle > 360.0 * 0.25 * 0.5) {
		_cube_face_selected = 1;
	}
	else {
		_cube_face_selected = 0;
	}

	win->Update();
}

void DesktopCube::OnPaint3D(ax::GC gc)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
	glLoadIdentity(); // Reset
	ax::Rect rect(win->dimension.GetDrawingRect());
	double asp_ratio = rect.size.x / (double)rect.size.y;

	gluPerspective(45.0f, asp_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity(); // Reset the model-view matrix
	glTranslatef(0.0f, 0.0f, -4.0f); // Move right and into the screen
	glRotated(_cube_angle, 0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	if (_cube_face_selected == 0) {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
		glLineWidth(4.0);

		glBegin(GL_LINES);
		{
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);

			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
		}
		glEnd();

		glLineWidth(1.0);
	}

	if (_cube_face_selected == 1) {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
		glLineWidth(4.0);

		glBegin(GL_LINES);
		{
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);

			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);

			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
		}
		glEnd();

		glLineWidth(1.0);
	}

	// Front face  (z = 1.0f)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _bg_imgs[0]->GetTexture());
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _bg_imgs[1]->GetTexture());
	glBegin(GL_QUADS);
	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue

	glTexCoord2d(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2d(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);
}
}
