#version 110
uniform sampler2D texture;
uniform vec4 visibleRect;
uniform int numberOfLights;
uniform float lights[512];
uniform float defaultLight;
uniform float lightRadius;
uniform float elapsedTime;

void main()
{
	float light = 1.0 - defaultLight;

	if (numberOfLights > 0 && light > 0.0)
	{
		for(int i = 0; i < numberOfLights; i += 4)
		{
			vec2 coord = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);
			vec2 pixelPos = visibleRect.xy + (visibleRect.zw * coord);
			float dist = distance(pixelPos, vec2(lights[i], lights[i+1]));
			dist = clamp(dist / lightRadius, 0.0, lights[i+3]) / lights[i+3];
			light = clamp(dist, 0.0, light);
			if (light == 0.0)
			{
				break;
			}
		}
	}

	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	pixel.r = pixel.r - light;
	pixel.g = pixel.g - light;
	pixel.b = pixel.b - light;
	gl_FragColor = pixel;
}