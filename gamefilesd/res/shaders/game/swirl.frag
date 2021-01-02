#version 110
uniform sampler2D texture;
uniform vec2 textureSize;
uniform vec4 fade;
uniform float gamma;

vec3 Gamma(vec3 value, float param)
{
	return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

vec4 swirl()
{
	float radius = textureSize.y / 2.0;
	float angle = fade.a;

	vec2 tc = gl_TexCoord[0].xy * textureSize;
	vec2 center = textureSize / 2.0;
	tc -= textureSize / 2.0;
	float dist = length(tc);
	if (dist < radius)
	{
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 25.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, s)), dot(tc, vec2(-s, c)));
	}
	tc += center;
	vec3 color = texture2D(texture, tc / textureSize).rgb;
	return vec4(color, 1.0);
}

void main()
{
	vec4 pixel = swirl() - fade;
	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor = vec4(Gamma(pixel.rgb, gamma2), pixel.a);
}