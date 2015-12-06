uniform sampler2D texture1;

varying vec4 fragColor;
varying vec2 v_textureCoordinate;

void main() 
{
	vec4 txtColor = texture2D(texture1, v_textureCoordinate);
	gl_FragColor = fragColor * txtColor.a;
}
