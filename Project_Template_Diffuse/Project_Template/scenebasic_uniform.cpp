#include "scenebasic_uniform.h"
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::vec4;
#include "helper/texture.h"



//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f), 
plane(10.0f, 10.0f, 100, 100)
{
	tallTree = ObjMesh::load("../Project_Template/media/tall tree.obj",
		true);
	rock = ObjMesh::load("../Project_Template/media/Rock_01.obj",
		true);
	sword = ObjMesh::load("../Project_Template/media/Sword.obj",
		true);
	crystal = ObjMesh::load("../Project_Template/media/Crystal_02.obj",
		true);



}






void SceneBasic_Uniform::initScene()
{
	compile();
	glClearColor(0.5f, 0.35f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	 	//view = glm::lookAt(vec3(0.5f, 0.75f, 0.75f), vec3(0.0f, 0.0f, 0.0f),
		//vec3(0.0f, 1.0f, 0.0f));
	view = mat4(1.0f);
	view = glm::translate(view, vec3(0.0f, 0.0f, -2.0f));
	projection = mat4(1.0f);
	float x, z;

	prog.setUniform("Fog.MaxDist", 15.0f);
	prog.setUniform("Fog.MinDist", 0.0f);
	prog.setUniform("Fog.Color", vec3(0.5f, 0.35f, 0.7f));

	prog.setUniform("light[0].Position", vec3(-5.0f, 2.4f, -3.9f));
	prog.setUniform("light[1].Position", vec3(0.0f, 5.8f, 0.0f));
	prog.setUniform("light[2].Position", vec3(0.8f, 5.8f, 0.0f));

	prog.setUniform("light[0].L", vec3( 0.2f));
	prog.setUniform("light[1].L", vec3( 0.0f));
	prog.setUniform("light[2].L", vec3( 0.0f));	prog.setUniform("light[0].La", vec3(0.05f));
	prog.setUniform("light[1].La", vec3(0.05f));
	prog.setUniform("light[2].La", vec3(0.05f));
	GLuint texID =
		Texture::loadTexture("../Project_Template/media/tree textures/Colorsheet Tree Cold.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/Prototype/trees.vert");
		prog.compileShader("shader/Prototype/trees.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	prog.setUniform("light[0].L", 0.27f + vec3(cos(angle)/4));
	prog.setUniform("Fog.MaxDist", 12.5f + cos(angle)* 2.5f);



	float deltaT = t - tPrev;
	if (tPrev == 0.0f)
		deltaT = 0.0f;
	tPrev = t;
	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>())
		angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
	GLuint texID =
		Texture::loadTexture("../Project_Template/media/tree textures/Colorsheet Tree Cold.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.emission", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Shininess", 5.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(2.0f, 0.0f, -7.0f));
	model = glm::scale(model, vec3(1.3f));
	model = glm::rotate(model, glm::radians(250.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);	model = glm::translate(model, vec3(-4.0f, 0.0f, -6.0f));	model = glm::scale(model, vec3(1.2f));
	model = glm::rotate(model, glm::radians(190.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);	model = glm::translate(model, vec3(5.5f, 0.0f, -6.0f));	model = glm::scale(model, vec3(1.2f));
	model = glm::rotate(model, glm::radians(220.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);	model = glm::translate(model, vec3(-5.0f, 0.0f, -4.0f));	model = glm::scale(model, vec3(1.3f));
	model = glm::rotate(model, glm::radians(170.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);	model = glm::translate(model, vec3(4.7f, 0.0f, -3.0f));	model = glm::scale(model, vec3(1.25f));
	model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);
	model = glm::translate(model, vec3(-1.8f, 0.0f, -6.6f));
	model = glm::scale(model, vec3(1.6f));
	model = glm::rotate(model, glm::radians(30.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);
	model = glm::translate(model, vec3(-0.2f, 0.0f, -9.0f));
	model = glm::scale(model, vec3(2.0f));
	model = glm::rotate(model, glm::radians(50.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);
	model = glm::translate(model, vec3(4.0f, 0.0f, -9.7f));
	model = glm::scale(model, vec3(2.0f));
	setMatrices();
	tallTree->render();	model = mat4(1.0f);
	model = glm::translate(model, vec3(-7.0f, 0.0f, -8.0f));
	model = glm::scale(model, vec3(2.0f));
	setMatrices();
	tallTree->render();
	prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.emission", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Shininess", 5.0f);
	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -3.0f, -4.0f));
	model = glm::scale(model, vec3(2.0f));
	setMatrices();
	plane.render();	//sword in the stone	 texID =
		Texture::loadTexture("../Project_Template/media/MeshDino_Cyan.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);



	prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.emission", 0.0f, 0.0f, 0.0f);
	prog.setUniform("Material.Shininess", 300.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(0.0f, -2.7f, -3.9f));
	model = glm::scale(model, vec3(2.2f));
	model = glm::rotate(model, glm::radians(20.0f), vec3(0.0f, 1.0f, 0.0f));
	setMatrices();
	rock->render();



		model = mat4(1.0f);
		model = glm::translate(model, vec3(0.0f, -2.4f, -3.9f));
		model = glm::scale(model, vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), vec3(1.0f, 0.0f, 0.0f));



	setMatrices();
	sword->render();



	prog.setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
	prog.setUniform("Material.emission", 0.2f, 0.1f, 0.4f);
	prog.setUniform("Material.Shininess", 300.0f);

	model = mat4(1.0f);
	model = glm::translate(model, vec3(sin(angle), 0.5f, + cos(angle)-3.9f));
	model = glm::scale(model, vec3(0.5f));
	prog.setUniform("light[0].Position", vec3(sin(angle), 0.5f, +cos(angle) - 3.9f));
	setMatrices();
	crystal->render();

	model = mat4(1.0f);
	model = glm::translate(model, vec3(sin(angle +180), 0.5f, +cos(angle+180) - 3.9f));
	model = glm::scale(model, vec3(0.5f));
	prog.setUniform("light[2].Position", vec3(sin(angle+180), 0.5f, +cos(angle+180) - 3.9f));
	setMatrices();
	crystal->render();

	model = mat4(1.0f);
	model = glm::translate(model, vec3(sin(angle+360), 0.5f, +cos(angle+360) - 3.9f));
	model = glm::scale(model, vec3(0.5f));
	prog.setUniform("light[3].Position", vec3(sin(angle + 360), 0.5f, +cos(angle + 360) - 3.9f));
	setMatrices();
	crystal->render();
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


