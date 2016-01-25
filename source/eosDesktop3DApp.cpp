#include "eosDesktop3DApp.h"

namespace eos
{
	Desktop3DApp::Desktop3DApp(const ax::Rect& rect)
	{
		win = ax::Window::Create(rect);
		
		win->event.OnPaint = ax::WBind<ax::GC>(this, &Desktop3DApp::OnPaint);
		win->event.OnPaint3D = ax::WBind<ax::GC>(this, &Desktop3DApp::OnPaint3D);
	}
	
	void Desktop3DApp::DrawCube()
	{
		Vec3 v[8] = {
			Vec3(-1.0, -1.0, 1.0), Vec3(1.0, -1.0, 1.0), Vec3(1.0, 1.0, 1.0),
			Vec3(-1.0, 1.0, 1.0), Vec3(-1.0, -1.0, -1.0), Vec3(1.0, -1.0, -1.0),
			Vec3(1.0, 1.0, -1.0), Vec3(-1.0, 1.0, -1.0),
		};
		
		Vec3 cube_vertices[] = {
			v[0], v[1], v[2], v[2], v[3], v[0], // Front.
			v[1], v[5], v[6], v[6], v[2], v[1], // top
			v[7], v[6], v[5], v[5], v[4], v[7], // back
			v[4], v[0], v[3], v[3], v[7], v[4], // bottom
			v[4], v[5], v[1], v[1], v[0], v[4], // left
			v[3], v[2], v[6], v[6], v[7], v[3] // right
		};
		
		float alpha = 1.0;
		
		ax::Color colors[8] = {
			ax::Color(0.9607, 0.6824, 0.1098, alpha),
			ax::Color(0.9607, 0.6824, 0.1098, alpha),
			//			ax::Color(0.8235, 0.2275, 0.4745, alpha),
			ax::Color(0.4), ax::Color(1.0, 1.0, 1.0, alpha),
			ax::Color(0.0, 1.0, 1.0, alpha), ax::Color(0.0, 1.0, 1.0, alpha),
			ax::Color(1.0, 0.0, 1.0, alpha), ax::Color(1.0, 1.0, 1.0, alpha),
		};
		
		ax::Color cube_colors[] = { colors[0], colors[0], colors[0], colors[0],
			colors[0], colors[0], colors[1], colors[1], colors[1], colors[1],
			colors[1], colors[1], colors[2], colors[2], colors[2], colors[2],
			colors[2], colors[2], colors[3], colors[3], colors[3], colors[3],
			colors[3], colors[3], colors[4], colors[4], colors[4], colors[4],
			colors[4], colors[4], colors[5], colors[5], colors[5], colors[5],
			colors[5], colors[5], colors[6], colors[6], colors[6], colors[6],
			colors[6], colors[6], colors[7], colors[7], colors[7], colors[7],
			colors[7], colors[7] };
		
		glEnableClientState(GL_VERTEX_ARRAY);
		//	glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
		glColorPointer(4, GL_FLOAT, 0, cube_colors);
		//	glNormalPointer(GL_FLOAT, 0, normals.data());
		
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		//	glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	void Desktop3DApp::OnPaint(ax::GC gc)
	{
		ax::Rect rect(win->dimension.GetDrawingRect());
		gc.SetColor(ax::Color(0.0, 0.0));
		gc.DrawRectangle(rect);
	}
	
	void Desktop3DApp::OnPaint3D(ax::GC gc)
	{
		glUseProgram(0);
		
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_shininess[] = { 50.0 };
		
		GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
		
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glShadeModel(GL_SMOOTH);
		
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glEnable(GL_MULTISAMPLE_ARB);
		
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		
		glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
		glLoadIdentity(); // Reset
		ax::Rect rect(win->dimension.GetDrawingRect());
		double asp_ratio = rect.size.x / (double)rect.size.y;
		
		gluPerspective(45.0f, asp_ratio, 0.1f, 100.0f);
		//			gluPerspective(50.0, 1.0, 3.0, 7.0);
		
		glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix
		
		// Render a color-cube consisting of 6 quads with different colors
		glLoadIdentity(); // Reset the model-view matrix
		
		gluLookAt(-12.0, -12.0, -12.0, 12.0, 12.0, 12.0, 0.0, 0.0, 1.0);
		
		float angle = 270;
		
		glTranslatef(-3.5f, -0.5f, -0.8f);
		glTranslatef(0.0f, 0.0f, -0.8f);
		glRotated(angle, 0, 0.0, 1.0);
		DrawCube();
		
		glTranslatef(0.0f, 2.0f, -2.0f);
		DrawCube();
		
		glTranslatef(2.0f, 2.0f, 0.0f);
		DrawCube();
		
		glTranslatef(0.0f, -2.0f, 2.0f);
		DrawCube();
		
		glTranslatef(2.0f, 2.0f, 0.0f);
		DrawCube();
		
		// Top.
		glTranslatef(-2.0f, -2.0f, 0.0f);
		glTranslatef(-2.0f, 2.0f, -4.0f);
		DrawCube();
		
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		
		gc.shader_fb.Activate();
	}
}
