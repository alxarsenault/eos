attribute vec4 vPosition;
varying vec4 vColor;

void main()
{	
	vColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
}