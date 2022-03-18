#ifdef GL_ES
varying lowp vec2 TextureCoordOut;
varying lowp vec3 v_normal;
varying lowp vec3 v_FragPos;

#else
varying vec2 TextureCoordOut;
varying vec3 v_normal;
varying vec3 v_FragPos;
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

uniform sampler2D u_specularTex;
uniform sampler2D u_shadowTex;

void main(void)
{
	vec3 objColor = texture2D(u_shadowTex, TextureCoordOut).rgb;
	gl_FragColor = vec4(objColor,TextureCoordOut.y);

}