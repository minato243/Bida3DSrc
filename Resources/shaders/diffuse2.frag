#ifdef GL_ES
varying mediump vec2 TextureCoordOut;
varying mediump vec3 v_diffuse;
varying lowp vec3 v_spec;
varying lowp vec3 v_FragPos;
varying lowp vec3 v_normal;
varying lowp vec4 v_FragPosLightSpace;
varying lowp vec3 v_projCoords;

#else
varying vec2 TextureCoordOut;
varying vec3 v_diffuse;
varying vec3 v_spec;
varying vec3 v_FragPos;
varying vec3 v_normal;
varying vec4 v_FragPosLightSpace;
varying vec3 v_projCoords;
#endif

uniform vec4 u_color;
uniform vec3 u_viewPosition;
uniform int u_hasShadow;

uniform vec3 light_position;
uniform vec3 light_intensities;
uniform float light_attenuation;
uniform float light_ambient_coefficient;
uniform float light_specular;

uniform sampler2D u_shadowTex;

float near = 0.1; 
float far  = 100.0;

float ShadowOptimize(vec3 projCoords )
{
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture2D(u_shadowTex, projCoords.xy).r;
        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;

         float bias = 0.005;
         // check whether current frag pos is in shadow
         float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
         shadow = shadow > 0.5?0.5:shadow;

         return shadow;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture2D(u_shadowTex, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(v_normal);
    vec3 lightDir = normalize(light_position - v_FragPos);
    //return dot(normal, lightDir);
//    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.005;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    shadow = shadow > 0.5?0.5:shadow;
//    return shadow;
    // PCF
    shadow = 0.0;
	float s = 1.0 / 1024.0;
    vec2 texelSize = vec2(s , s) ;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture2D(u_shadowTex, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    shadow = shadow > 0.5?0.5:shadow;
    shadow = projCoords.z > 1.0?0.0:shadow;
    return shadow;
}


void main(void)
{
	vec3 objColor  = texture2D(CC_Texture0, TextureCoordOut).rgb;

	vec3 diffuse = v_diffuse * objColor;
    vec3 specular = v_spec;

	//shadow cal
    float shadow = (u_hasShadow>0)? ShadowOptimize(v_projCoords):0.0;

	// linear color
	vec3 linearColor = (diffuse + specular) ;
	gl_FragColor = vec4(linearColor * (1.0 -  shadow),1.0) * u_color;

}