#ifdef GL_ES
 precision lowp float;
 #endif

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
    float gray = dot(v_orColor.rgb, vec3(0.3, 0.587, 0.114));
    gl_FragColor = vec4(gray, gray, gray, v_orColor.a);
}
