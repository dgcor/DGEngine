#version 110
uniform sampler2D palette;
uniform sampler2D texture;
uniform vec4 outline;
uniform vec4 ignore;
uniform bool hasPalette;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (hasPalette == true && pixel.a == 1.0)
	{
		pixel = texture2D(palette, vec2(pixel.r, 0.0));
	}

	if (outline.a > 0.0 && pixel.a != 0.0 && pixel != ignore)
	{
		pixel.r = min(pixel.r + 0.1, 1.0);
		pixel.g = min(pixel.g + 0.1, 1.0);
		pixel.b = min(pixel.b + 0.1, 1.0);
	}
	gl_FragColor = pixel;
}