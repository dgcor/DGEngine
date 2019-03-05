#version 110
uniform sampler2D texture;
uniform vec4 fade;
uniform float gamma;

vec3 Gamma(vec3 value, float param)
{
	return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

void main()
{
	float factor = 100.0 + (800.0 - (fade.a * 800.0));
	vec2 pos = floor(gl_TexCoord[0].xy * factor + 0.5) / factor;
	vec4 pixel = texture2D(texture, pos) * gl_Color;
	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor = vec4(Gamma(pixel.rgb, gamma2), pixel.a);
}