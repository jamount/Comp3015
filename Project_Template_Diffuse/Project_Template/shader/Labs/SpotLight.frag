#version 450

in vec3 Position;
in vec3 Normal;

uniform struct SpotLightInfo {
	vec3 Position; // Position in cam coords
	vec3 L; // Diffuse/spec intensity
	vec3 La; // Amb intensity
	vec3 Direction; // Direction of the spotlight in cam coords.
	float Exponent; // Angular attenuation exponent
	float Cutoff; // Cutoff angle (between 0 and pi/2)
} Spot;

uniform struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
}Material;

layout( location = 0 ) out vec4 FragColor;

vec3 blinnPhongSpot( vec3 position, vec3 n ) {
	//ambient
	vec3 ambient = Material.Ka * Spot.La;


	vec3 s = normalize(vec3(Spot.Position) - position);

	float cosAng = dot(-s, normalize(Spot.Direction)); //cosine of the angle
	float angle = acos( cosAng ); //gives you the actual angle
	float spotScale = 0.0;

	if(angle < Spot.Cutoff )
	{
		spotScale = pow( cosAng, Spot.Exponent );
		float sDotN =  max( dot(s,n), 0.0 );
		vec3 diffuse = Material.Kd * sDotN;

		vec3 spec = vec3(0.0);

		if( sDotN > 0.0 )
		{
			vec3 v = normalize (-position.xyz);
			vec3 h = normalize(v + s);

			spec = Material.Ks * pow( max( dot(h, n), 0.0), Material.Shininess );
		}

		return ambient + spotScale * Spot.L * (diffuse + spec);

	}
}

void main()
{
	FragColor = vec4(blinnPhongSpot(Position, normalize(Normal)), 1);
}