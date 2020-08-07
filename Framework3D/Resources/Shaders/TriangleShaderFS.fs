precision mediump float;

uniform sampler2D u_mainTexture;
uniform vec4 u_fogColor;
varying vec2 v_uv;

varying float v_fogDensity;
void main()
{
	vec4 color =  texture2D(u_mainTexture,v_uv);
	gl_FragColor = u_fogColor * v_fogDensity + color * (1.0 - v_fogDensity);
}
