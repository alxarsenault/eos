attribute vec4 vPosition;
varying vec4 vColor;
varying vec2 blurCoordinates[5];
uniform sampler2D texture1;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	float singleStepOffset = 1.0 / 300.0;
	
	blurCoordinates[0] = gl_TexCoord[0].st;
	blurCoordinates[1] = gl_TexCoord[0].st + singleStepOffset * 1.407333;
	blurCoordinates[2] = gl_TexCoord[0].st - singleStepOffset * 1.407333;
	blurCoordinates[3] = gl_TexCoord[0].st + singleStepOffset * 3.294215;
	blurCoordinates[4] = gl_TexCoord[0].st - singleStepOffset * 3.294215;
}