#include "scenebasic_uniform.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;
using glm::vec4;




//constructor for torus
SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 8.0f),
terrain(50.0f, 50.0f, 20, 20)
{
	tallTree = ObjMesh::loadWithAdjacency("../Project_Template/media/tall tree.obj",
		true);
	rock = ObjMesh::loadWithAdjacency("../Project_Template/media/Rock_01.obj",
		true);
	sword = ObjMesh::loadWithAdjacency("../Project_Template/media/Sword.obj",
		true);
	crystal = ObjMesh::loadWithAdjacency("../Project_Template/media/Crystal_02.obj",
		true);


	
}






void SceneBasic_Uniform::initScene()
{
	compile();
	glClearColor(0.3f, 0.3f, 0.6f, 1.0f);
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);
	angle = 0.0f;

	setupFBO();

	shadowRender.use();	shadowRender.setUniform("LightIntensity", vec3(1.0f));
	GLfloat verts[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
	GLuint bufHandle;
	glGenBuffers(1, &bufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);


	glGenVertexArrays(1, &fsQuad);
	glBindVertexArray (fsQuad);

	glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0); 
	glBindVertexArray(0);

	//Load textures 
	glActiveTexture(GL_TEXTURE2);
	texTree = Texture::loadTexture("../Project_Template/media/tree textures/Colorsheet Tree Cold.png");
	texRock = Texture::loadTexture("../Project_Template/media/texture/MeshDino_Cyan.png");
	texGrass = Texture::loadTexture("../Project_Template/media/texture/grass.png");

	updateLight();

	shadowRender.use();
	shadowRender.setUniform("Tex", 2);

	shadowComp.use();
	shadowComp.setUniform("DiffSpecTex", 0);

	this->animate(true);




	objectPos = terrain.getobjPos();
	for (size_t i = 0; i < objectPos.size(); i++)
	{
		Object obj;
		
		int random = rand() % 7;

		if (random <= 4) {
			obj.pos = objectPos[i] + glm::vec3(-25.0f, 3.0f, -25.0f);
			obj.type = objectType(Tree);

		}
		else if(random <=7) {
			obj.pos = objectPos[i] + glm::vec3(-25.0f, -1.0f, -25.0f);
			obj.type = objectType(Rock);
		}


		objects.push_back( obj);
	}
}

void SceneBasic_Uniform::compile()
{
	try {
		//main shader for lighting objects
		shadowVolume.compileShader("shader/Shadows/ShadowVolumes.vert");
		shadowVolume.compileShader("shader/Shadows/ShadowVolumes.frag");
		shadowVolume.compileShader("shader/Shadows/ShadowVolumes.geom");
		shadowVolume.link();


		shadowRender.compileShader("shader/Shadows/ShadowRender.vert");
		shadowRender.compileShader("shader/Shadows/ShadowRender.frag");
		shadowRender.link();

		shadowComp.compileShader("shader/Shadows/ShadowComp.vert");
		shadowComp.compileShader("shader/Shadows/ShadowComp.frag");
		shadowComp.link();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	 
}

void SceneBasic_Uniform::update( float delta )
{



	if (animating()) {
		angle += delta * 0.1f;
		
		if (angle > glm::two_pi<float>()) {
			angle -= glm::two_pi<float>();
		}
		updateLight();
	}
}



void SceneBasic_Uniform::render( glm::mat4 projection, glm::mat4 view)
{
	this->view = view;
	this->projection = projection;
	pass1();
	glFlush();
	pass2();
	glFlush();
	pass3();


	

}
void SceneBasic_Uniform::pass1() {	glDepthMask(GL_TRUE);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	/*projection = glm::infinitePerspective(glm::radians(50.0f), (float)width / height, 0.5f);
	view = glm::lookAt(vec3(5.0f), vec3(0, 2, 0), vec3(0, 1, 0));*/

	shadowRender.use();
	shadowRender.setUniform("LightPosition",  lightPos);

	glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	drawScene(shadowRender, false);}void SceneBasic_Uniform::pass2() {	shadowVolume.use();
	shadowVolume.setUniform("LightPosition", lightPos);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, colorDepthFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width - 1, height - 1, 0, 0, width - 1, height - 1,
		GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);


	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask (GL_FALSE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, 0xffff); 
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	glStencilOpSeparate (GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);


	drawScene(shadowVolume, true);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void SceneBasic_Uniform::pass3() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glStencilFunc(GL_EQUAL, 0,0xffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	shadowComp.use();

	model = mat4(1.0f);
	projection = model;
	view = model;
	setMatrices(shadowComp);

	glBindVertexArray(fsQuad);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void SceneBasic_Uniform::setMatrices(GLSLProgram& prog)
{
	mat4 mv = view * model;
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("ProjMatrix", projection);
	prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

}

void SceneBasic_Uniform::drawScene(GLSLProgram& prog, bool onlyShadowCasters) {
	
	vec3 color;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(!onlyShadowCasters){
		glActiveTexture(GL_TEXTURE2);

		color = vec3(1.0f);
		prog.setUniform("Ka", color * 0.1f);
		prog.setUniform("Kd", color);
		prog.setUniform("Ks", vec3(0.9f));
		prog.setUniform("Shininess", 150.0f);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		drawObject(objects[i], prog);
	}


	if (!onlyShadowCasters) {
		glBindTexture(GL_TEXTURE_2D, texGrass);

		color = vec3(0.5f);
		prog.setUniform("Kd", color);
		prog.setUniform("Ks", vec3(0.0f));
		prog.setUniform("Ka", vec3(0.1f));
		prog.setUniform("Shininess", 1.0f);
		model = mat4(1.0f);
		model = glm::scale(model, vec3(1.5f, 1.0f, 1.5f));
		setMatrices(prog);
		terrain.render();
	}


}

void SceneBasic_Uniform::drawObject(Object obj, GLSLProgram& prog) {



		if(obj.type == objectType(Tree)) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texTree);
			prog.setUniform("Shininess", 1.0f);
			model = mat4(1.0f);
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			model = glm::translate(model, obj.pos);
			setMatrices(prog);
			tallTree->render();
		}

		
		if( obj.type == objectType(Rock)) {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texRock);
			model = mat4(1.0f);
			model = glm::translate(model, obj.pos);
			model = glm::scale(model, glm::vec3(3.5f, 5.5f, 3.5f));
			setMatrices(prog);
			rock->render();
		}


}

void SceneBasic_Uniform::updateLight() {
	lightPos = vec4(5.0f * vec3(cosf(angle) * 7.5f, 1.5f, sinf(angle) * 7.5f), 1.0f);
}

void SceneBasic_Uniform::setupFBO() {

	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// The ambient buffer
	GLuint ambBuf;
	glGenRenderbuffers(1, &ambBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, ambBuf);
	glRenderbufferStorage (GL_RENDERBUFFER, GL_RGBA, width, height);

	// The diffuse+specular component
	glActiveTexture(GL_TEXTURE0);
	GLuint diffSpecTex;
	glGenTextures(1, &diffSpecTex);
	glBindTexture(GL_TEXTURE_2D, diffSpecTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create and set up the FBO
	glGenFramebuffers(1, &colorDepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ambBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffSpecTex, 0);
	
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is complete. \n");
	}
	else {
		printf("Framebuffer is not complete. \n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void SceneBasic_Uniform::skiptime() {
	printf("skipping time");
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}


