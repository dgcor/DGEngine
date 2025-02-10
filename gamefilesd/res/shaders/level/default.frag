#version 110
uniform sampler2D texture;
uniform float defaultLight;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float light = min(pixel.a, defaultLight);
	pixel.r = max(pixel.r - light, 0.0);
	pixel.g = max(pixel.g - light, 0.0);
	pixel.b = max(pixel.b - light, 0.0);
	pixel.a = 1.0;
	gl_FragColor = pixel;
}