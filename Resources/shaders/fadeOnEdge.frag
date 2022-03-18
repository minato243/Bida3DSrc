#ifdef GL_ES
precision lowp float;
#endif

uniform float opacity;
uniform vec3 iResolution;
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

//uniform mat4 CC_PMatrix;
//uniform mat4 CC_MVMatrix;
//uniform mat4 CC_MVPMatrix;
//uniform mat3 CC_NormalMatrix;
//uniform vec4 CC_Time;
//uniform vec4 CC_SinTime;
//uniform vec4 CC_CosTime;
//uniform vec4 CC_Random01;
//uniform sampler2D CC_Texture0;
//uniform sampler2D CC_Texture1;
//uniform sampler2D CC_Texture2;
//uniform sampler2D CC_Texture3;

void main()
{
    vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);

    float alpha = 1.0;
    if (gl_FragCoord.x < (iResolution.x * 0.075))
        if (gl_FragCoord.x > (iResolution.x * 0.025))
            alpha = (gl_FragCoord.x - iResolution.x * 0.025) / (iResolution.x * 0.05);
        else alpha = 0.0;
    if (gl_FragCoord.x > (iResolution.x * 0.925))
        if (gl_FragCoord.x < (iResolution.x * 0.975))
            alpha = (iResolution.x * 0.975 - gl_FragCoord.x) / (iResolution.x * 0.05);
        else alpha = 0.0;

    if (opacity < alpha) alpha = opacity;

    v_orColor.r = v_orColor.r * alpha;
    v_orColor.g = v_orColor.g * alpha;
    v_orColor.b = v_orColor.b * alpha;
    v_orColor.a = v_orColor.a * alpha;
    gl_FragColor = v_orColor;
}
