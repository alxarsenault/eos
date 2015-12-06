uniform mat4 mvp_matrix;

attribute vec4 vPosition;
attribute vec2 vTexCoord;
attribute vec4 vColor;

varying vec4 fragColor;
varying vec2 v_textureCoordinate;

void main()
{
	v_textureCoordinate = vTexCoord;
	fragColor = vColor;
	gl_Position = mvp_matrix * vPosition;
}
