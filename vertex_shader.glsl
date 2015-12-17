attribute vec4 vPosition;
attribute vec4 vColor;
uniform sampler2D texture1;
uniform mat4 mvp_matrix;

varying vec4 fragColor;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	fragColor = vColor;
//	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	gl_Position = mvp_matrix * vPosition;
	
}