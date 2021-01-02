void main()
{
	float x = 0.001;
	float y = 0.001;

	vec2 dg1 = vec2( x,y);  vec2 dg2 = vec2(-x,y);
	vec2 sd1 = dg1*0.5;     vec2 sd2 = dg2*0.5;
	vec2 ddx = vec2(x,0.0); vec2 ddy = vec2(0.0,y);

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1].xy = gl_TexCoord[0].xy - sd1;
	gl_TexCoord[2].xy = gl_TexCoord[0].xy - sd2;
	gl_TexCoord[3].xy = gl_TexCoord[0].xy + sd1;
	gl_TexCoord[4].xy = gl_TexCoord[0].xy + sd2;
	gl_TexCoord[5].xy = gl_TexCoord[0].xy - dg1;
	gl_TexCoord[6].xy = gl_TexCoord[0].xy + dg1;
	gl_TexCoord[5].zw = gl_TexCoord[0].xy - dg2;
	gl_TexCoord[6].zw = gl_TexCoord[0].xy + dg2;
	gl_TexCoord[1].zw = gl_TexCoord[0].xy - ddy;
	gl_TexCoord[2].zw = gl_TexCoord[0].xy + ddx;
	gl_TexCoord[3].zw = gl_TexCoord[0].xy + ddy;
	gl_TexCoord[4].zw = gl_TexCoord[0].xy - ddx;
}