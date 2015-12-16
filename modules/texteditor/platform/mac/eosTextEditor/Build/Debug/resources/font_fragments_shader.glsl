varying vec4 vColor;
uniform sampler2D texture1;


void main() 
{
//	gl_FragColor = vColor * texture2D(texture1, gl_TexCoord[0].st);
//	lowp vec4 destColor = gl_LastFragData[0]; // Only GLES 2.0 Apple.
	vec4 txtColor = texture2D(texture1, gl_TexCoord[0].st);
	gl_FragColor = vColor * txtColor.a;
}