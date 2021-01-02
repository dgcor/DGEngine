#define FIX(c) max(abs(c), 1e-5);

uniform sampler2D texture;
uniform vec2 textureSize;
uniform vec4 fade;
uniform float gamma;

vec3 Gamma(vec3 value, float param)
{
	return vec3(pow(abs(value.r), param), pow(abs(value.g), param), pow(abs(value.b), param));
}

const float PI = 3.1415926535897932384626433832795;

vec3 weight3(float x)
{
	const float radius = 3.0;
	vec3 sample = FIX(2.0 * PI * vec3(x - 1.5, x - 0.5, x + 0.5));

	// Lanczos3. Note: we normalize outside this function, so no point in multiplying by radius.
	return /*radius **/ sin(sample) * sin(sample / radius) / (sample * sample);
}

vec3 pixel(float xpos, float ypos)
{
	return texture2D(texture, vec2(xpos, ypos)).rgb;
}

vec3 line(float ypos, vec3 xpos1, vec3 xpos2, vec3 linetaps1, vec3 linetaps2)
{
	return
		pixel(xpos1.r, ypos) * linetaps1.r +
		pixel(xpos1.g, ypos) * linetaps2.r +
		pixel(xpos1.b, ypos) * linetaps1.g +
		pixel(xpos2.r, ypos) * linetaps2.g +
		pixel(xpos2.g, ypos) * linetaps1.b +
		pixel(xpos2.b, ypos) * linetaps2.b;
}

void main()
{
	vec2 stepxy = 1.0 / textureSize.xy;
	vec2 pos = gl_TexCoord[0].xy + stepxy * 0.5;
	vec2 f = fract(pos / stepxy);

	vec3 linetaps1   = weight3(0.5 - f.x * 0.5);
	vec3 linetaps2   = weight3(1.0 - f.x * 0.5);
	vec3 columntaps1 = weight3(0.5 - f.y * 0.5);
	vec3 columntaps2 = weight3(1.0 - f.y * 0.5);

	// make sure all taps added together is exactly 1.0, otherwise some
	// (very small) distortion can occur
	float suml = dot(linetaps1, vec3(1.0)) + dot(linetaps2, vec3(1.0));
	float sumc = dot(columntaps1, vec3(1.0)) + dot(columntaps2, vec3(1.0));
	linetaps1 /= suml;
	linetaps2 /= suml;
	columntaps1 /= sumc;
	columntaps2 /= sumc;

	vec2 xystart = (-2.5 - f) * stepxy + pos;
	vec3 xpos1 = vec3(xystart.x, xystart.x + stepxy.x, xystart.x + stepxy.x * 2.0);
	vec3 xpos2 = vec3(xystart.x + stepxy.x * 3.0, xystart.x + stepxy.x * 4.0, xystart.x + stepxy.x * 5.0);

	gl_FragColor = vec4(
		line(xystart.y                 , xpos1, xpos2, linetaps1, linetaps2) * columntaps1.r +
		line(xystart.y + stepxy.y      , xpos1, xpos2, linetaps1, linetaps2) * columntaps2.r +
		line(xystart.y + stepxy.y * 2.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps1.g +
		line(xystart.y + stepxy.y * 3.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps2.g +
		line(xystart.y + stepxy.y * 4.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps1.b +
		line(xystart.y + stepxy.y * 5.0, xpos1, xpos2, linetaps1, linetaps2) * columntaps2.b,
		1.0);

	float gamma2 = 1.0 - (0.007 * gamma);
	gl_FragColor.xyz = Gamma(gl_FragColor.xyz - fade.xyz, gamma2);
	gl_FragColor.a = gl_FragColor.a - fade.a;
}