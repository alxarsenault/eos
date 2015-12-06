attribute vec4 vPosition;
attribute vec2 vTexCoord;
attribute vec4 vColor;
varying vec2 v_textureCoordinate;
uniform mat4 mvp_matrix;

void main()
{
	v_textureCoordinate = vTexCoord;
	gl_Position = mvp_matrix * vPosition;
}
