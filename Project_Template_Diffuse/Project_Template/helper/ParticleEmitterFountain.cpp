#include "ParticleEmitterFountain.h"
#include <glm\gtc\matrix_transform.hpp>

ParticleEmitterFountain::ParticleEmitterFountain() : time(0), particleLifetime(5.5), nParticles(5000),
									emitterPos(1, 0, 5), emitterDir(-1, 2, 0)
{
	init();

}

void ParticleEmitterFountain::init() {

	prog.compileShader("../Project_Template/shader/Particles/Particle.vert");
	prog.compileShader("../Project_Template/shader/Particles/Particle.frag");
	prog.link();

	angle = glm::half_pi<float>();
	initBuffers();

	prog.use();
	GLuint texID = Texture::loadTexture("../Project_Template/media/particles/fire.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);


	prog.setUniform("ParticleTex", 0);
	prog.setUniform("ParticleLifetime", particleLifetime);
	prog.setUniform("ParticleSize", 0.05f);
	prog.setUniform("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
	prog.setUniform("EmiterPos", emitterPos);
}

void ParticleEmitterFountain::initBuffers()
{
	glGenBuffers(1, &initVel);
	glGenBuffers(1, &StartTime);

	int size = nParticles * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, StartTime);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);

	glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	std::vector<GLfloat> data(nParticles * 3);
	for (uint32_t i = 0; i < nParticles; i++)
	{
		theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
		phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(1.25f, 1.5f, randFloat());
		v = glm::normalize(emitterBasis * v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());

	float rate = particleLifetime / nParticles;
	for (int i = 0; i < nParticles; i++)
	{
		data[i] = rate * i;
	}
	glBindBuffer(GL_ARRAY_BUFFER, StartTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &Particles);
	glBindVertexArray(Particles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, StartTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glBindVertexArray(0);
}

float ParticleEmitterFountain::randFloat() {
	return rand.nextFloat();
}

void ParticleEmitterFountain::Update(float dt) {
	time += dt;
	angle = std::fmod(angle + 0.01f, glm::two_pi<float>());
}

void ParticleEmitterFountain::Render(glm::mat4 proj, glm::mat4 view) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLuint texID = Texture::loadTexture("../Project_Template/media/particles/fire.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glDepthMask(GL_FALSE);
	prog.use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mv = view * model;
	prog.setUniform("MV", mv);
	prog.setUniform("Proj", proj);
	prog.setUniform("Time", time);

	glBindVertexArray(Particles);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}