#include "Fire.h"
#include <glm\gtc\matrix_transform.hpp>

Fire::Fire(glm::vec3 pos, float rot, float scale) : angle(0.0f), drawBuf(1), time(0), deltaT(0), nParticles(100),
				particleLifetime(2.0f), emitterPos(pos), emitterDir(0, 2, 0)
{
	position = pos;
	rotation = rot;
	this->scale = scale;
	init();
}

void Fire::init() {
	prog.compileShader("shader/Particles/Fire.vert");
	prog.compileShader("Particles/Fire.frag");

	GLuint progHandle = prog.getHandle();
	const char* outputNames[] = { "Position", "Velocity", "Age" };
	glTransformFeedbackVaryings(progHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);

	prog.link();

	glActiveTexture(GL_TEXTURE0);
	Texture::loadTexture("media / particles / fire.png");

	glActiveTexture(GL_TEXTURE1);
	ParticleUtils::createRandomTex1D(nParticles * 3);

	initBuffers();

	prog.use();
	prog.setUniform("RandomTex", 1);
	prog.setUniform("ParticleTex", 0);
	prog.setUniform("ParticleLifetime", particleLifetime);
	prog.setUniform("Accel", glm::vec3(0.0f, 0.0f, 0.0f));
	prog.setUniform("ParticleSize", 0.3f);
	prog.setUniform("Emitter", emitterPos);
	prog.setUniform("EmitterBasis", ParticleUtils::makeArbitraryBasis(emitterDir));

}

void Fire::initBuffers() {

	glGenBuffers(2, posBuf);
	glGenBuffers(2, velBuf);
	glGenBuffers(2, age);

	int size = nParticles * 3 * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(GLfloat), 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(GLfloat), 0, GL_DYNAMIC_COPY);

	std::vector<GLfloat> tempData(nParticles);
	float rate = particleLifetime / nParticles;
	for (int i = 0; i < nParticles; i++)
	{ 
		tempData[i] = rate * (i - nParticles);
	}

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tempData.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(2, particleArray);

	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glGenTransformFeedbacks(2, feedback);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void Fire::Update(float dt) {
	time += dt;
	deltaT = dt;
}

void Fire::Render(glm::mat4 proj, glm::mat4 view, glm::mat4 model) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);


	prog.use();
	prog.setUniform("Time", time);
	prog.setUniform("DeltaT", deltaT);

	//update pass
	prog.setUniform("Pass", 1);

	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
	glBeginTransformFeedback(GL_POINTS);

	glBindVertexArray(particleArray[1 - drawBuf]);
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);

	glDrawArrays(GL_POINTS, 0, nParticles);
	glBindVertexArray(0);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);

	//Render pass
	prog.setUniform("Pass", 2);


	glm::mat4 mv = view * model;
	prog.setUniform("MV", mv);
	prog.setUniform("Proj", proj);

	glDepthMask(GL_FALSE);
	glBindVertexArray(particleArray[drawBuf]);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	drawBuf = 1 - drawBuf; 

}