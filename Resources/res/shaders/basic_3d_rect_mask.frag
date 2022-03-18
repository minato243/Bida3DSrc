varying vec2 v_texture_coord;

uniform vec4 u_color;
uniform vec2 u_min;
uniform vec2 u_max;

void main(void)
{
	// gl_FragColor = vec4(v_normal, 1.0);
	if(gl_FragCoord.x < u_min.x || gl_FragCoord.x > u_max.x || gl_FragCoord.y < u_min.y || gl_FragCoord.y > u_max.y) 
		discard;
	gl_FragColor = texture2D(CC_Texture0, v_texture_coord) * u_color;
}
