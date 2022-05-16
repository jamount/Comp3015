#version 450


in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout(binding=0) uniform sampler2D BrickTex;
layout(binding=1) uniform sampler2D MossTex;


uniform struct LightInfo{
	vec4 Position;
	vec3 La;
	vec3 L;
}light;

uniform struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
}Material;

//uniforms for matrices required in the shader
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 MVP;				//model view projection matrix

layout( location = 0 ) out vec4 FragColor;

vec3 blinnPhong(vec3 position, vec3 n)
{
	//texture
	vec4 BrickTexColor = texture(BrickTex, TexCoord);	vec4 MossTexColor = texture(MossTex, TexCoord);	vec3 texColor = mix(BrickTexColor.rgb, MossTexColor.rgb, MossTexColor.a);

	//ambient
	vec3 ambient = texColor * light.La;

	//diffuse
	vec3 s = normalize(vec3(light.Position) - position);
	float sDotN =  max( dot(s,n), 0.0 );
	vec3 diffuse = texColor * sDotN;

	//specular
	vec3 spec = vec3(0.0);

	if(sDotN > 0.0){
		vec3 v = normalize (-position.xyz);
		vec3 h = normalize(v + s);

		spec = Material.Ks * pow( max( dot(h, n), 0.0), Material.Shininess );
	}

	return ambient + light.L * (diffuse + spec);
}

void main(){
	FragColor = vec4(blinnPhong(Position, normalize(Normal)), 1);
}