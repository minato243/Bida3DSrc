#ifdef GL_ES
varying mediump vec2 TextureCoordOut;
varying mediump vec3 v_diffuse;
varying lowp vec3 v_spec;
varying lowp vec3 v_projCoords;

#else
varying vec2 TextureCoordOut;
varying vec3 v_diffuse;
varying vec3 v_spec;
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
         shadow = shadow > 0.25?0.25:shadow;

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