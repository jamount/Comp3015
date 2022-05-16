#pragma once
#include <glad/glad.h>
#include "glslprogram.h"
#include <glm/glm.hpp>
#include "random.h"
#include "texture.h"
#include "particleutils.h"
class ParticleEmitterFountain {
private:
	GLSLProgram prog;

	Random rand;

	GLuint initVel, StartTime, Particles, nParticles;

	glm::vec3 emitterPos, emitterDir;

	float angle, time, particleLifetime;


	void init();
	void initBuffers();
	float randFloat();

public:
	ParticleEmitterFountain();

	void Update(float dt);
	void Render(glm::mat4 proj, glm::mat4 view);
};