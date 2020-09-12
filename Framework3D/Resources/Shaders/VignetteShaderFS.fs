precision mediump float;
uniform sampler2D u_mainTexture;
uniform float u_intensify;
uniform float u_extend;
uniform vec4 u_color;
varying vec2 v_uv;

void main()
{
	vec2 uv = v_uv;
	uv *=  vec2(1.0)- uv.yx; 
    float vignette = uv.x*uv.y * u_intensify; // multiply with sth for intensity
    vignette = pow(vignette, u_extend); // change pow for modifying the extend of the  vignette
	gl_FragColor = vec4(u_color.xyz,u_color.w*(1.0-vignette));
}
