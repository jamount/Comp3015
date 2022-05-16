#pragma once
#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "random.h"
#include "texture.h"
#include "particleutils.h"
class Fire {
private:
	GLSLProgram prog;

	Random rand;

	glm::vec3 emitterPos, emitterDir;

	GLuint posBuf[2], velBuf[2], age[2];

	GLuint particleArray[2];

	GLuint feedback[2];

	GLuint drawBuf;

	int nParticles;
	float particleLifetime;
	float angle;
	float time, deltaT;

	glm::vec3 position;
	float rotation;
	float scale;

	void init();
	void initBuffers();

public:
	Fire(glm::vec3 pos, float rot, float scale);

	void Update(float dt);
	void Render(glm::mat4 proj, glm::mat4 view, glm::mat4 model);
};