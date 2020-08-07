precision mediump float;

uniform sampler2D u_texture[4];

uniform vec4 u_fogColor;

varying vec2 v_uv;

varying float v_fogDensity;

void main()
{
	vec4 blend_color = texture2D(u_texture[0],v_uv);
	vec4 color = (texture2D(u_texture[1],v_uv)*blend_color.r + texture2D(u_texture[2],v_uv)*blend_color.g + texture2D(u_texture[3],v_uv)*blend_color.b) / (blend_color.r+blend_color.g+blend_color.b) ;
	gl_FragColor = u_fogColor * v_fogDensity + color * (1.0 - v_fogDensity);
}
