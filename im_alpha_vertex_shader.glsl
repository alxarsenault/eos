//attribute vec4 vPosition;
//varying vec4 vColor;
//
//void main()
//{	
//	gl_TexCoord[0] = gl_MultiTexCoord0;
//	vColor = gl_Color;
//	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
//}

attribute vec4 vPosition;
attribute vec2 vTexCoord;
attribute vec4 vColor;
varying vec4 fragColor;
varying vec2 v_textureCoordinate;
uniform mat4 mvp_matrix;

void main()
{
	//	gl_TexCoord[0] = gl_MultiTexCoord0;
	//	gl_TexCoord[0] = gl_MultiTexCoord0;
	v_textureCoordinate = vTexCoord;
	fragColor = vColor;
//	v_textureCoordinate[0] = vTexCoord[0];
//	v_textureCoordinate[1] = vTexCoord[1];
//	vColor = gl_Color;
//	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	gl_Position = mvp_matrix * vPosition;
}