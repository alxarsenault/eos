attribute vec4 vPosition;
attribute vec2 vTexCoord;
attribute vec4 vColor;

uniform mat4 mvp_matrix;
varying vec2 v_textureCoordinate;

void main()
{
	v_textureCoordinate = vTexCoord;
	gl_Position = mvp_matrix * vPosition;
}
