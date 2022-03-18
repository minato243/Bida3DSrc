uniform vec4 u_color;
uniform vec4 u_line_color;
uniform float u_opacity;

void main(void)
{
	// int posx = int(gl_FragCoord.x);
	// int posy = int(gl_FragCoord.y);

	// float thresholdMatrix[16];
	// thresholdMatrix[0] = 1.0/17.0;
	// thresholdMatrix[1] = 9.0/17.0;
	// thresholdMatrix[2] = 3.0/17.0;
	// thresholdMatrix[3] = 11.0/17.0;
	// thresholdMatrix[4] = 13.0/17.0;
	// thresholdMatrix[5] = 5.0/17.0;
	// thresholdMatrix[6] = 15.0/17.0;
	// thresholdMatrix[7] = 7.0/17.0;
	// thresholdMatrix[8] = 4.0/17.0;
	// thresholdMatrix[9] = 12.0/17.0;
	// thresholdMatrix[10] = 2.0/17.0;
	// thresholdMatrix[11] = 10.0/17.0;
	// thresholdMatrix[12] = 16.0/17.0;
	// thresholdMatrix[13] = 8.0/17.0;
	// thresholdMatrix[14] = 14.0/17.0;
	// thresholdMatrix[15] = 6.0/17.0;

	// if(u_opacity < thresholdMatrix[mod(posx, 4) * 4 + mod(posy, 4)]){
	// 	discard;
	// }

	gl_FragColor = vec4((u_line_color).rgb, 1.0) * u_color;
	gl_FragColor.a *= u_opacity;
}
