precision mediump float;
uniform vec4 u_color;
uniform sampler2D u_mainTexture;
varying vec2 v_uv;

void main()
{
	vec4 color = u_color;
	color.w = texture2D(u_mainTexture,v_uv).w;
	gl_FragColor = color;
}
