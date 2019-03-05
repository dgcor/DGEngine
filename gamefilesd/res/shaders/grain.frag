#version 110
uniform sampler2D texture;
uniform float elapsedTime;
uniform float gamma;
uniform vec4 fade;

float noise(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 Gamma(vec3 value, float param)
{
	return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

void main()
{
	float n = noise(gl_TexCoord[0].xy * elapsedTime);
	if (n > 0.5)
	{
		n = 1.0;
	}
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) - fade;
	pixel.r = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
	pixel.g = pixel.r;
	pixel.b = pixel.r;
	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor = vec4(Gamma(pixel.rgb * vec3(n, n, n), gamma2), pixel.a);
}