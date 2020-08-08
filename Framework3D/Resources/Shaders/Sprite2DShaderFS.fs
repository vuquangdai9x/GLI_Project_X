precision mediump float;

uniform sampler2D u_mainTexture;
varying vec2 v_uv;

void main()
{
	vec4 color =  texture2D(u_mainTexture,v_uv);
	gl_FragColor = color;
	//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
