uniform sampler2D texture1;
varying vec2 v_textureCoordinate;

void main()
{
	gl_FragColor = texture2D(texture1, v_textureCoordinate);
}
