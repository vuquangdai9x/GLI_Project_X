precision mediump float;

uniform sampler2D u_mainTexture;
uniform sampler2D u_displTex;
uniform sampler2D u_alphaMaskTex;
uniform vec4 u_fogColor;
uniform float u_time;
uniform float u_displMax;

varying vec2 v_uv;
	
varying float v_fogDensity;

void main()
{
	vec2 offset = (2.0 * texture2D(u_displTex, vec2(v_uv.x, v_uv.y + u_time)).rg - 1.0) * u_displMax;
	float alpha = texture2D(u_alphaMaskTex, v_uv).r;
	vec4 color = texture2D(u_mainTexture, v_uv + offset);
	color = u_fogColor * v_fogDensity + color * (1.0 - v_fogDensity);
	gl_FragColor = color * vec4(1.0,1.0,1.0,alpha);
}
