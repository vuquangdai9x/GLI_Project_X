precision mediump float;
uniform vec4 u_color;
uniform sampler2D u_mainTexture;
varying vec4 v_colorOffset;

void main()
{
	gl_FragColor = texture2D(u_mainTexture,vec2(gl_PointCoord.x,1.0-gl_PointCoord.y)) * (u_color+v_colorOffset);
}
