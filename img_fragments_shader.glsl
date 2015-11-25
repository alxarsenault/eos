
uniform sampler2D texture1;
uniform float img_alpha;

varying vec4 vColor;
varying vec2 blurCoordinates[5];


void main() 
{
//	gl_FragColor = texture2D(texture1, gl_TexCoord[0].st) * img_alpha;
	
	vec4 sum = vec4(0.0);
	sum += texture2D(texture1, blurCoordinates[0]) * 0.204164;
	sum += texture2D(texture1, blurCoordinates[1]) * 0.304005;
	sum += texture2D(texture1, blurCoordinates[2]) * 0.304005;
	sum += texture2D(texture1, blurCoordinates[3]) * 0.093913;
	sum += texture2D(texture1, blurCoordinates[4]) * 0.093913;
	gl_FragColor = sum;
}