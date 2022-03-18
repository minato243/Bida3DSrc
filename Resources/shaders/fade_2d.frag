#ifdef GL_ES
precision float mediump;
#endif

varying vec2 v_uv;

uniform float u_opacity;
uniform float u_minOpacity;

//these ranges should not overlap, would lead to undefined behaviour
uniform float u_thresholdMaxLeft;
uniform float u_thresholdMaxRight;
uniform float u_thresholdMaxTop;
uniform float u_thresholdMaxBottom;

uniform float u_thresholdMinLeft;
uniform float u_thresholdMinRight;
uniform float u_thresholdMinTop;
uniform float u_thresholdMinBottom;

float getPart(float val, float low, float high){
    return (val-low)/(high-low);
}

void main()
{
    vec4 color = texture2D(CC_Texture0, v_uv);

    float opacityLevel = 1.0;
    float x = gl_FragCoord.x, y = gl_FragCoord.y;
    // if(y > u_thresholdMaxBottom){
    //     gl_FragColor = vec4(1,0,0,1);
    //     return;
    // }
    // if(y > u_thresholdMinBottom){
    //     gl_FragColor = vec4(0,1,0,1);
    //     return;
    // }
    // if(y < u_thresholdMinBottom){
    //     gl_FragColor = vec4(0,0,1,1);
    //     return;
    // }
    if(x < u_thresholdMinLeft || x > u_thresholdMinRight || y > u_thresholdMinTop || y < u_thresholdMinBottom){
        // opacityLevel = 0.0;
        discard;
    }

    if(x > u_thresholdMinLeft && x < u_thresholdMaxLeft){
        opacityLevel *= getPart(x, u_thresholdMinLeft, u_thresholdMaxLeft);
    }
    if(y > u_thresholdMinBottom && y < u_thresholdMaxBottom){
        opacityLevel *= getPart(y, u_thresholdMinBottom, u_thresholdMaxBottom);
    }
    if(x < u_thresholdMinRight && x > u_thresholdMaxRight){
        opacityLevel *= getPart(x, u_thresholdMinRight, u_thresholdMaxRight);
    }
    if(y < u_thresholdMinTop && y > u_thresholdMaxTop){
        opacityLevel *= getPart(y, u_thresholdMinTop, u_thresholdMaxTop);
    }

    float opacity = opacityLevel * (u_opacity - u_minOpacity) + u_minOpacity;

    gl_FragColor = vec4(opacity * color.rgb, color.a * opacity);
}