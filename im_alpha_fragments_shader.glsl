//varying vec4 vColor;
//uniform sampler2D texture1;
//
//
//void main() 
//{
//	gl_FragColor = texture2D(texture1, gl_TexCoord[0].st);
//}

//varying vec4 vColor;
varying vec4 fragColor;
uniform sampler2D texture1;
varying vec2 v_textureCoordinate;

void main()
{
	//	gl_FragColor = texture2D(texture1, gl_TexCoord[0].st);
	gl_FragColor = fragColor.a * texture2D(texture1, v_textureCoordinate);
}