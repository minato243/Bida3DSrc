varying vec4 v_position;
varying vec2 v_texture_coord;
varying vec3 v_normal;

void main(){
    vec3 color = texture2D(CC_Texture0, v_texture_coord).rgb;
    float alpha = 1.0;

    const float startW = 500.0;
    const float startH = 350.0;
    const float d = 150.0;
    if(v_position.x > startW + d) discard;
    if(v_position.x < -(startW + d)) discard;
    if(v_position.z > startH + d) discard;
    if(v_position.z < -(startH + d)) discard;

    if(v_position.x > startW){
        alpha *= 1.0 - (v_position.x - startW)/d;
    }
    if(v_position.x < -startW){
        alpha *= 1.0 - (-v_position.x - startW)/d;
    }
    if(v_position.z > startH){
        alpha *= 1.0 - (v_position.z - startH)/d;
    }
    if(v_position.z < -startH){
        alpha *= 1.0 - (-v_position.z - startH)/d;
    }

    gl_FragColor = vec4(alpha * color, alpha);
}