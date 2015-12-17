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
uniform mat4 mvp_matrix;
varying vec4 vColor;
varying vec2 v_textureCoordinate;

void main()
{
	//	gl_TexCoord[0] = gl_MultiTexCoord0;
	//	gl_TexCoord[0] = gl_MultiTexCoord0;
	v_textureCoordinate = vTexCoord;
//	v_textureCoordinate[0] = vTexCoord[0];
//	v_textureCoordinate[1] = vTexCoord[1];
	vColor = gl_Color;
//	gl_Position = gl_ModelViewProjectionMatrix * vPosition;
	gl_Position = mvp_matrix * vPosition;
}