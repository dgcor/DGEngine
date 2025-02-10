#version 110
uniform sampler2D palette;
uniform sampler2D texture;
uniform vec2 pixelSize;
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

	if (outline.a > 0.0 && pixel.a == 0.0)
	{
		vec4 vu = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - pixelSize.y));
		vec4 vd = texture2D(texture, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + pixelSize.y));
		vec4 vl = texture2D(texture, vec2(gl_TexCoord[0].x - pixelSize.x, gl_TexCoord[0].y));
		vec4 vr = texture2D(texture, vec2(gl_TexCoord[0].x + pixelSize.x, gl_TexCoord[0].y));

		if (vu.a == 1.0 || vd.a == 1.0 || vl.a == 1.0 || vr.a == 1.0)
		{
			if (ignore.a == 0.0)
			{
				pixel = outline;
			}
			else
			{
				if (vu != ignore &&
					vd != ignore &&
					vl != ignore &&
					vr != ignore)
				{
					pixel = outline;
				}
			}
		}
	}
	gl_FragColor = pixel;
}