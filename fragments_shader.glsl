//varying vec4 vColor;
varying vec4 fragColor;
uniform sampler2D texture1;

void main() 
{
	gl_FragColor = fragColor;
//	gl_FragColor = texture2D(texture1, gl_TexCoord[0].st);
}