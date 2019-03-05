#version 110
uniform sampler2D texture;
uniform float elapsedTime;

float noise(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	float n = noise(gl_TexCoord[0].xy * elapsedTime);
	if (n > 0.5)
	{
		n = 1.0;
	}
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	pixel.r = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
	pixel.g = pixel.r;
	pixel.b = pixel.r;
	gl_FragColor = vec4(pixel.rgb * vec3(n, n, n), pixel.a);
}