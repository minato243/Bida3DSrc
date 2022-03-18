varying vec2 v_texture_coord;

uniform vec4 u_color;
uniform vec2 fadeBottomLeft;
uniform vec2 opaqueBottomLeft;
uniform vec2 fadeTopRight;
uniform vec2 opaqueTopRight;
uniform float u_disabled;

float getPart(float min, float max, float x){
	return (x - min)/(max - min);
}

int mod(int a, int b){
	return a - ((a/b) * b);
}

vec4 grayscale(vec4 c){
	float a = c.r * 0.29 + c.g * 0.59 + c.b * 0.11;
	return vec4(a, a, a, c.a);
}

void main(void)
{
	// gl_FragColor = vec4(v_normal, 1.0);
	float x = gl_FragCoord.x;
	float y = gl_FragCoord.y;
	if(x < fadeBottomLeft.x || x > fadeTopRight.x || y < fadeBottomLeft.y || y > fadeTopRight.y) 
		discard;

	float opacity = 1.0;

	if(x > fadeBottomLeft.x && x < opaqueBottomLeft.x){
		opacity *= getPart(fadeBottomLeft.x, opaqueBottomLeft.x, x);
	}

	if(x < fadeTopRight.x && x > opaqueTopRight.x){
		opacity *= getPart(fadeTopRight.x, opaqueTopRight.x, x);
	}

	if(y > fadeBottomLeft.y && y < opaqueBottomLeft.y){
		opacity *= getPart(fadeBottomLeft.y, opaqueBottomLeft.y, y);
	}

	if(y < fadeTopRight.y && y > opaqueTopRight.y){
		opacity *= getPart(fadeTopRight.y, opaqueTopRight.y, y);
	}
		
	int posx = int(x);
	int posy = int(y);

	float thresholdMatrix[16];
	thresholdMatrix[0] = 1.0/17.0;
	thresholdMatrix[1] = 9.0/17.0;
	thresholdMatrix[2] = 3.0/17.0;
	thresholdMatrix[3] = 11.0/17.0;
	thresholdMatrix[4] = 13.0/17.0;
	thresholdMatrix[5] = 5.0/17.0;
	thresholdMatrix[6] = 15.0/17.0;
	thresholdMatrix[7] = 7.0/17.0;
	thresholdMatrix[8] = 4.0/17.0;
	thresholdMatrix[9] = 12.0/17.0;
	thresholdMatrix[10] = 2.0/17.0;
	thresholdMatrix[11] = 10.0/17.0;
	thresholdMatrix[12] = 16.0/17.0;
	thresholdMatrix[13] = 8.0/17.0;
	thresholdMatrix[14] = 14.0/17.0;
	thresholdMatrix[15] = 6.0/17.0;

	if(opacity < thresholdMatrix[mod(posx, 4) * 4 + mod(posy, 4)]){
		discard;
	}

    if(u_disabled > 1.5){
        gl_FragColor = grayscale(texture2D(CC_Texture0, v_texture_coord));
    }
	else if(u_disabled > 0.5){
		gl_FragColor = vec4(vec3(0.4), 1.0) * grayscale(texture2D(CC_Texture0, v_texture_coord));
	}
	else{
		gl_FragColor = texture2D(CC_Texture0, v_texture_coord) * u_color;
	}
}
