//attribute vec4 vPosition;
//varying vec4 vColor;

attribute vec4 vPosition;
attribute vec2 vTexCoord;
uniform mat4 mvp_matrix;
attribute vec4 vColor;
varying vec4 fragColor;
varying vec2 v_textureCoordinate;

void main()
{
	v_textureCoordinate = vTexCoord;
//	vColor = gl_Color;
	fragColor = vColor;
	gl_Position = mvp_matrix * vPosition;
//	gl_TexCoord[0] = gl_MultiTexCoord0;
//	vColor = gl_Color;
//	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
}