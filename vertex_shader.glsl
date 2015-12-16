attribute vec4 vPosition;
varying vec4 vColor;
uniform sampler2D texture1;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	
}