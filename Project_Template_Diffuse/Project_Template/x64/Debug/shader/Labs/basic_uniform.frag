#version 450

//in variable that receives the diffuse calculation from the vertex shader
in vec3 ambient;
in vec3 LightIntensity;
in vec3 ReflectionIntensity;

//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;

void main()
{
    //we pass LightInyensity to outr FragColor, notice the difference between vector types
    // vec3 and vec4 and how we solved the problem

    FragColor = normalize(vec4(ambient,1.0)+ vec4(LightIntensity, 1.0) + vec4(ReflectionIntensity, 1.0));

	//FragColor = vec4(normalize(ambient + LightIntensity + ReflectionIntensity), 1.0);

}
