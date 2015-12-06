uniform sampler2D texture1;
uniform mat4 mvp_matrix;

attribute vec4 vPosition;
attribute vec4 vColor;

varying vec4 fragColor;

void main()
{
	fragColor = vColor;
	gl_Position = mvp_matrix * vPosition;
}
