precision mediump float;
uniform vec4 u_color;
uniform float u_mixColor;
uniform sampler2D u_mainTexture;
varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_mainTexture,v_uv);
	gl_FragColor = color;
}
