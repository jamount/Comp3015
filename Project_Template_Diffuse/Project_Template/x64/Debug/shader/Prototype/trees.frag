#version 450


in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout(binding=0) uniform sampler2D Tex1;
uniform struct FogInfo
{
	float MaxDist; //max distance
	float MinDist; //min distance
	vec3 Color; //colour of the fog
} Fog;


uniform struct LightInfo{
	vec3 Position;
	vec3 La;
	vec3 L;
}light[3];

uniform struct MaterialInfo{

	vec3 emission;
	vec3 Ks;
	float Shininess;
}Material;



layout( location = 0 ) out vec4 FragColor;



vec3 blinnPhong(int i, vec3 position, vec3 n)
{
	vec3 texColor = texture(Tex1, TexCoord).rgb;

	//ambient
	vec3 ambient = texColor * (light[i].La );

	//diffuse
	vec3 s = normalize(vec3(light[i].Position) - position);
	float sDotN =  max( dot(s,n), 0.0 );
	vec3 diffuse = texColor * sDotN;

	//specular
	vec3 spec = vec3(0.0);

	if(sDotN > 0.0){
		vec3 v = normalize (-position.xyz);
		vec3 h = normalize(v + s);

		spec = Material.Ks * pow( max( dot(h, n), 0.0), Material.Shininess );
	}

	//attenuateLight

	float d = length(light[i].Position - position);
	float attenuation = clamp(1.0f / d*d, 0.00, 1.0);
	vec3 light =  light[i].L * attenuation;

	return ambient + light * (diffuse + spec) + Material.emission;
}

void main(){

	float dist = abs( Position.z ); 

	float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
	fogFactor = clamp( fogFactor, 0.0, 1.0 ); 
	vec3 shadeColor = vec3(0.0);	for(int i = 0; i<3;i++){		shadeColor += blinnPhong(i, Position, normalize(Normal));	}	vec3 color = mix( Fog.Color, shadeColor, fogFactor );
	FragColor = vec4(color, 1.0); //final colour 
}
