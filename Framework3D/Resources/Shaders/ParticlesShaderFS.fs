precision mediump float;
uniform vec4 u_color;
uniform sampler2D u_mainTexture;

void main()
{
	gl_FragColor = texture2D(u_mainTexture,gl_PointCoord) * u_color;
}
