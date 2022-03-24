#include "scenebasic_uniform.h"
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
#include "helper/texture.h"

//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform() 
{ }






void SceneBasic_Uniform::initScene()
{
	compile();
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(1.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
	

	prog.setUniform("light.La", vec3(0.1f));
	prog.setUniform("light.L", vec3(0.75f));

	GLuint brick =
		Texture::loadTexture("../Project_Template/media/texture/brick1.jpg");
	GLuint moss =
		Texture::loadTexture("../Project_Template/media/texture/moss.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brick);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moss);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/blinn-phong.vert");
		prog.compileShader("shader/texture.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{

}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec4 lightPos = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	prog.setUniform("light.Position", vec3(view * lightPos));

	//prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
	//prog.setUniform("Material.Ks", vec3(0.1f, 0.1f, 0.1f));
	//prog.setUniform("Material.Ka", vec3(0.0f, 0.0f, 0.0f));
	prog.setUniform("Material.Shininess", 10.0f);

	model = mat4(1.0f);
	//model = glm::translate(model, vec3(0.0f, 0.0f, -2.0f));
	//model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	setMatrices();
	cube.render();
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //we create a model view matrix
    
    prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
