varying vec4 fragColor;
varying vec2 v_textureCoordinate;

uniform sampler2D texture1;

void main()
{
	gl_FragColor = fragColor.a * texture2D(texture1, v_textureCoordinate);
}
