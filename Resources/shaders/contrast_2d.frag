#ifdef GL_ES
precision mediump float;
#endif

// uniform vec4 u_color;
uniform float u_enabled;
varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(CC_Texture0, v_uv);
	// color = color * vec3(u_contrast);

	if(u_enabled < 0.5){
	    const float contrastFactor = 0.8;
        color.r = clamp(color.r * contrastFactor, 0.0, 1.0);
        color.g = clamp(color.g * contrastFactor, 0.0, 1.0);
        color.b = clamp(color.b * contrastFactor, 0.0, 1.0);

        const float grayFactor = 0.8;
        float gray = color.r * 0.29 + color.g * 0.59 + color.b * 0.11;
        color.r = gray * grayFactor + color.r * (1.0 - grayFactor);
        color.g = gray * grayFactor + color.g * (1.0 - grayFactor);
        color.b = gray * grayFactor + color.b * (1.0 - grayFactor);
	}

	gl_FragColor = color;
}
