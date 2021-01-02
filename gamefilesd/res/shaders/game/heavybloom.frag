uniform sampler2D texture;
#define glarebasesize 0.42
#define power 0.65 // 0.50 is good

uniform vec2 textureSize;

void main()
{
	vec4 sum = vec4(0.0);
	vec4 bum = vec4(0.0);
	vec2 texcoord = vec2(gl_TexCoord[0]);
	int j;
	int i;

	vec2 glaresize = vec2(glarebasesize) / textureSize;

	for(i = -2; i < 5; i++)
	{
		for (j = -1; j < 1; j++)
		{
			sum += texture2D(texture, texcoord + vec2(-i, j)*glaresize) * power;
			bum += texture2D(texture, texcoord + vec2(j, i)*glaresize) * power;
		}
	}

	if (texture2D(texture, texcoord).r < 2.0)
	{
		gl_FragColor = sum*sum*sum*0.001+bum*bum*bum*0.0080 + texture2D(texture, texcoord);
	}
}