#version 110
uniform sampler2D texture;

void main()
{
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}