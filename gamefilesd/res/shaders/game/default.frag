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
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) - fade;
	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor = vec4(Gamma(pixel.rgb, gamma2), pixel.a);
}