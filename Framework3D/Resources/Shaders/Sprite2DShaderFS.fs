precision mediump float;
uniform vec4 u_color;
uniform sampler2D u_mainTexture;
varying vec2 v_uv;

void main()
{
	gl_FragColor = texture2D(u_mainTexture,v_uv)*u_color;
}
