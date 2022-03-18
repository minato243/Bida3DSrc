#ifdef GL_ES
varying lowp vec2 TextureCoordOut;
varying lowp vec3 v_normal;
varying lowp vec3 v_FragPos;

varying lowp vec3 TangentLightPos;
varying lowp vec3 TangentViewPos;
varying lowp vec3 TangentFragPos;

varying lowp vec4 v_FragPosLightSpace;
#else
varying vec2 TextureCoordOut;
varying vec3 v_normal;
varying vec3 v_FragPos;

varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;

varying vec4 v_FragPosLightSpace;
#endif


uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;

uniform vec3 light_position;
uniform vec3 light_intensities;
uniform float light_attenuation;
uniform float light_ambient_coefficient;

uniform vec4 u_color;
uniform vec3 u_viewPosition;
uniform int u_hasShadow;

uniform sampler2D u_normalTex;
uniform sampler2D u_specularTex;
uniform sampler2D u_shadowTex;

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
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
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
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


void main(void)
{
	// init value
	vec3 lightDir = normalize(light_position - v_FragPos);
	vec3 viewDir = normalize(u_viewPosition - v_FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 norm = normalize(v_normal);


	vec3 objColor = texture2D(CC_Texture0, TextureCoordOut).rgb;
	vec3 normalTagent = texture2D(u_normalTex, TextureCoordOut).rgb;
	normalTagent = normalize(normalTagent * 2.0 - 1.0);

	vec3 specularColor = texture2D(u_specularTex, TextureCoordOut).rgb;

	vec3 ligthDirTangent = TangentLightPos - TangentFragPos;

	//ambient
	vec3 ambient = light_ambient_coefficient * light_intensities  * objColor + material_ambient;
	

	// diffuse
	float diffCoefficient = max(dot(normalize(ligthDirTangent),normalTagent),0.0);
	vec3 diffuse = diffCoefficient * light_intensities*  objColor * material_diffuse;


	//specular 
	vec3 reflectDir = reflect(normalize(ligthDirTangent),normalTagent);

	float specCoefficient = pow(max(dot(-normalize(TangentViewPos - TangentFragPos),reflectDir),0.0),material_shininess);
	vec3 specular = specCoefficient * light_intensities  * specularColor * material_specular;

	//shadow cal
	float shadow = (u_hasShadow>0)? ShadowCalculation(v_FragPosLightSpace):0.0;

	// linear color
	vec3 linearColor = ambient + (diffuse + specular ) * (1.0 - shadow);

	 gl_FragColor = vec4(linearColor,1) *u_color ;

}