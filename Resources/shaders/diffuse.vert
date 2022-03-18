attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_blendWeight;
attribute vec4 a_blendIndex;
attribute vec3 a_tangent;
attribute vec3 a_binormal;
attribute vec2 a_texCoord;

const int SKINNING_JOINT_COUNT = 60;
// Uniforms
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];

uniform mat4 u_MMatrix;
uniform mat4 u_LightMatrix;

uniform int u_hasSkin;
uniform int u_hasShadow;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;

uniform vec3 light_position;
uniform vec3 light_intensities;
uniform float light_attenuation;
uniform float light_ambient_coefficient;
uniform float light_specular;

uniform float spot_light_cullOff;
uniform float spot_light_cutOff;


uniform vec3 u_viewPosition;

// Varyings
varying vec2 TextureCoordOut;
varying vec3 v_diffuse;
varying vec3 v_spec;
varying vec3 v_projCoords;

vec4 getPosition(out vec3 normal)
{
    float blendWeight = a_blendWeight[0];

    int matrixIndex = int (a_blendIndex[0]) * 3;
    vec4 matrixPalette1 = u_matrixPalette[matrixIndex] * blendWeight;
    vec4 matrixPalette2 = u_matrixPalette[matrixIndex + 1] * blendWeight;
    vec4 matrixPalette3 = u_matrixPalette[matrixIndex + 2] * blendWeight;
    
    
    blendWeight = a_blendWeight[1];
    if (blendWeight > 0.0)
    {
        matrixIndex = int(a_blendIndex[1]) * 3;
        matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
        matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
        matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;
        
        blendWeight = a_blendWeight[2];
        if (blendWeight > 0.0)
        {
            matrixIndex = int(a_blendIndex[2]) * 3;
            matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
            matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
            matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;
            
            blendWeight = a_blendWeight[3];
            if (blendWeight > 0.0)
            {
                matrixIndex = int(a_blendIndex[3]) * 3;
                matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
                matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
                matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;
            }
        }
    }

    vec4 _skinnedPosition;
    vec4 position = vec4(a_position, 1.0);
    _skinnedPosition.x = dot(position, matrixPalette1);
    _skinnedPosition.y = dot(position, matrixPalette2);
    _skinnedPosition.z = dot(position, matrixPalette3);
    _skinnedPosition.w = position.w;

    vec4 n = vec4(a_normal,0.0);
    normal.x = dot(n, matrixPalette1);
    normal.y = dot(n, matrixPalette2);
    normal.z = dot(n, matrixPalette3);


    return _skinnedPosition;
}

mat3 transpose3(mat3 m)
{
	return mat3(m[0][0],m[1][0], m[2][0],
			m[0][1],m[1][1], m[2][1],
			m[0][2],m[1][2], m[2][2]);
}

mat3 inverse(mat3 m) {
  float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
  float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
  float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

  float b01 = a22 * a11 - a12 * a21;
  float b11 = -a22 * a10 + a12 * a20;
  float b21 = a21 * a10 - a11 * a20;

  float det = a00 * b01 + a01 * b11 + a02 * b21;

  return mat3(b01, (-a22 * a01 + a02 * a21), (a12 * a01 - a02 * a11),
              b11, (a22 * a00 - a02 * a20), (-a12 * a00 + a02 * a10),
              b21, (-a21 * a00 + a01 * a20), (a11 * a00 - a01 * a10)) / det;
}

mat4 inverse(mat4 m) {
  float
      a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
      a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
      a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
      a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

      b00 = a00 * a11 - a01 * a10,
      b01 = a00 * a12 - a02 * a10,
      b02 = a00 * a13 - a03 * a10,
      b03 = a01 * a12 - a02 * a11,
      b04 = a01 * a13 - a03 * a11,
      b05 = a02 * a13 - a03 * a12,
      b06 = a20 * a31 - a21 * a30,
      b07 = a20 * a32 - a22 * a30,
      b08 = a20 * a33 - a23 * a30,
      b09 = a21 * a32 - a22 * a31,
      b10 = a21 * a33 - a23 * a31,
      b11 = a22 * a33 - a23 * a32,

      det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

  return mat4(
      a11 * b11 - a12 * b10 + a13 * b09,
      a02 * b10 - a01 * b11 - a03 * b09,
      a31 * b05 - a32 * b04 + a33 * b03,
      a22 * b04 - a21 * b05 - a23 * b03,
      a12 * b08 - a10 * b11 - a13 * b07,
      a00 * b11 - a02 * b08 + a03 * b07,
      a32 * b02 - a30 * b05 - a33 * b01,
      a20 * b05 - a22 * b02 + a23 * b01,
      a10 * b10 - a11 * b08 + a13 * b06,
      a01 * b08 - a00 * b10 - a03 * b06,
      a30 * b04 - a31 * b02 + a33 * b00,
      a21 * b02 - a20 * b04 - a23 * b00,
      a11 * b07 - a10 * b09 - a12 * b06,
      a00 * b09 - a01 * b07 + a02 * b06,
      a31 * b01 - a30 * b03 - a32 * b00,
      a20 * b03 - a21 * b01 + a22 * b00) / det;
}

void main()
{
    vec3 new_normal;
	vec4 position = u_hasSkin>0?getPosition(new_normal):vec4(a_position,1);
	vec3 v_FragPos = vec3(CC_MVMatrix * position);
	vec3 v_normal = CC_NormalMatrix * (u_hasSkin>0?new_normal:a_normal);

	//light
	vec3 lightDir = normalize(light_position - v_FragPos);
    vec3 viewDir = normalize(u_viewPosition - v_FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 norm = normalize(v_normal);

    float distance = length(light_position - v_FragPos);
    float attenuation = 1.0 / (1.0 + 0.000001 * distance + 0.000025 * distance * distance);
    
    /* per vertex lighting*/
    // spot
	float theta = dot(lightDir,normalize(vec3(0,1,0)));
	float epsilon = spot_light_cullOff - spot_light_cutOff;
	float intensity = clamp((theta - spot_light_cutOff)/epsilon ,0.0,1.0);

    
    //ambient
	vec3 ambient = vec3(light_ambient_coefficient) *  light_intensities;

    // diffuse
	float diffCoefficient = max(dot(lightDir,norm),0.0);
	v_diffuse = diffCoefficient  * light_intensities  * 0.65 + ambient  ;

 	//specular
     vec3 reflectDir = normalize(reflect(-lightDir,norm));
     float specCoefficient = pow(max(dot(norm,halfwayDir),0.0),material_shininess ) ;
     v_spec = specCoefficient * light_intensities * light_specular  * material_specular  ;

 	TextureCoordOut = a_texCoord;
     TextureCoordOut.y = 1.0 - TextureCoordOut.y;

 	vec4 v_FragPosLightSpace = u_LightMatrix * vec4(vec3(u_MMatrix * position),1);

 	v_projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;
 	// transform to [0,1] range
     v_projCoords = v_projCoords * 0.5 + 0.5;

 	gl_Position = CC_PMatrix * vec4(v_FragPos,1);


 }